#!/usr/bin/env python3
"""Inventory raw GEO, PB, and TEX chunks referenced by decoded worlds.

The matching world loaders treat each pool reference as an independent ROM
range. Ranges may overlap, so the inventory preserves the exact (offset, size)
pairs instead of merging them. Extracted bytes are written only when
--extract-dir is supplied; that output belongs in the ignored extraction tree.
"""

from __future__ import annotations

import argparse
import hashlib
import json
from collections import Counter
from pathlib import Path

from btga_world import POOL_LIMITS, parse_world
from inventory_lzari_assets import (
    ROM_SHA1,
    inventory_ranges,
    level_world_ranges,
)


POOL_BASES = {
    "texture": 0x102C70,
    "state": 0x2F8070,
    "geometry": 0x3013F0,
}
COMMON_WORLD = {
    "kind": "world",
    "name": "common_world",
    "start": 0x3F6EE8,
    "end": 0x3F9B5C,
}


def world_ranges(boundary_path: Path) -> list[dict]:
    return [COMMON_WORLD, *level_world_ranges(boundary_path)]


def chunk_filename(chunk: dict) -> str:
    return (
        f"{chunk['index']:04d}_{chunk['pool_offset']:06X}_"
        f"{chunk['size']:06X}.bin"
    )


def gap_filename(index: int, gap: dict) -> str:
    return f"{index:04d}_{gap['pool_offset']:06X}_{gap['size']:06X}.bin"


def coverage_ranges(chunks: list[dict], pool_size: int) -> tuple[list[dict], list[dict]]:
    """Return the union of referenced intervals and its complement."""
    merged: list[list[int]] = []
    for chunk in chunks:
        start, end = chunk["pool_offset"], chunk["pool_offset"] + chunk["size"]
        if not merged or start > merged[-1][1]:
            merged.append([start, end])
        elif end > merged[-1][1]:
            merged[-1][1] = end

    coverage = [
        {"pool_offset": start, "size": end - start} for start, end in merged
    ]
    gaps = []
    cursor = 0
    for start, end in merged:
        if cursor < start:
            gaps.append({"pool_offset": cursor, "size": start - cursor})
        cursor = end
    if cursor < pool_size:
        gaps.append({"pool_offset": cursor, "size": pool_size - cursor})
    return coverage, gaps


def inventory_pools(rom: bytes, worlds: list[dict]) -> dict:
    occurrences: dict[str, Counter[tuple[int, int]]] = {
        name: Counter() for name in POOL_BASES
    }
    parsed_worlds = [parse_world(stream["data"]) for stream in worlds]
    for world in parsed_worlds:
        for reference in world["references"]:
            for name in POOL_BASES:
                pair = (
                    reference[f"{name}_offset"],
                    reference[f"{name}_size"],
                )
                if name == "texture" and pair == (-1, -1):
                    continue
                occurrences[name][pair] += 1

    pools = []
    for name, base in POOL_BASES.items():
        chunks = []
        for index, ((offset, size), reference_count) in enumerate(
            sorted(occurrences[name].items())
        ):
            if offset < 0 or size < 0 or offset + size > POOL_LIMITS[name]:
                raise ValueError(f"{name} chunk exceeds its loader-defined pool")
            start = base + offset
            payload = rom[start : start + size]
            chunks.append(
                {
                    "index": index,
                    "pool_offset": offset,
                    "rom_start": start,
                    "rom_end": start + size,
                    "size": size,
                    "reference_count": reference_count,
                    "sha256": hashlib.sha256(payload).hexdigest(),
                }
            )
        coverage, gaps = coverage_ranges(chunks, POOL_LIMITS[name])
        for item in coverage:
            item["rom_start"] = base + item["pool_offset"]
            item["rom_end"] = item["rom_start"] + item["size"]
        for item in gaps:
            item["rom_start"] = base + item["pool_offset"]
            item["rom_end"] = item["rom_start"] + item["size"]
            item["sha256"] = hashlib.sha256(
                rom[item["rom_start"] : item["rom_end"]]
            ).hexdigest()
        overlap_count = sum(
            left["rom_end"] > right["rom_start"]
            for left, right in zip(chunks, chunks[1:])
        )
        pool_data = rom[base : base + POOL_LIMITS[name]]
        if len(pool_data) != POOL_LIMITS[name]:
            raise ValueError(f"ROM is truncated inside the {name} pool")
        pools.append(
            {
                "name": name,
                "rom_start": base,
                "rom_end": base + POOL_LIMITS[name],
                "size": POOL_LIMITS[name],
                "sha256": hashlib.sha256(pool_data).hexdigest(),
                "chunk_count": len(chunks),
                "referenced_bytes_with_overlap": sum(item["size"] for item in chunks),
                "referenced_bytes": sum(item["size"] for item in coverage),
                "unreferenced_bytes": sum(item["size"] for item in gaps),
                "overlapping_adjacent_pairs": overlap_count,
                "coverage": coverage,
                "gaps": gaps,
                "chunks": chunks,
            }
        )
    return {
        "format": "BattleTanx Global Assault world pool inventory v1",
        "world_count": len(worlds),
        "pool_reference_count": sum(len(world["references"]) for world in parsed_worlds),
        "pools": pools,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument(
        "--boundaries", type=Path, default=Path("src/code/slot_asset_ranges.c")
    )
    parser.add_argument("--output", type=Path, help="write metadata-only inventory JSON")
    parser.add_argument("--extract-dir", type=Path, help="write referenced raw pool chunks")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    actual_sha1 = hashlib.sha1(rom).hexdigest()
    if actual_sha1 != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {actual_sha1}")
    worlds = inventory_ranges(rom, world_ranges(args.boundaries))
    if len(worlds) != 75:
        raise SystemExit(f"expected 75 decoded worlds, found {len(worlds)}")
    document = {"rom_sha1": actual_sha1, **inventory_pools(rom, worlds)}

    if args.extract_dir:
        for pool in document["pools"]:
            directory = args.extract_dir / pool["name"]
            directory.mkdir(parents=True, exist_ok=True)
            directory.joinpath("pool.bin").write_bytes(
                rom[pool["rom_start"] : pool["rom_end"]]
            )
            for chunk in pool["chunks"]:
                directory.joinpath(chunk_filename(chunk)).write_bytes(
                    rom[chunk["rom_start"] : chunk["rom_end"]]
                )
            gap_directory = directory / "unreferenced"
            gap_directory.mkdir(exist_ok=True)
            for index, gap in enumerate(pool["gaps"]):
                gap_directory.joinpath(gap_filename(index, gap)).write_bytes(
                    rom[gap["rom_start"] : gap["rom_end"]]
                )

    encoded = json.dumps(document, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
