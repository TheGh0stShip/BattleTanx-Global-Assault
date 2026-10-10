#!/usr/bin/env python3
"""Account for every byte in the BattleTanx: Global Assault retail ROM."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

from inventory_libmus_assets import read_file_table
from inventory_lzari_assets import (
    ROM_SHA1,
    image_ranges,
    inventory_ranges,
    level_world_ranges,
)


CAMPAIGN_RANGE = re.compile(
    r"\{ 0x00000001, 0xB0([0-9A-Fa-f]{6}), "
    r"0xB0([0-9A-Fa-f]{6}), 0x00000000 \}"
)

STALE_SONG_DUPLICATES = (
    (0xC0000, 18),
    (0xD0000, 10),
    (0xD8000, 15),
    (0xE0000, 21),
    (0xF0000, 7),
    (0xF8000, 6),
)


def stale_build_regions(rom: bytes) -> list[dict]:
    """Split stale linker material around six exact live libmus songs."""

    files = {item["index"]: item for item in read_file_table(rom)}
    regions = [
        {
            "kind": "stale_padding",
            "name": "stale_zero_prefix",
            "start": 0xB8230,
            "end": 0xB8400,
        },
        {
            "kind": "stale_debug_symbols",
            "name": "stale_debug_symbols",
            "start": 0xB8400,
            "end": 0xC0000,
        },
    ]
    cursor = 0xC0000
    for duplicate_number, (start, file_index) in enumerate(STALE_SONG_DUPLICATES):
        source = files[file_index]
        end = start + source["size"]
        if cursor < start:
            regions.append(
                {
                    "kind": "stale_build_material",
                    "name": f"stale_build_material_{duplicate_number:02d}",
                    "start": cursor,
                    "end": start,
                }
            )
        regions.append(
            {
                "kind": "stale_song_duplicate",
                "name": f"stale_song_duplicate_{duplicate_number:02d}",
                "start": start,
                "end": end,
                "duplicate_of_file": file_index,
                "duplicate_of_start": source["start"],
            }
        )
        cursor = end
    if cursor < 0x100000:
        regions.append(
            {
                "kind": "stale_build_material",
                "name": "stale_build_material_06",
                "start": cursor,
                "end": 0x100000,
            }
        )
    return regions


def script_ranges(path: Path) -> list[dict]:
    ranges = [
        {
            "kind": "script",
            "name": f"campaign_script_{index:02d}",
            "start": int(start, 16),
            "end": int(end, 16),
        }
        for index, (start, end) in enumerate(CAMPAIGN_RANGE.findall(path.read_text()))
    ]
    ranges.extend(
        (
            {"kind": "script", "name": "cheat_cutscene", "start": 0x564C78, "end": 0x580EF1},
            {"kind": "script", "name": "ending_script", "start": 0x581C00, "end": 0x58FAD1},
        )
    )
    return sorted(ranges, key=lambda item: item["start"])


def declared_regions(rom: bytes, boundaries: Path, campaign: Path) -> list[dict]:
    regions = [
        {"kind": "header", "name": "rom_header", "start": 0, "end": 0x40},
        {"kind": "ipl3", "name": "ipl3", "start": 0x40, "end": 0x1000},
        {"kind": "main_image", "name": "main_image", "start": 0x1000, "end": 0xB7E30},
        {
            "kind": "runtime_blob",
            "name": "loaded_segment_1_tail",
            "start": 0xB7E30,
            "end": 0xB8230,
        },
        {
            "kind": "leftover",
            "name": "btx1_world_leftover",
            "start": 0x100000,
            "end": 0x102068,
        },
        {"kind": "raw_buffer", "name": "raw_buffer_0", "start": 0x102068, "end": 0x102468},
        {"kind": "raw_buffer", "name": "raw_buffer_1", "start": 0x102468, "end": 0x102868},
        {"kind": "raw_buffer", "name": "raw_buffer_2", "start": 0x102868, "end": 0x102C68},
        {"kind": "texture_pool", "name": "texture_pool", "start": 0x102C70, "end": 0x2F8070},
        {"kind": "state_pool", "name": "state_pool", "start": 0x2F8070, "end": 0x3013F0},
        {"kind": "geometry_pool", "name": "geometry_pool", "start": 0x3013F0, "end": 0x3F6EE8},
        {"kind": "world", "name": "common_world", "start": 0x3F6EE8, "end": 0x3F9B5C},
    ]
    regions.extend(stale_build_regions(rom))
    regions.extend(
        {
            "kind": "world",
            "name": item["name"],
            "start": item["rom_start"],
            "end": item["rom_end"],
        }
        for item in inventory_ranges(rom, level_world_ranges(boundaries))
    )
    regions.extend(image_ranges(rom))
    for index, (start, end) in enumerate(
        ((0x4729C0, 0x472BAC), (0x472BB0, 0x472D9C), (0x472DA0, 0x472F8C), (0x472F90, 0x47317C))
    ):
        regions.append({"kind": "image_aux", "name": f"image_aux_{index}", "start": start, "end": end})
    regions.extend(script_ranges(campaign))
    regions.extend(
        {
            "kind": "audio",
            "name": f"libmus_file_{item['index']:02d}",
            "start": item["start"],
            "end": item["end"],
        }
        for item in read_file_table(rom)
    )
    regions.append(
        {"kind": "terminal_padding", "name": "terminal_ff", "start": 0x7BE9AE, "end": len(rom)}
    )
    return sorted(regions, key=lambda item: (item["start"], item["end"]))


def inventory_layout(rom: bytes, boundaries: Path, campaign: Path) -> dict:
    regions = declared_regions(rom, boundaries, campaign)
    complete = []
    cursor = 0
    padding_index = 0
    for region in regions:
        start, end = region["start"], region["end"]
        if start < cursor:
            raise ValueError(f"ROM regions overlap at 0x{start:X}")
        if start > cursor:
            padding = rom[cursor:start]
            if len(padding) > 15 or any(padding):
                raise ValueError(f"unclassified ROM bytes at 0x{cursor:X}-0x{start:X}")
            complete.append(
                {
                    "kind": "alignment_padding",
                    "name": f"alignment_{padding_index:03d}",
                    "start": cursor,
                    "end": start,
                }
            )
            padding_index += 1
        complete.append(region)
        cursor = end
    if cursor != len(rom):
        raise ValueError(f"ROM map ends at 0x{cursor:X}, expected 0x{len(rom):X}")
    if set(rom[0x7BE9AE:]) != {0xFF}:
        raise ValueError("terminal ROM padding is not uniformly 0xFF")

    for region in complete:
        payload = rom[region["start"] : region["end"]]
        region["size"] = len(payload)
        region["sha256"] = hashlib.sha256(payload).hexdigest()
        if region["kind"] == "script":
            if len(payload) < 64:
                raise ValueError(f"{region['name']} is shorter than its loader header")
            region["scene_type"] = payload[0]
        elif region["kind"] == "stale_song_duplicate":
            source_start = region["duplicate_of_start"]
            source = rom[source_start : source_start + len(payload)]
            if payload != source:
                raise ValueError(f"{region['name']} no longer matches its live libmus song")

    counts = Counter(item["kind"] for item in complete)
    sizes = Counter()
    for item in complete:
        sizes[item["kind"]] += item["size"]
    return {
        "format": "BattleTanx Global Assault complete ROM layout v1",
        "rom_size": len(rom),
        "region_count": len(complete),
        "counts": dict(sorted(counts.items())),
        "bytes": dict(sorted(sizes.items())),
        "regions": complete,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument(
        "--boundaries", type=Path, default=Path("src/code/slot_asset_ranges.c")
    )
    parser.add_argument(
        "--campaign", type=Path, default=Path("src/code/campaign_mission_config.c")
    )
    parser.add_argument("--output", type=Path, help="write metadata inventory JSON")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    actual_sha1 = hashlib.sha1(rom).hexdigest()
    if actual_sha1 != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {actual_sha1}")
    report = {"rom_sha1": actual_sha1, **inventory_layout(rom, args.boundaries, args.campaign)}
    encoded = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
