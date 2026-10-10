#!/usr/bin/env python3
"""Decode and optionally split the game's 17 script/cutscene assets."""

from __future__ import annotations

import argparse
import hashlib
import json
from collections import Counter
from pathlib import Path

from inventory_lzari_assets import ROM_SHA1
from inventory_rom_layout import script_ranges


def require(data: bytes, offset: int, size: int, what: str) -> None:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise ValueError(f"truncated {what} at script offset 0x{offset:X}")


def command_size(data: bytes, offset: int, stream_type: int) -> int:
    require(data, offset, 1, "opcode")
    opcode = data[offset]
    if opcode == 0:
        return 0
    if opcode == 1:
        size = 3
    elif opcode in (2, 17, 18, 29, 30, 36, 39, 45, 46):
        size = 1
    elif opcode in (3, 4):
        require(data, offset, 2, f"opcode {opcode}")
        mode = data[offset + 1]
        size = (
            2
            + (mode & 3)
            + ((mode >> 2) & 3)
            + ((mode >> 4) & 3)
            + (2 if mode & 0x40 else 0)
            + (2 if mode & 0x80 else 0)
        )
    elif opcode == 5:
        require(data, offset, 2, "opcode 5")
        mode = data[offset + 1]
        size = 2 + 2 * sum(bool(mode & mask) for mask in (3, 0xC, 0x30))
    elif opcode == 7:
        require(data, offset, 2, "opcode 7")
        mode = data[offset + 1]
        size = 2 + sum(bool(mode & mask) for mask in (1, 4, 0x10))
    elif opcode in (6, 34, 35, 38, 42, 43, 44):
        size = 3
    elif opcode == 19:
        size = 4
    elif opcode in (20, 23, 24):
        size = 7
    elif opcode in (8, 12, 16, 26, 28, 31, 32, 37, 41, 47):
        size = 2
    elif opcode in (9, 10, 11):
        size = 1 if stream_type in (30, 31) else 2
    elif opcode in (13, 14, 15, 27, 48, 49):
        size = 4
    elif opcode == 25:
        size = 5
    elif opcode == 21:
        end = data.find(b"\0", offset + 1)
        if end < 0:
            raise ValueError(f"unterminated opcode-21 string at 0x{offset:X}")
        size = end + 1 - offset
    elif opcode in (22, 33, 40):
        size = 5
    else:
        # The matching decoder's default case consumes the unknown opcode byte.
        size = 1
    require(data, offset, size, f"opcode {opcode}")
    return size


def discover_stream_type(data: bytes, start: int) -> int:
    offset = start
    while True:
        require(data, offset, 1, "stream type scan")
        opcode = data[offset]
        if opcode == 0:
            return 0xFFFF
        if opcode == 8:
            require(data, offset, 2, "stream type command")
            return data[offset + 1]
        offset += command_size(data, offset, 0xFFFF)


def parse_script(data: bytes) -> dict:
    require(data, 0, 4, "script header")
    stream_count = int.from_bytes(data[2:4], "big")
    offset = 4
    streams = []
    opcode_totals = Counter()
    for index in range(stream_count):
        start = offset
        stream_type = discover_stream_type(data, start)
        commands = []
        while True:
            require(data, offset, 1, f"stream {index}")
            opcode = data[offset]
            if opcode == 0:
                size = 1
            else:
                size = command_size(data, offset, stream_type)
            command = {"offset": offset, "opcode": opcode, "size": size}
            if opcode == 21:
                command["text"] = data[offset + 1 : offset + size - 1].decode(
                    "ascii", errors="backslashreplace"
                )
            commands.append(command)
            opcode_totals[opcode] += 1
            offset += size
            if opcode == 0:
                break
        streams.append(
            {
                "index": index,
                "offset": start,
                "size": offset - start,
                "type": stream_type,
                "command_count": len(commands),
                "commands": commands,
            }
        )
    if offset != len(data):
        raise ValueError(f"script parser stopped at 0x{offset:X} of 0x{len(data):X}")
    return {
        "scene_type": data[0],
        "scene_setting": data[1],
        "stream_count": stream_count,
        "command_count": sum(item["command_count"] for item in streams),
        "opcode_counts": {str(key): value for key, value in sorted(opcode_totals.items())},
        "streams": streams,
    }


def inventory_scripts(rom: bytes, campaign: Path) -> dict:
    scripts = []
    opcode_totals = Counter()
    for index, declared in enumerate(script_ranges(campaign)):
        payload = rom[declared["start"] : declared["end"]]
        parsed = parse_script(payload)
        opcode_totals.update({int(key): value for key, value in parsed["opcode_counts"].items()})
        scripts.append(
            {
                "index": index,
                "name": declared["name"],
                "rom_start": declared["start"],
                "rom_end": declared["end"],
                "size": len(payload),
                "sha256": hashlib.sha256(payload).hexdigest(),
                **parsed,
            }
        )
    return {
        "format": "BattleTanx Global Assault script inventory v1",
        "script_count": len(scripts),
        "stream_count": sum(item["stream_count"] for item in scripts),
        "command_count": sum(item["command_count"] for item in scripts),
        "opcode_counts": {str(key): value for key, value in sorted(opcode_totals.items())},
        "scripts": scripts,
    }


def extract_scripts(rom: bytes, report: dict, output: Path) -> None:
    output.mkdir(parents=True, exist_ok=True)
    for script in report["scripts"]:
        directory = output / f"script_{script['index']:02d}_{script['rom_start']:06X}"
        directory.mkdir(parents=True, exist_ok=True)
        payload = rom[script["rom_start"] : script["rom_end"]]
        directory.joinpath("header.bin").write_bytes(payload[:4])
        for stream in script["streams"]:
            directory.joinpath(f"stream_{stream['index']:03d}.bin").write_bytes(
                payload[stream["offset"] : stream["offset"] + stream["size"]]
            )
        directory.joinpath("script.json").write_text(json.dumps(script, indent=2) + "\n")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument(
        "--campaign", type=Path, default=Path("src/code/campaign_mission_config.c")
    )
    parser.add_argument("--output", type=Path, help="write metadata inventory JSON")
    parser.add_argument("--extract-dir", type=Path, help="write split script files")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    actual_sha1 = hashlib.sha1(rom).hexdigest()
    if actual_sha1 != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {actual_sha1}")
    report = {"rom_sha1": actual_sha1, **inventory_scripts(rom, args.campaign)}
    if args.extract_dir:
        extract_scripts(rom, report, args.extract_dir)
    encoded = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
