#!/usr/bin/env python3
"""Verify every known retail resource family without extracting ROM data.

This is the resource equivalent of the matching-code gate.  It verifies the
base ROM, accounts for every byte, exercises each format parser, and requires
every known LZARI stream to survive the canonical encoder byte-for-byte.
Generated reports and extracted assets are intentionally not repository
inputs.
"""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

from btga_world import build_world, parse_world
from inventory_libmus_assets import inventory as inventory_libmus
from inventory_lzari_assets import ROM_SHA1, inventory_known, inventory_ranges
from inventory_rom_layout import inventory_layout
from inventory_script_assets import build_script, inventory_scripts
from inventory_world_display_lists import inventory_display_lists
from inventory_world_geometry import inventory_geometry
from inventory_world_pools import inventory_pools, world_ranges
from inventory_world_textures import inventory_textures


EXPECTED = {
    "rom_size": 0x800000,
    "lzari_streams": 271,
    "lzari_packed_bytes": 1_174_975,
    "lzari_decoded_bytes": 2_886_765,
    "worlds": 75,
    "images": 195,
    "pool_references": 39_103,
    "display_list_chunks": 5_783,
    "texture_usages": 1_351,
    "geometry_chunks": 4_638,
    "scripts": 17,
    "script_streams": 249,
    "script_commands": 144_298,
    "libmus_files": 26,
    "libmus_bytes": 2_289_292,
}


def require(value: object, expected: object, what: str) -> None:
    if value != expected:
        raise ValueError(f"{what}: expected {expected!r}, got {value!r}")


def verify_corpus(rom: bytes, root: Path) -> dict:
    require(hashlib.sha1(rom).hexdigest(), ROM_SHA1, "base ROM SHA-1")
    require(len(rom), EXPECTED["rom_size"], "base ROM size")

    boundaries = root / "src/code/slot_asset_ranges.c"
    campaign = root / "src/code/campaign_mission_config.c"

    streams = inventory_known(rom, boundaries)
    require(len(streams), EXPECTED["lzari_streams"], "LZARI stream count")
    require(
        sum(x["packed_size"] for x in streams),
        EXPECTED["lzari_packed_bytes"],
        "LZARI stored bytes",
    )
    require(
        sum(x["decoded_size"] for x in streams),
        EXPECTED["lzari_decoded_bytes"],
        "LZARI decoded bytes",
    )
    require(sum(x["kind"] == "world" for x in streams), EXPECTED["worlds"], "world count")
    require(sum(x["kind"] == "image" for x in streams), EXPECTED["images"], "image count")
    if not all(x["reencode_exact"] for x in streams):
        failed = [x["name"] for x in streams if not x["reencode_exact"]]
        raise ValueError(f"LZARI streams do not re-encode exactly: {failed}")

    worlds = inventory_ranges(rom, world_ranges(boundaries))
    for world in worlds:
        rebuilt = build_world(parse_world(world["data"]))
        if rebuilt != world["data"]:
            raise ValueError(f"structured world does not rebuild exactly: {world['name']}")
    pools = inventory_pools(rom, worlds)
    require(pools["world_count"], EXPECTED["worlds"], "pool world count")
    require(pools["pool_reference_count"], EXPECTED["pool_references"], "pool reference count")
    for pool in pools["pools"]:
        require(
            pool["referenced_bytes"] + pool["unreferenced_bytes"],
            pool["size"],
            f"{pool['name']} pool coverage",
        )

    display_lists = inventory_display_lists(rom, pools)
    require(
        sum(x["chunk_count"] for x in display_lists["pools"]),
        EXPECTED["display_list_chunks"],
        "display-list chunks",
    )

    textures = inventory_textures(rom, worlds)
    require(textures["usage_count"], EXPECTED["texture_usages"], "texture usages")

    geometry = inventory_geometry(rom, pools)
    require(geometry["chunk_count"], EXPECTED["geometry_chunks"], "geometry chunks")

    scripts = inventory_scripts(rom, campaign)
    require(scripts["script_count"], EXPECTED["scripts"], "script count")
    require(scripts["stream_count"], EXPECTED["script_streams"], "script stream count")
    require(scripts["command_count"], EXPECTED["script_commands"], "script command count")
    for script in scripts["scripts"]:
        rebuilt = build_script(script)
        original = rom[script["rom_start"] : script["rom_end"]]
        if rebuilt != original:
            raise ValueError(f"structured script does not rebuild exactly: {script['name']}")

    audio = inventory_libmus(rom)
    require(audio["file_count"], EXPECTED["libmus_files"], "libmus file count")
    require(audio["stored_bytes"], EXPECTED["libmus_bytes"], "libmus stored bytes")

    layout = inventory_layout(rom, boundaries, campaign)
    require(sum(layout["bytes"].values()), len(rom), "complete ROM accounting")

    return {
        "format": "BattleTanx Global Assault verified asset corpus v1",
        "rom_sha1": ROM_SHA1,
        "rom_bytes": len(rom),
        "rom_regions": layout["region_count"],
        "lzari": {
            "streams": len(streams),
            "stored_bytes": sum(x["packed_size"] for x in streams),
            "decoded_bytes": sum(x["decoded_size"] for x in streams),
            "exact_reencodes": sum(x["reencode_exact"] for x in streams),
        },
        "worlds": {
            "count": pools["world_count"],
            "pool_references": pools["pool_reference_count"],
            "display_list_chunks": sum(x["chunk_count"] for x in display_lists["pools"]),
            "texture_usages": textures["usage_count"],
            "geometry_chunks": geometry["chunk_count"],
        },
        "scripts": {
            "files": scripts["script_count"],
            "streams": scripts["stream_count"],
            "commands": scripts["command_count"],
        },
        "audio": {
            "files": audio["file_count"],
            "stored_bytes": audio["stored_bytes"],
            "sfx_waves": audio["sfx"]["pointer_bank"]["wave_count"],
            "effects": audio["sfx"]["effect_bank"]["effect_count"],
            "music_waves": audio["music"]["pointer_bank"]["wave_count"],
            "songs": audio["music"]["song_count"],
        },
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument("--output", type=Path, help="write the verification summary as JSON")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    report = verify_corpus(args.rom.read_bytes(), root)
    encoded = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
