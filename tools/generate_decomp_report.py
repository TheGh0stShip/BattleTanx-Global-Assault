#!/usr/bin/env python3
"""Generate an objdiff v2 progress report for decomp.dev.

The report is derived from the reviewed function catalogue and the production
splat layout. A function counts as matched only when its entry point belongs to
a production C subsegment; the byte-exact reconstruction gate separately
proves that those C subsegments reproduce the supported ROM.
"""

from __future__ import annotations

import argparse
import json
import re
import tomllib
from pathlib import Path


ROM_VRAM_DELTA = 0x80070000
ROM_CODE_END = 0x101000
SUBSEGMENT = re.compile(
    r"^\s*- \[(0x[0-9A-Fa-f]+),\s*([^,\]]+)(?:,\s*([^\]]+))?\]\s*(?:#.*)?$"
)


def percent(part: int, total: int) -> float:
    return 0.0 if total == 0 else part * 100.0 / total


def measures(functions: list[dict], matched: bool | None = None) -> dict:
    total_code = sum(item["size"] for item in functions)
    if matched is None:
        matched_items = [item for item in functions if item["matched"]]
    else:
        matched_items = functions if matched else []
    matched_code = sum(item["size"] for item in matched_items)
    matched_functions = len(matched_items)
    return {
        "fuzzy_match_percent": percent(matched_code, total_code),
        "total_code": str(total_code),
        "matched_code": str(matched_code),
        "matched_code_percent": percent(matched_code, total_code),
        "total_functions": len(functions),
        "matched_functions": matched_functions,
        "matched_functions_percent": percent(matched_functions, len(functions)),
        "complete_code": str(matched_code),
        "complete_code_percent": percent(matched_code, total_code),
        "total_units": 0,
        "complete_units": 0,
    }


def load_functions(path: Path) -> list[dict]:
    document = tomllib.loads(path.read_text())
    functions = []
    for section in document["section"]:
        for function in section["functions"]:
            functions.append(
                {
                    "name": function["name"],
                    "vram": function["vram"],
                    "size": function["size"],
                }
            )
    return sorted(functions, key=lambda item: item["vram"])


def load_segments(path: Path) -> list[dict]:
    segments = []
    for line in path.read_text().splitlines():
        match = SUBSEGMENT.match(line)
        if not match:
            continue
        start = int(match.group(1), 16)
        kind = match.group(2).strip()
        name = (match.group(3) or f"{kind}_{start:06X}").strip()
        segments.append({"start": start, "kind": kind, "name": name})
    segments.sort(key=lambda item: item["start"])
    for index, segment in enumerate(segments):
        segment["end"] = (
            segments[index + 1]["start"] if index + 1 < len(segments) else ROM_CODE_END
        )
    return segments


def build_report(functions_path: Path, splat_path: Path) -> dict:
    functions = load_functions(functions_path)
    segments = load_segments(splat_path)
    if not functions or not segments:
        raise ValueError("function catalogue and splat layout must not be empty")

    report_units = []
    all_functions = []
    assigned = set()
    for segment in segments:
        start_vram = ROM_VRAM_DELTA + segment["start"]
        end_vram = ROM_VRAM_DELTA + segment["end"]
        members = [item for item in functions if start_vram <= item["vram"] < end_vram]
        if not members:
            continue
        is_c = segment["kind"] == "c"
        for item in members:
            item["matched"] = is_c
            assigned.add(item["vram"])
        unit_measures = measures(members, is_c)
        unit_measures["total_units"] = 1
        unit_measures["complete_units"] = int(is_c)
        report_units.append(
            {
                "name": segment["name"],
                "measures": unit_measures,
                "functions": [
                    {
                        "name": item["name"],
                        "size": str(item["size"]),
                        "fuzzy_match_percent": 100.0 if is_c else 0.0,
                        "address": str(item["vram"]),
                        "metadata": {"virtual_address": str(item["vram"])},
                    }
                    for item in members
                ],
                "metadata": {
                    "complete": is_c,
                    "source_path": (
                        f"src/{segment['name']}.c" if is_c else f"asm/us/{segment['name']}.s"
                    ),
                    "progress_categories": ["code"],
                },
            }
        )
        all_functions.extend(members)

    missing = [item for item in functions if item["vram"] not in assigned]
    if missing:
        names = ", ".join(item["name"] for item in missing[:5])
        raise ValueError(f"{len(missing)} catalogue functions are outside splat subsegments: {names}")

    overall = measures(all_functions)
    overall["total_units"] = len(report_units)
    overall["complete_units"] = sum(
        1 for unit in report_units if unit["metadata"]["complete"]
    )
    category = dict(overall)
    return {
        "measures": overall,
        "units": report_units,
        "version": 2,
        "categories": [{"id": "code", "name": "Code", "measures": category}],
    }


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--functions", type=Path, default=Path("config/us/recomp_function_boundaries.toml"))
    parser.add_argument("--splat", type=Path, default=Path("config/us/splat.yaml"))
    parser.add_argument("--output", type=Path, default=Path("build/us/report.json"))
    args = parser.parse_args()
    report = build_report(args.functions, args.splat)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    values = report["measures"]
    print(
        f"Wrote {args.output}: {values['matched_functions']}/{values['total_functions']} functions, "
        f"{values['matched_code']}/{values['total_code']} function bytes"
    )


if __name__ == "__main__":
    main()
