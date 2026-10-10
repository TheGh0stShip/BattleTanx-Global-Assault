#!/usr/bin/env python3
"""Recover world-texture metadata from paired texture and state display lists."""

from __future__ import annotations

import argparse
import hashlib
import json
from collections import Counter
from pathlib import Path

from btga_world import parse_world
from inventory_lzari_assets import ROM_SHA1, inventory_ranges
from inventory_world_display_lists import find_command_stream
from inventory_world_pools import world_ranges
from n64_texture import decode_texture, encode_png_rgba


TEXTURE_BASE = 0x102C70
STATE_BASE = 0x2F8070
BITS_PER_PIXEL = (4, 8, 16, 32)
FORMAT_NAMES = {0: "RGBA", 2: "CI", 3: "IA", 4: "I"}


def texture_usages(worlds: list[dict]) -> Counter[tuple[int, int, int, int]]:
    usages: Counter[tuple[int, int, int, int]] = Counter()
    for stream in worlds:
        for reference in parse_world(stream["data"])["references"]:
            if reference["texture_offset"] < 0:
                continue
            usages[
                (
                    reference["texture_offset"],
                    reference["texture_size"],
                    reference["state_offset"],
                    reference["state_size"],
                )
            ] += 1
    return usages


def command_words(payload: bytes, kind: str, opcode: int) -> list[tuple[int, int, int]]:
    _start, commands = find_command_stream(payload, kind)
    return [item for item in commands if item[1] >> 24 == opcode]


def inspect_usage(rom: bytes, key: tuple[int, int, int, int], count: int) -> dict:
    texture_offset, texture_size, state_offset, state_size = key
    texture = rom[
        TEXTURE_BASE + texture_offset : TEXTURE_BASE + texture_offset + texture_size
    ]
    state = rom[STATE_BASE + state_offset : STATE_BASE + state_offset + state_size]
    images = command_words(texture, "texture", 0xFD)
    tiles = command_words(state, "state", 0xF5)
    sizes = command_words(state, "state", 0xF2)
    record = {
        "texture_offset": texture_offset,
        "texture_size": texture_size,
        "state_offset": state_offset,
        "state_size": state_size,
        "reference_count": count,
    }
    if not images or not tiles or not sizes:
        return {
            **record,
            "status": "missing texture-state commands",
            "image_command_count": len(images),
            "tile_command_count": len(tiles),
            "tile_size_command_count": len(sizes),
        }

    tile_w0 = tiles[-1][1]
    size_w1 = sizes[-1][2]
    format_id = (tile_w0 >> 21) & 7
    size_id = (tile_w0 >> 19) & 3
    width = ((size_w1 >> 12) & 0xFFF) // 4 + 1
    height = (size_w1 & 0xFFF) // 4 + 1
    image_offset = images[-1][2]
    image_size = (width * height * BITS_PER_PIXEL[size_id] + 7) // 8
    image_end = image_offset + image_size
    if image_end == texture_size:
        status = "exact"
    elif image_end < texture_size:
        status = "trailing bytes"
    else:
        status = "short payload"
    palette_offsets = [item[2] for item in images[:-1]] if format_id == 2 else []
    return {
        **record,
        "status": status,
        "format": FORMAT_NAMES.get(format_id, f"unknown-{format_id}"),
        "format_id": format_id,
        "size_id": size_id,
        "width": width,
        "height": height,
        "image_offset": image_offset,
        "image_size": image_size,
        "available_image_bytes": texture_size - image_offset,
        "palette_offsets": palette_offsets,
        "frame_count": len(palette_offsets) if format_id == 2 else 1,
    }


def inventory_textures(rom: bytes, worlds: list[dict]) -> dict:
    records = [
        inspect_usage(rom, key, count)
        for key, count in sorted(texture_usages(worlds).items())
    ]
    formats = Counter(
        f"{item['format']}{BITS_PER_PIXEL[item['size_id']]}"
        for item in records
        if "format" in item
    )
    statuses = Counter(item["status"] for item in records)
    return {
        "format": "BattleTanx Global Assault world texture inventory v1",
        "usage_count": len(records),
        "reference_count": sum(item["reference_count"] for item in records),
        "preview_count": sum(
            item.get("frame_count", 0) for item in records if item["status"] == "exact"
        ),
        "formats": dict(sorted(formats.items())),
        "statuses": dict(sorted(statuses.items())),
        "textures": records,
    }


def extract_previews(rom: bytes, report: dict, output: Path) -> None:
    output.mkdir(parents=True, exist_ok=True)
    for index, record in enumerate(report["textures"]):
        if record["status"] != "exact":
            continue
        start = TEXTURE_BASE + record["texture_offset"]
        image = rom[
            start + record["image_offset"] : start + record["image_offset"] + record["image_size"]
        ]
        palettes = record["palette_offsets"] or [None]
        for frame, palette_offset in enumerate(palettes):
            payload = image
            if palette_offset is not None:
                payload = rom[start + palette_offset : start + palette_offset + 32] + image
            pixels = decode_texture(
                payload,
                record["format_id"],
                record["size_id"],
                record["width"],
                record["height"],
            )
            filename = f"texture_{index:04d}_frame_{frame:02d}.png"
            (output / filename).write_bytes(
                encode_png_rgba(record["width"], record["height"], pixels)
            )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument(
        "--boundaries", type=Path, default=Path("src/code/slot_asset_ranges.c")
    )
    parser.add_argument("--output", type=Path, help="write metadata inventory JSON")
    parser.add_argument("--preview-dir", type=Path, help="write decoded PNG previews")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    actual_sha1 = hashlib.sha1(rom).hexdigest()
    if actual_sha1 != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {actual_sha1}")
    worlds = inventory_ranges(rom, world_ranges(args.boundaries))
    report = {"rom_sha1": actual_sha1, **inventory_textures(rom, worlds)}
    if args.preview_dir:
        extract_previews(rom, report, args.preview_dir)
    encoded = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
