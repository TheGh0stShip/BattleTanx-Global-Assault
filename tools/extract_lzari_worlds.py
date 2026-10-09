#!/usr/bin/env python3
"""Extract structured manifests for every LZARI-compressed Global Assault world."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

from btga_world import parse_world
from inventory_lzari_assets import ROM_SHA1, inventory_known


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path, help="ignored directory for world manifests")
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

    manifest = []
    for stream in inventory_known(rom, args.boundaries):
        if stream["kind"] != "world":
            continue
        world = parse_world(stream["data"])
        name = stream["name"]
        directory = args.output / name
        directory.mkdir(parents=True, exist_ok=True)
        (directory / "world.json").write_text(json.dumps(world, indent=2) + "\n")
        offsets = world["offsets"]
        for index, (start, end) in enumerate(zip(offsets, offsets[1:])):
            (directory / f"component{index}.bin").write_bytes(stream["data"][start:end])
        manifest.append(
            {
                "name": name,
                "rom_start": stream["rom_start"],
                "rom_end": stream["rom_end"],
                "decoded_sha256": stream["sha256"],
                "group_count": len(world["groups"]),
                "placement_count": len(world["placements"]),
                "definition_count": len(world["definitions"]),
                "model_count": len(world["models"]),
                "part_count": len(world["parts"]),
                "reference_count": len(world["references"]),
            }
        )

    document = {
        "format": "BattleTanx Global Assault structured world inventory v1",
        "rom_sha1": actual_sha1,
        "world_count": len(manifest),
        "worlds": manifest,
    }
    (args.output / "manifest.json").write_text(json.dumps(document, indent=2) + "\n")
    print(f"Extracted {len(manifest)} structured worlds to {args.output}")


if __name__ == "__main__":
    main()
