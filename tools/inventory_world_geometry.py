#!/usr/bin/env python3
"""Decode and validate F3DEX2 meshes in the raw world geometry pool."""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
from collections import Counter
from pathlib import Path

from inventory_lzari_assets import ROM_SHA1, inventory_ranges
from inventory_world_display_lists import find_command_stream
from inventory_world_pools import inventory_pools, world_ranges


GEOMETRY_BASE = 0x3013F0
VERTEX = struct.Struct(">hhhHhhbbbB")


def inspect_geometry_chunk(rom: bytes, chunk: dict) -> dict:
    payload = rom[chunk["rom_start"] : chunk["rom_end"]]
    command_start, commands = find_command_stream(payload, "geometry")
    command_end = commands[-1][0] + 8
    cache: dict[int, int] = {}
    vertex_offsets: set[int] = set()
    triangles = []
    load_count = 0
    overlaps_commands = False

    for _offset, w0, w1 in commands:
        opcode = w0 >> 24
        if opcode == 0x01:
            count = (w0 >> 12) & 0xFF
            end_slot = (w0 >> 1) & 0x7F
            first_slot = end_slot - count
            if first_slot < 0 or end_slot > 32:
                raise ValueError(f"geometry chunk {chunk['index']} has invalid VTX slots")
            if w1 + count * VERTEX.size > len(payload):
                raise ValueError(f"geometry chunk {chunk['index']} has truncated vertices")
            if max(w1, command_start) < min(w1 + count * VERTEX.size, command_end):
                overlaps_commands = True
            for index in range(count):
                vertex_offset = w1 + index * VERTEX.size
                cache[first_slot + index] = vertex_offset
                vertex_offsets.add(vertex_offset)
            load_count += 1
        elif opcode == 0x05:
            encoded = [(w0 >> shift) & 0xFF for shift in (16, 8, 0)]
            if any(value & 1 for value in encoded):
                raise ValueError(f"geometry chunk {chunk['index']} has odd TRI1 index")
            slots = [value // 2 for value in encoded]
            if any(slot not in cache for slot in slots):
                raise ValueError(f"geometry chunk {chunk['index']} uses an unloaded vertex")
            triangle = [cache[slot] for slot in slots]
            if len(set(triangle)) != 3:
                raise ValueError(f"geometry chunk {chunk['index']} has a degenerate triangle")
            triangles.append(triangle)

    vertices = []
    for offset in sorted(vertex_offsets):
        x, y, z, flag, s, t, nx, ny, nz, alpha = VERTEX.unpack_from(payload, offset)
        vertices.append(
            {
                "offset": offset,
                "position": [x, y, z],
                "flag": flag,
                "st": [s, t],
                "normal": [nx, ny, nz],
                "alpha": alpha,
            }
        )
    nonzero_flags = sum(item["flag"] != 0 for item in vertices)
    status = "vertex-command overlap" if overlaps_commands else "exact"
    if nonzero_flags and not overlaps_commands:
        status = "nonzero vertex flags"
    positions = [component for item in vertices for component in item["position"]]
    bounds = {
        "min": [min(item["position"][axis] for item in vertices) for axis in range(3)],
        "max": [max(item["position"][axis] for item in vertices) for axis in range(3)],
    }
    if len(positions) != len(vertices) * 3:
        raise AssertionError("vertex position accounting failed")
    return {
        "index": chunk["index"],
        "pool_offset": chunk["pool_offset"],
        "size": chunk["size"],
        "reference_count": chunk["reference_count"],
        "status": status,
        "command_offset": command_start,
        "vertex_load_count": load_count,
        "vertex_count": len(vertices),
        "triangle_count": len(triangles),
        "nonzero_vertex_flags": nonzero_flags,
        "bounds": bounds,
        "vertices": vertices,
        "triangles": triangles,
    }


def inventory_geometry(rom: bytes, pool_report: dict) -> dict:
    pool = next(item for item in pool_report["pools"] if item["name"] == "geometry")
    chunks = [inspect_geometry_chunk(rom, chunk) for chunk in pool["chunks"]]
    statuses = Counter(item["status"] for item in chunks)
    return {
        "format": "BattleTanx Global Assault world geometry inventory v1",
        "chunk_count": len(chunks),
        "vertex_load_count": sum(item["vertex_load_count"] for item in chunks),
        "vertex_count": sum(item["vertex_count"] for item in chunks),
        "triangle_count": sum(item["triangle_count"] for item in chunks),
        "statuses": dict(sorted(statuses.items())),
        "chunks": chunks,
    }


def write_obj(report: dict, output: Path) -> None:
    lines = ["# BattleTanx: Global Assault world geometry inspection export"]
    vertex_base = 1
    for chunk in report["chunks"]:
        if chunk["status"] != "exact":
            continue
        lines.append(f"g geometry_{chunk['index']:04d}_{chunk['pool_offset']:06X}")
        by_offset = {}
        for index, vertex in enumerate(chunk["vertices"]):
            by_offset[vertex["offset"]] = vertex_base + index
            x, y, z = vertex["position"]
            s, t = vertex["st"]
            nx, ny, nz = vertex["normal"]
            lines.append(f"v {x} {y} {z}")
            lines.append(f"vt {s / 32.0:.6f} {-t / 32.0:.6f}")
            lines.append(f"vn {nx / 127.0:.6f} {ny / 127.0:.6f} {nz / 127.0:.6f}")
        for triangle in chunk["triangles"]:
            indices = [by_offset[offset] for offset in triangle]
            lines.append("f " + " ".join(f"{i}/{i}/{i}" for i in indices))
        vertex_base += len(chunk["vertices"])
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text("\n".join(lines) + "\n")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument(
        "--boundaries", type=Path, default=Path("src/code/slot_asset_ranges.c")
    )
    parser.add_argument("--output", type=Path, help="write metadata inventory JSON")
    parser.add_argument("--obj", type=Path, help="write a combined inspection OBJ")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    actual_sha1 = hashlib.sha1(rom).hexdigest()
    if actual_sha1 != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {actual_sha1}")
    worlds = inventory_ranges(rom, world_ranges(args.boundaries))
    report = {
        "rom_sha1": actual_sha1,
        **inventory_geometry(rom, inventory_pools(rom, worlds)),
    }
    if args.obj:
        write_obj(report, args.obj)
    encoded = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
