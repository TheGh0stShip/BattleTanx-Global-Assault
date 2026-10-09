#!/usr/bin/env python3
"""Extract the game's LZARI-compressed N64 images as PNG previews."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

from inventory_lzari_assets import ROM_SHA1, inventory_known
from n64_texture import decode_texture, encode_png_rgba


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path, help="ignored directory for PNG previews and manifest")
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument(
        "--boundaries", type=Path, default=Path("src/code/slot_asset_ranges.c")
    )
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    actual_sha1 = hashlib.sha1(rom).hexdigest()
    if actual_sha1 != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {actual_sha1}")

    args.output.mkdir(parents=True, exist_ok=True)
    records = []
    for stream in inventory_known(rom, args.boundaries):
        if stream["kind"] != "image":
            continue
        pixels = decode_texture(
            stream["data"],
            stream["format"],
            stream["flags"],
            stream["width"],
            stream["height"],
        )
        filename = f"image_{stream['table_index']:03d}.png"
        (args.output / filename).write_bytes(
            encode_png_rgba(stream["width"], stream["height"], pixels)
        )
        records.append(
            {
                "table_index": stream["table_index"],
                "file": filename,
                "rom_start": stream["rom_start"],
                "rom_end": stream["rom_end"],
                "format": stream["format"],
                "size": stream["flags"],
                "width": stream["width"],
                "height": stream["height"],
                "decoded_sha256": stream["sha256"],
            }
        )

    manifest = {
        "format": "BattleTanx Global Assault image preview manifest v1",
        "rom_sha1": actual_sha1,
        "image_count": len(records),
        "images": records,
    }
    (args.output / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"Extracted {len(records)} PNG previews to {args.output}")


if __name__ == "__main__":
    main()
