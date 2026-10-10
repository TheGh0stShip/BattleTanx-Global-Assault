#!/usr/bin/env python3
"""Inventory F3DEX2 command streams inside the three raw world pools."""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
from collections import Counter
from pathlib import Path

from inventory_lzari_assets import ROM_SHA1, inventory_ranges
from inventory_world_pools import inventory_pools, world_ranges


OPCODES = {
    0x01: "G_VTX",
    0x05: "G_TRI1",
    0xD7: "G_TEXTURE",
    0xD9: "G_GEOMETRYMODE",
    0xDF: "G_ENDDL",
    0xE2: "G_SETOTHERMODE_L",
    0xE3: "G_SETOTHERMODE_H",
    0xE6: "G_RDPLOADSYNC",
    0xE7: "G_RDPPIPESYNC",
    0xE8: "G_RDPTILESYNC",
    0xF0: "G_LOADTLUT",
    0xF2: "G_SETTILESIZE",
    0xF3: "G_LOADBLOCK",
    0xF5: "G_SETTILE",
    0xFA: "G_SETPRIMCOLOR",
    0xFB: "G_SETENVCOLOR",
    0xFC: "G_SETCOMBINE",
    0xFD: "G_SETTIMG",
}

ALLOWED = {
    "texture": {0xE3, 0xE6, 0xE7, 0xE8, 0xF0, 0xF3, 0xF5, 0xFD, 0xDF},
    "state": {0xD7, 0xD9, 0xDF, 0xE2, 0xE3, 0xE7, 0xF2, 0xF5, 0xFA, 0xFB, 0xFC},
    "geometry": {0x01, 0x05, 0xDF},
}


def find_command_stream(payload: bytes, kind: str) -> tuple[int, list[tuple[int, int, int]]]:
    """Find the longest aligned, pool-appropriate stream ending in G_ENDDL."""
    candidates = []
    for start in range(0, len(payload) - 7, 8):
        commands = []
        for offset in range(start, len(payload) - 7, 8):
            w0, w1 = struct.unpack_from(">II", payload, offset)
            opcode = w0 >> 24
            if opcode not in ALLOWED[kind]:
                break
            commands.append((offset, w0, w1))
            if opcode == 0xDF:
                if w1 == 0 and len(commands) > 1:
                    candidates.append(commands)
                break
    if not candidates:
        raise ValueError(f"no {kind} F3DEX2 command stream found")
    commands = max(candidates, key=lambda item: (len(item), -item[0][0]))
    return commands[0][0], commands


def inspect_chunk(rom: bytes, kind: str, chunk: dict) -> dict:
    payload = rom[chunk["rom_start"] : chunk["rom_end"]]
    start, commands = find_command_stream(payload, kind)
    end = commands[-1][0] + 8
    opcodes = Counter(OPCODES[w0 >> 24] for _offset, w0, _w1 in commands)
    relocations = []
    relocation_opcode = 0xFD if kind == "texture" else 0x01 if kind == "geometry" else None
    for offset, w0, w1 in commands:
        if w0 >> 24 != relocation_opcode:
            continue
        if w1 >= len(payload):
            raise ValueError(
                f"{kind} chunk {chunk['index']} relocation target exceeds its chunk"
            )
        relocation = {
            "command_offset": offset,
            "target_offset": w1,
            "opcode": OPCODES[w0 >> 24],
        }
        if kind == "geometry":
            vertex_count = (w0 >> 12) & 0xFF
            if w1 + vertex_count * 16 > len(payload):
                raise ValueError(
                    f"geometry chunk {chunk['index']} vertex payload exceeds its chunk"
                )
            relocation["vertex_count"] = vertex_count
        relocations.append(relocation)
    return {
        "index": chunk["index"],
        "pool_offset": chunk["pool_offset"],
        "size": chunk["size"],
        "reference_count": chunk["reference_count"],
        "command_offset": start,
        "command_count": len(commands),
        "command_bytes": end - start,
        "leading_payload_bytes": start,
        "trailing_payload_bytes": len(payload) - end,
        "opcodes": dict(sorted(opcodes.items())),
        "relocations": relocations,
    }


def inventory_display_lists(rom: bytes, pool_report: dict) -> dict:
    pools = []
    for pool in pool_report["pools"]:
        chunks = [inspect_chunk(rom, pool["name"], chunk) for chunk in pool["chunks"]]
        opcode_totals = Counter()
        for chunk in chunks:
            opcode_totals.update(chunk["opcodes"])
        pools.append(
            {
                "name": pool["name"],
                "chunk_count": len(chunks),
                "command_bytes": sum(item["command_bytes"] for item in chunks),
                "leading_payload_bytes": sum(
                    item["leading_payload_bytes"] for item in chunks
                ),
                "trailing_payload_bytes": sum(
                    item["trailing_payload_bytes"] for item in chunks
                ),
                "opcode_totals": dict(sorted(opcode_totals.items())),
                "nonzero_command_offsets": sum(
                    item["command_offset"] != 0 for item in chunks
                ),
                "chunks": chunks,
            }
        )
    return {
        "format": "BattleTanx Global Assault world F3DEX2 inventory v1",
        "pools": pools,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument(
        "--boundaries", type=Path, default=Path("src/code/slot_asset_ranges.c")
    )
    parser.add_argument("--output", type=Path, help="write metadata inventory JSON")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    actual_sha1 = hashlib.sha1(rom).hexdigest()
    if actual_sha1 != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {actual_sha1}")
    worlds = inventory_ranges(rom, world_ranges(args.boundaries))
    pools = inventory_pools(rom, worlds)
    report = {"rom_sha1": actual_sha1, **inventory_display_lists(rom, pools)}
    encoded = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
