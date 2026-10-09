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
IMAGE_TABLE_ROM = 0xA6860
IMAGE_RECORD_COUNT = 196
IMAGE_RECORD_SIZE = 28


def load_boundaries(path: Path) -> list[int]:
    return sorted({int(match, 16) for match in SYMBOL.findall(path.read_text())})


def level_world_ranges(path: Path) -> list[dict]:
    boundaries = load_boundaries(path)
    return [
        {"kind": "world", "name": f"level_world_{index:03d}", "start": start, "end": end}
        for index, (start, end) in enumerate(zip(boundaries, boundaries[1:]))
    ]


def image_ranges(rom: bytes) -> list[dict]:
    ranges = []
    record = struct.Struct(">BBHHHHHIIII")
    for index in range(IMAGE_RECORD_COUNT):
        offset = IMAGE_TABLE_ROM + index * IMAGE_RECORD_SIZE
        fields = record.unpack_from(rom, offset)
        start = fields[-2] & 0x0FFFFFFF
        inclusive_end = fields[-1] & 0x0FFFFFFF
        if start == 0:
            continue
        # func_8007BCF0 rounds the inclusive range down to an even DMA size.
        stored_size = (inclusive_end - start + 1) & ~1
        ranges.append(
            {
                "kind": "image",
                "name": f"image_{index:03d}",
                "table_index": index,
                "start": start,
                "end": start + stored_size,
                "format": fields[0],
                "flags": fields[1],
                "width": fields[2],
                "height": fields[3],
            }
        )
    return ranges


def known_ranges(rom: bytes, boundary_path: Path) -> list[dict]:
    ranges = [
        {
            "kind": "leftover",
            "name": "btx1_world_leftover",
            "start": 0x100000,
            "end": 0x102068,
        },
        {
            "kind": "world",
            "name": "common_world",
            "start": 0x3F6EE8,
            "end": 0x3F9B5C,
        },
    ]
    ranges.extend(level_world_ranges(boundary_path))
    ranges.extend(image_ranges(rom))
    return ranges


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
    if sizes[2] % 12 or sizes[4] % 16 or sizes[5] % 4 or sizes[6] % 24:
        return None
    placement_count = sizes[2] // 12
    model_count = sizes[4] // 16
    part_count = sizes[5] // 4
    pool_ref_count = sizes[6] // 24
    for index in range(table_count):
        count, first = struct.unpack_from(">HH", data, offsets[1] + index * 16)
        if first + count > placement_count:
            return None
    for index in range(model_count):
        count, zero, first = struct.unpack_from(">BBH", data, offsets[4] + index * 16)
        if zero != 0 or first + count > part_count:
            return None
    for index in range(part_count):
        count, zero, first = struct.unpack_from(">BBH", data, offsets[5] + index * 4)
        if zero != 0 or first + count > pool_ref_count:
            return None
    return {
        "offsets": list(offsets),
        "component_sizes": sizes,
        "group_count": table_count,
        "placement_count": placement_count,
        "object_definition_bytes": sizes[3],
        "model_count": model_count,
        "part_count": part_count,
        "pool_ref_count": pool_ref_count,
    }


def format_hint(data: bytes) -> str:
    if bundle_layout(data) is not None:
        return "btga_7_component_bundle"
    offsets = bundle_offsets(data)
    if offsets is not None:
        return f"be_{len(offsets)}_offset_bundle"
    return "unknown"


def inventory_ranges(rom: bytes, ranges: list[dict]) -> list[dict]:
    streams = []
    for declared in ranges:
        start, end = declared["start"], declared["end"]
        packed = rom[start:end]
        try:
            result = decompress_with_info(packed)
        except LzariError:
            continue
        # Image DMA records can append one zero alignment byte, leaving the
        # arithmetic reader exactly byte-aligned (0 bits) instead of with its
        # usual 8- or 16-bit coder tail.
        valid_padding = (0, 8, 16) if declared["kind"] == "image" else (8, 16)
        if result.consumed_bytes != len(packed) or result.padding_bits not in valid_padding:
            continue
        layout = bundle_layout(result.data)
        canonical = compress(result.data)
        storage_padding = packed[len(canonical) :]
        record = {
            **{
                key: value
                for key, value in declared.items()
                if key not in ("start", "end")
            },
            "index": len(streams),
            "rom_start": start,
            "rom_end": end,
            "packed_size": len(packed),
            "decoded_size": len(result.data),
            "padding_bits": result.padding_bits,
            "format_hint": format_hint(result.data),
            "sha256": hashlib.sha256(result.data).hexdigest(),
            "codec_size": len(canonical),
            "storage_padding_bytes": len(storage_padding),
            "reencode_exact": canonical + storage_padding == packed
            and not storage_padding.strip(b"\0"),
            "data": result.data,
        }
        if layout is not None:
            record.update(layout)
        streams.append(record)
    return streams


def inventory(rom: bytes, boundary_path: Path) -> list[dict]:
    """Inventory the original 74 level ranges (compatibility API)."""
    return inventory_ranges(rom, level_world_ranges(boundary_path))


def inventory_known(rom: bytes, boundary_path: Path) -> list[dict]:
    return inventory_ranges(rom, known_ranges(rom, boundary_path))


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
    streams = inventory_known(rom, args.boundaries)

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
        "format": "BattleTanx Global Assault LZARI asset inventory v2",
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
