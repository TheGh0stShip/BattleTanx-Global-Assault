#!/usr/bin/env python3
"""Re-gate maintained decompilation candidates as one parallel campaign.

This intentionally orchestrates each lane's existing ``kmc_cmp.py`` rather than
inventing another compiler pipeline.  Candidate and tool content hashes make
reruns cheap, while the final table makes newly exact units and low-distance
near misses visible without invoking dozens of commands by hand.

Typical use::

    python3 tools/run_decomp_campaign.py \
        --discover claude-work/output \
        --output build/decomp-campaign/current.tsv

Only rows with a real candidate file and a lane-local comparator are run.
Nothing is copied into production and the full-ROM gate is not invoked.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import csv
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time


FUNCTION_KEYS = ("function", "func", "name")
ADDRESS_KEYS = ("unit_vram", "address", "vram", "linked_at", "unit_start")
CANDIDATE_KEYS = ("best_candidate", "candidate", "source", "source_unit")
MISMATCH_KEYS = ("mismatch_words", "mismatched_words", "after", "before")
RESULT_RE = re.compile(r"\b(UNIT MATCH|NORMALIZER_ASSISTED|DIFF\((\d+)\)|DIFF)")
TEXT_DIFF_RE = re.compile(r"^\.text\s+[^:]+:\s+(\d+)\s+mismatched words", re.MULTILINE)
HEX_RE = re.compile(r"(?:0x)?([0-9A-Fa-f]{8})")


def first_function_definition(source: str) -> re.Match[str] | None:
    return re.search(
        r"(?:^|\n)[^;{}\n]*\b(func_([0-9A-Fa-f]{8}))\s*\([^;{}]*\)\s*\{",
        source,
    )


def first(row: dict[str, str], keys: tuple[str, ...]) -> str:
    for key in keys:
        value = row.get(key, "").strip()
        if value:
            return value
    return ""


def clean_candidate(value: str) -> str:
    # Handoff tables sometimes append a parenthetical unit note.
    return value.split(" (", 1)[0].strip().strip("`")


def target_for(row: dict[str, str], function: str) -> str:
    address = first(row, ADDRESS_KEYS)
    match = HEX_RE.search(address)
    if match:
        return match.group(1).upper()
    match = re.search(r"func_([0-9A-Fa-f]{8})", function)
    return match.group(1).upper() if match else ""


def old_score(row: dict[str, str]) -> int | None:
    value = first(row, MISMATCH_KEYS)
    match = re.search(r"\d+", value)
    return int(match.group()) if match else None


def discover(root: Path) -> list[dict[str, object]]:
    jobs: list[dict[str, object]] = []
    seen: set[tuple[Path, Path, str]] = set()
    tables = sorted(root.rglob("NEAR_MISSES.tsv")) + sorted(root.rglob("MATCHES.tsv"))
    for table in tables:
        lane = table.parent
        comparator = lane / "tools" / "kmc_cmp.py"
        if not comparator.is_file():
            continue
        try:
            with table.open(newline="", errors="replace") as stream:
                rows = csv.DictReader(
                    (line for line in stream if line.strip() and not line.startswith("#")),
                    delimiter="\t",
                )
                for row in rows:
                    function = first(row, FUNCTION_KEYS)
                    candidate_text = clean_candidate(first(row, CANDIDATE_KEYS))
                    target = target_for(row, function)
                    if not candidate_text or not target:
                        continue
                    candidate = lane / candidate_text
                    if not candidate.is_file() or candidate.suffix != ".c":
                        continue
                    key = (comparator.resolve(), candidate.resolve(), target)
                    if key in seen:
                        continue
                    seen.add(key)
                    jobs.append(
                        {
                            "name": function or f"func_{target}",
                            "lane": lane,
                            "table": table,
                            "inventory": table.name,
                            "comparator": comparator,
                            "candidate": candidate,
                            "candidate_arg": candidate_text,
                            "target": target,
                            "old_score": old_score(row),
                        }
                    )
        except (OSError, csv.Error):
            continue
    # Bounded searches often leave a better ``best.c`` than the handoff table
    # names.  Include those automatically so a stale TSV cannot hide a newly
    # exact compiler output after production rules change.
    for candidate in sorted(root.glob("**/search/*/best.c")):
        lane = candidate
        while lane != root and not (lane / "tools/kmc_cmp.py").is_file():
            lane = lane.parent
        comparator = lane / "tools/kmc_cmp.py"
        if not comparator.is_file():
            continue
        source = candidate.read_text(errors="replace")
        span = re.search(r"\bSPAN\s+(?:0x)?([0-9A-Fa-f]{8})", source)
        function_match = first_function_definition(source)
        target = (span.group(1) if span else function_match.group(2) if function_match else "").upper()
        if not target:
            continue
        key = (comparator.resolve(), candidate.resolve(), target)
        if key in seen:
            continue
        seen.add(key)
        score = None
        best_json = candidate.with_name("best.json")
        if best_json.is_file():
            try:
                score = json.loads(best_json.read_text()).get("result", {}).get("words")
            except (OSError, json.JSONDecodeError):
                pass
        jobs.append(
            {
                "name": function_match.group(1) if function_match else f"func_{target}",
                "lane": lane,
                "table": best_json,
                "inventory": "SEARCH_BEST",
                "comparator": comparator,
                "candidate": candidate,
                "candidate_arg": str(candidate.relative_to(lane)),
                "target": target,
                "old_score": score,
            }
        )
    return jobs


def splat_segments(repo: Path) -> list[tuple[int, str, str]]:
    entries: list[tuple[int, str, str]] = []
    pattern = re.compile(r"- \[0x([0-9A-Fa-f]+),\s*([^,\]]+)(?:,\s*([^\]]+))?")
    for line in (repo / "config/us/splat.yaml").read_text().splitlines():
        match = pattern.search(line)
        if match:
            entries.append((int(match.group(1), 16), match.group(2).strip(), (match.group(3) or "").strip()))
    return sorted(entries)


def production_owner(target: str, segments: list[tuple[int, str, str]]) -> tuple[str, str]:
    # US .text is loaded at 0x80071000 from ROM offset 0x1000, hence the
    # constant 0x80070000 mapping used throughout this repository's catalogue.
    offset = int(target, 16) - 0x80070000
    owner: tuple[int, str, str] | None = None
    for segment in segments:
        if segment[0] > offset:
            break
        owner = segment
    return (owner[1], owner[2]) if owner else ("unknown", "")


def digest_job(job: dict[str, object], repo: Path) -> str:
    digest = hashlib.sha256()
    digest.update(Path(__file__).read_bytes())
    digest.update(Path(job["candidate"]).read_bytes())
    digest.update(Path(job["comparator"]).read_bytes())
    # Changes to production normalization/build orchestration invalidate all
    # cached narrow gates, which is precisely when a campaign rerun matters.
    for relative in ("tools/normalize_kmc_gcc_asm.py", "tools/build_code.sh"):
        path = repo / relative
        if path.is_file():
            digest.update(path.read_bytes())
    digest.update(str(job["target"]).encode())
    return digest.hexdigest()


def parse_result(output: str, returncode: int) -> tuple[str, int | None]:
    # Comparators use a non-zero exit status for an ordinary byte difference.
    # Parse their explicit RESULT line before treating the process as broken.
    matches = list(RESULT_RE.finditer(output))
    if not matches:
        return ("ERROR" if returncode else "UNKNOWN"), None
    result = matches[-1].group(1)
    if result == "UNIT MATCH":
        return "MATCH", 0
    if result == "NORMALIZER_ASSISTED":
        return "ASSISTED", 0
    if result.startswith("DIFF"):
        whole_unit = TEXT_DIFF_RE.search(output)
        if whole_unit:
            return "DIFF", int(whole_unit.group(1))
        count = matches[-1].group(2)
        if count is None:
            count = next((match.group(2) for match in reversed(matches) if match.group(2) is not None), None)
        return "DIFF", int(count) if count is not None else None
    return result, None


def run_job(job: dict[str, object], repo: Path, cache: Path) -> dict[str, object]:
    key = digest_job(job, repo)
    cached = cache / f"{key}.json"
    if cached.is_file():
        result = json.loads(cached.read_text())
        result["cached"] = True
        return result
    command = [sys.executable, "tools/kmc_cmp.py", str(job["candidate_arg"]), str(job["target"])]
    started = time.monotonic()
    proc = subprocess.run(
        command,
        cwd=Path(job["lane"]),
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        timeout=300,
    )
    status, score = parse_result(proc.stdout, proc.returncode)
    result = {
        "status": status,
        "score": score,
        "returncode": proc.returncode,
        "seconds": round(time.monotonic() - started, 3),
        "output_hash": hashlib.sha256(proc.stdout.encode()).hexdigest(),
        "tail": "\n".join(proc.stdout.splitlines()[-12:]),
        "cached": False,
    }
    cache.mkdir(parents=True, exist_ok=True)
    cached.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
    return result


def rank(row: dict[str, object]) -> tuple[int, int, str]:
    status_order = {"MATCH": 0, "ASSISTED": 1, "DIFF": 2, "UNKNOWN": 3, "ERROR": 4}
    score = row.get("score")
    return status_order.get(str(row["status"]), 5), int(score) if score is not None else 10**9, str(row["name"])


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--discover", type=Path, default=Path("claude-work/output"))
    parser.add_argument("--output", type=Path, default=Path("build/decomp-campaign/current.tsv"))
    parser.add_argument("--cache", type=Path, default=Path("build/decomp-campaign/cache"))
    parser.add_argument("--jobs", type=int, default=max(1, min(os.cpu_count() or 1, 8)))
    parser.add_argument("--max-old-score", type=int, default=200)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parent.parent
    root = (repo / args.discover).resolve() if not args.discover.is_absolute() else args.discover
    output = (repo / args.output).resolve() if not args.output.is_absolute() else args.output
    cache = (repo / args.cache).resolve() if not args.cache.is_absolute() else args.cache
    segments = splat_segments(repo)
    jobs = [job for job in discover(root) if job["old_score"] is None or int(job["old_score"]) <= args.max_old_score]
    for job in jobs:
        job["production_kind"], job["production_owner"] = production_owner(str(job["target"]), segments)
    if not jobs:
        print("no runnable maintained candidates discovered", file=sys.stderr)
        return 2
    completed: list[dict[str, object]] = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = {pool.submit(run_job, job, repo, cache): job for job in jobs}
        for future in concurrent.futures.as_completed(futures):
            job = futures[future]
            try:
                result = future.result()
            except Exception as exc:  # keep the campaign alive and report the failed target
                result = {"status": "ERROR", "score": None, "seconds": 0, "cached": False, "tail": str(exc)}
            completed.append({**job, **result})
    completed.sort(key=rank)
    output.parent.mkdir(parents=True, exist_ok=True)
    fields = (
        "status", "score", "old_score", "production_kind", "production_owner",
        "inventory", "name", "target", "candidate", "lane", "seconds", "cached", "output_hash", "tail",
    )
    with output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", extrasaction="ignore", lineterminator="\n")
        writer.writeheader()
        for row in completed:
            serial = dict(row)
            serial["candidate"] = Path(row["candidate"]).relative_to(repo)
            serial["lane"] = Path(row["lane"]).relative_to(repo)
            serial["tail"] = str(row.get("tail", "")).replace("\t", " ").replace("\n", " | ")
            writer.writerow(serial)
    counts: dict[str, int] = {}
    for row in completed:
        counts[str(row["status"])] = counts.get(str(row["status"]), 0) + 1
    print(f"campaign: {len(completed)} candidates -> {output.relative_to(repo)}")
    print(" ".join(f"{key}={counts[key]}" for key in sorted(counts)))
    actionable = [row for row in completed if row["status"] in ("MATCH", "ASSISTED") and row["production_kind"] == "asm"]
    print(f"actionable_exact_asm={len(actionable)}")
    for row in completed[:20]:
        print(f"{row['status']:8} {str(row.get('score')):>5} {row['name']:<24} {Path(row['candidate']).name}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
