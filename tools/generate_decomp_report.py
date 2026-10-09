#!/usr/bin/env python3
"""Generate an objdiff v2 progress report for decomp.dev.

The report is derived from the reviewed function catalogue and the production
splat layout. A function counts as matched only when its entry point belongs to
a production C subsegment; the byte-exact reconstruction gate separately
proves that those C subsegments reproduce the supported ROM.

Data progress covers initialized bytes in the main executable image. Cartridge
assets, RSP payloads, stale build material, padding outside that image, and BSS
are separate reconstruction concerns and are not part of decomp.dev's Data
denominator.
"""

from __future__ import annotations

import argparse
import json
import re
import tomllib
from pathlib import Path


ROM_VRAM_DELTA = 0x80070000
ROM_CODE_END = 0x101000
MAIN_IMAGE_START = 0x1000
MAIN_IMAGE_END = 0xB7E30
SUBSEGMENT = re.compile(
    r"^\s*- \[(0x[0-9A-Fa-f]+),\s*([^,\]]+)(?:,\s*([^\]]+))?\]\s*(?:#.*)?$"
)


def percent(part: int, total: int) -> float:
    return 0.0 if total == 0 else part * 100.0 / total


def empty_measures() -> dict:
    return {
        "fuzzy_match_percent": 100.0,
        "total_code": "0",
        "matched_code": "0",
        "matched_code_percent": 100.0,
        "total_data": "0",
        "matched_data": "0",
        "matched_data_percent": 100.0,
        "total_functions": 0,
        "matched_functions": 0,
        "matched_functions_percent": 100.0,
        "complete_code": "0",
        "complete_code_percent": 100.0,
        "complete_data": "0",
        "complete_data_percent": 100.0,
        "total_units": 0,
        "complete_units": 0,
    }


def measures(functions: list[dict], matched: bool | None = None) -> dict:
    total_code = sum(item["size"] for item in functions)
    if matched is None:
        matched_items = [item for item in functions if item["matched"]]
    else:
        matched_items = functions if matched else []
    matched_code = sum(item["size"] for item in matched_items)
    matched_functions = len(matched_items)
    result = empty_measures()
    result.update(
        {
            "fuzzy_match_percent": percent(matched_code, total_code),
            "total_code": str(total_code),
            "matched_code": str(matched_code),
            "matched_code_percent": percent(matched_code, total_code),
            "total_functions": len(functions),
            "matched_functions": matched_functions,
            "matched_functions_percent": percent(matched_functions, len(functions)),
            "complete_code": str(matched_code),
            "complete_code_percent": percent(matched_code, total_code),
        }
    )
    return result


def data_measures(
    total: int, matched: int, complete: int, units: int, complete_units: int
) -> dict:
    result = empty_measures()
    result.update(
        {
            "total_data": str(total),
            "matched_data": str(matched),
            "matched_data_percent": percent(matched, total),
            "complete_data": str(complete),
            "complete_data_percent": percent(complete, total),
            "total_units": units,
            "complete_units": complete_units,
        }
    )
    return result


def load_owned_data(paths: list[Path]) -> list[dict]:
    ranges = []
    for path in paths:
        section = ".rodata" if "rodata" in path.name else ".data"
        for line_number, line in enumerate(path.read_text().splitlines(), 1):
            if not line or line.startswith("#"):
                continue
            try:
                unit, address_text, size_text = line.split("\t")
                address = int(address_text, 0)
                size = int(size_text, 0)
            except (ValueError, TypeError) as error:
                raise ValueError(f"{path}:{line_number}: invalid data ownership row") from error
            if size <= 0:
                raise ValueError(f"{path}:{line_number}: data size must be positive")
            ranges.append(
                {
                    "unit": unit,
                    "address": address,
                    "end": address + size,
                    "size": size,
                    "section": section,
                    "source": path,
                }
            )

    ranges.sort(key=lambda item: item["address"])
    for previous, current in zip(ranges, ranges[1:]):
        if current["address"] < previous["end"]:
            raise ValueError(
                f"owned data ranges overlap: {previous['unit']} and {current['unit']}"
            )
    return ranges


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


def build_report(
    functions_path: Path,
    splat_path: Path,
    data_paths: list[Path] | None = None,
) -> dict:
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

    code_measures = measures(all_functions)
    code_measures["total_units"] = len(report_units)
    code_measures["complete_units"] = sum(
        1 for unit in report_units if unit["metadata"]["complete"]
    )
    if data_paths is None:
        config_dir = functions_path.parent
        data_paths = [config_dir / "unit_rodata.tsv", config_dir / "unit_data.tsv"]
    owned_data = load_owned_data(data_paths)
    tracked_vram_start = ROM_VRAM_DELTA + MAIN_IMAGE_START
    tracked_vram_end = ROM_VRAM_DELTA + MAIN_IMAGE_END
    for item in owned_data:
        if not (
            tracked_vram_start <= item["address"]
            and item["end"] <= tracked_vram_end
        ):
            raise ValueError(
                f"owned data range is outside the loaded main image: {item['unit']}"
            )
    matched_data = sum(item["size"] for item in owned_data)
    total_data = (
        MAIN_IMAGE_END
        - MAIN_IMAGE_START
        - sum(item["size"] for item in all_functions)
    )
    if matched_data > total_data:
        raise ValueError("source-owned data exceeds total non-function ROM bytes")

    for item in owned_data:
        unit_measures = data_measures(item["size"], item["size"], item["size"], 1, 1)
        report_units.append(
            {
                "name": f"data/{item['section'][1:]}/{item['unit']}",
                "measures": unit_measures,
                "sections": [
                    {
                        "name": item["section"],
                        "size": str(item["size"]),
                        "fuzzy_match_percent": 100.0,
                        "metadata": {"virtual_address": str(item["address"])},
                    }
                ],
                "functions": [],
                "metadata": {
                    "complete": True,
                    "source_path": f"src/{item['unit']}.c",
                    "progress_categories": ["data"],
                },
            }
        )

    unmatched_data = total_data - matched_data
    if unmatched_data:
        report_units.append(
            {
                "name": "data/unmatched_rom",
                "measures": data_measures(unmatched_data, 0, 0, 1, 0),
                "sections": [
                    {
                        "name": ".unmatched",
                        "size": str(unmatched_data),
                        "fuzzy_match_percent": 0.0,
                    }
                ],
                "functions": [],
                "metadata": {
                    "complete": False,
                    "progress_categories": ["data"],
                },
            }
        )

    data_category = data_measures(
        total_data,
        matched_data,
        matched_data,
        len(owned_data) + int(bool(unmatched_data)),
        len(owned_data),
    )
    overall = dict(code_measures)
    overall.update(
        {
            "total_data": data_category["total_data"],
            "matched_data": data_category["matched_data"],
            "matched_data_percent": data_category["matched_data_percent"],
            "complete_data": data_category["complete_data"],
            "complete_data_percent": data_category["complete_data_percent"],
            "total_units": code_measures["total_units"] + data_category["total_units"],
            "complete_units": code_measures["complete_units"] + data_category["complete_units"],
        }
    )
    return {
        "measures": overall,
        "units": report_units,
        "version": 2,
        "categories": [
            {"id": "code", "name": "Code", "measures": code_measures},
            {"id": "data", "name": "Data", "measures": data_category},
        ],
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
