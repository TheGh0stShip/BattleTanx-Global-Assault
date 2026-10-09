#!/usr/bin/env python3
"""Inventory and optionally extract ROM-resident LZARI asset streams.

Stream boundaries come from the matching slot-range translation unit, not from
signature scanning. Extracted bytes are written only when --extract-dir is
provided; the repository's assets/extracted tree is ignored by Git.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from pathlib import Path

from lzari import LzariError, compress, decompress_with_info


ROM_SHA1 = "805248fb0a0ee694cad8d7dc927b631d860dd8cf"
SYMBOL = re.compile(r"\bD_B0([0-9A-Fa-f]{6})\b")


def load_boundaries(path: Path) -> list[int]:
    return sorted({int(match, 16) for match in SYMBOL.findall(path.read_text())})


def bundle_offsets(data: bytes) -> tuple[int, ...] | None:
    if len(data) >= 8:
        first = struct.unpack_from(">I", data)[0]
        if 8 <= first <= len(data) and first % 4 == 0:
            offsets = struct.unpack_from(f">{first // 4}I", data)
            if (
                offsets[0] == first
                and all(
                    left <= right <= len(data)
                    for left, right in zip(offsets, offsets[1:])
                )
                and offsets[-1] == len(data)
            ):
                return offsets
    return None


def bundle_layout(data: bytes) -> dict | None:
    offsets = bundle_offsets(data)
    if offsets is None or len(offsets) != 8:
        return None
    sizes = [right - left for left, right in zip(offsets, offsets[1:])]
    if sizes[0] != 4:
        return None
    table_count = struct.unpack_from(">I", data, offsets[0])[0]
    if sizes[1] != table_count * 16:
        return None
    if sizes[2] % 12 or sizes[4] % 4 or sizes[5] % 4 or sizes[6] % 24:
        return None
    return {
        "offsets": list(offsets),
        "component_sizes": sizes,
        "component_counts": [
            1,
            table_count,
            sizes[2] // 12,
            sizes[3] // 16,
            sizes[4] // 4,
            None,
            sizes[6] // 24,
        ],
        "component_3_tail_bytes": sizes[3] % 16,
    }


def format_hint(data: bytes) -> str:
    if bundle_layout(data) is not None:
        return "btga_7_component_bundle"
    offsets = bundle_offsets(data)
    if offsets is not None:
        return f"be_{len(offsets)}_offset_bundle"
    return "unknown"


def inventory(rom: bytes, boundary_path: Path) -> list[dict]:
    boundaries = load_boundaries(boundary_path)
    streams = []
    for start, end in zip(boundaries, boundaries[1:]):
        packed = rom[start:end]
        try:
            result = decompress_with_info(packed)
        except LzariError:
            continue
        if result.consumed_bytes != len(packed) or result.padding_bits not in (8, 16):
            continue
        layout = bundle_layout(result.data)
        record = {
            "index": len(streams),
            "rom_start": start,
            "rom_end": end,
            "packed_size": len(packed),
            "decoded_size": len(result.data),
            "padding_bits": result.padding_bits,
            "format_hint": format_hint(result.data),
            "sha256": hashlib.sha256(result.data).hexdigest(),
            "reencode_exact": compress(result.data) == packed,
            "data": result.data,
        }
        if layout is not None:
            record.update(layout)
        streams.append(record)
    return streams


def public_record(stream: dict) -> dict:
    return {key: value for key, value in stream.items() if key != "data"}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument(
        "--boundaries", type=Path, default=Path("src/code/slot_asset_ranges.c")
    )
    parser.add_argument("--output", type=Path, help="write inventory JSON")
    parser.add_argument("--extract-dir", type=Path, help="write decoded .bin files")
    parser.add_argument(
        "--split-bundles",
        action="store_true",
        help="with --extract-dir, also write each offset-table component",
    )
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    actual_sha1 = hashlib.sha1(rom).hexdigest()
    if actual_sha1 != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {actual_sha1}")
    streams = inventory(rom, args.boundaries)

    if args.extract_dir:
        args.extract_dir.mkdir(parents=True, exist_ok=True)
        for stream in streams:
            name = f"{stream['index']:03d}_{stream['rom_start']:06X}_{stream['rom_end']:06X}.bin"
            (args.extract_dir / name).write_bytes(stream["data"])
            if args.split_bundles and "offsets" in stream:
                stem = name.removesuffix(".bin")
                offsets = stream["offsets"]
                for index, (left, right) in enumerate(zip(offsets, offsets[1:])):
                    (args.extract_dir / f"{stem}.part{index}.bin").write_bytes(
                        stream["data"][left:right]
                    )

    document = {
        "format": "BattleTanx Global Assault LZARI asset inventory v1",
        "rom_sha1": actual_sha1,
        "stream_count": len(streams),
        "packed_bytes": sum(item["packed_size"] for item in streams),
        "decoded_bytes": sum(item["decoded_size"] for item in streams),
        "streams": [public_record(item) for item in streams],
    }
    encoded = json.dumps(document, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
