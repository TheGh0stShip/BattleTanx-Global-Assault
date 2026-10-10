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


BYTE_VALUE_OPCODES = {8, 12, 16, 26, 28, 31, 32, 37, 41, 47}
RGB_OPCODES = {13, 14, 15, 48, 49}
PAIR_OPCODES = {22, 33}
U16_OPCODES = {6, 34, 35, 38, 42, 43, 44}
NO_PARAMETER_OPCODES = {0, 2, 17, 18, 29, 30, 36, 39, 45, 46}


def _signed24(data: bytes, offset: int) -> int:
    value = int.from_bytes(data[offset : offset + 3], "big")
    return value - 0x1000000 if value & 0x800000 else value


def _pack_signed24(value: int) -> bytes:
    if not -0x800000 <= value <= 0x7FFFFF:
        raise ValueError(f"signed 24-bit value out of range: {value}")
    return (value & 0xFFFFFF).to_bytes(3, "big")


def _signed_width(data: bytes, offset: int, width: int) -> int:
    return int.from_bytes(data[offset : offset + width], "big", signed=True)


def _pack_signed_width(value: int, width: int) -> bytes:
    minimum = -(1 << (width * 8 - 1))
    maximum = (1 << (width * 8 - 1)) - 1
    if not minimum <= value <= maximum:
        raise ValueError(f"signed {width * 8}-bit value out of range: {value}")
    return int(value).to_bytes(width, "big", signed=True)


def decode_command_fields(opcode: int, payload: bytes) -> dict:
    if opcode in (3, 4):
        mode = payload[1]
        fields = {"mode": mode}
        offset = 2
        for name, shift in (("x", 0), ("y", 2), ("z", 4)):
            width = (mode >> shift) & 3
            if width:
                fields[name] = _signed_width(payload, offset, width)
                offset += width
        if mode & 0x40:
            fields["parameter_40"] = int.from_bytes(payload[offset : offset + 2], "big")
            offset += 2
        if mode & 0x80:
            fields["parameter_80"] = int.from_bytes(payload[offset : offset + 2], "big")
        return fields
    if opcode == 5:
        mode = payload[1]
        fields = {"mode": mode}
        offset = 2
        for name, mask in (("x", 3), ("y", 0xC), ("z", 0x30)):
            if mode & mask:
                fields[name] = int.from_bytes(payload[offset : offset + 2], "big", signed=True)
                offset += 2
        return fields
    if opcode == 7:
        mode = payload[1]
        fields = {"mode": mode}
        offset = 2
        for name, mask in (("x", 1), ("y", 4), ("z", 0x10)):
            if mode & mask:
                fields[name] = payload[offset]
                offset += 1
        return fields
    if opcode == 1:
        return {"ticks": int.from_bytes(payload[1:3], "big")}
    if opcode in BYTE_VALUE_OPCODES or (opcode in (9, 10, 11) and len(payload) == 2):
        return {"value": payload[1]}
    if opcode in RGB_OPCODES:
        return {"red": payload[1], "green": payload[2], "blue": payload[3]}
    if opcode == 25:
        return {
            "red": payload[1],
            "green": payload[2],
            "blue": payload[3],
            "alpha": payload[4],
        }
    if opcode in U16_OPCODES:
        return {"value": int.from_bytes(payload[1:3], "big")}
    if opcode == 19:
        return {"value": _signed24(payload, 1)}
    if opcode == 20:
        return {"x": _signed24(payload, 1), "y": _signed24(payload, 4)}
    if opcode in PAIR_OPCODES:
        return {
            "first": int.from_bytes(payload[1:3], "big"),
            "second": int.from_bytes(payload[3:5], "big"),
        }
    if opcode == 23:
        return {
            "first": int.from_bytes(payload[1:3], "big"),
            "second": int.from_bytes(payload[3:5], "big"),
            "value": int.from_bytes(payload[5:7], "big"),
        }
    if opcode == 24:
        return {
            "red": payload[1],
            "green": payload[2],
            "blue": payload[3],
            "alpha": payload[4],
            "value": int.from_bytes(payload[5:7], "big"),
        }
    if opcode == 27:
        return {"alpha": payload[1], "value": int.from_bytes(payload[2:4], "big")}
    if opcode == 40:
        return {
            "x": int.from_bytes(payload[1:3], "big", signed=True),
            "y": int.from_bytes(payload[3:5], "big", signed=True),
        }
    return {}


def encode_command_fields(command: dict, payload: bytes) -> bytes:
    fields = command.get("fields", {})
    if not fields:
        return payload
    opcode = command["opcode"]
    result = bytearray(payload)

    def put_u16(offset: int, value: int) -> None:
        result[offset : offset + 2] = int(value).to_bytes(2, "big")

    if opcode in (3, 4):
        mode = fields["mode"]
        result = bytearray((opcode, mode))
        for name, shift in (("x", 0), ("y", 2), ("z", 4)):
            width = (mode >> shift) & 3
            if width:
                result.extend(_pack_signed_width(fields[name], width))
        if mode & 0x40:
            result.extend(int(fields["parameter_40"]).to_bytes(2, "big"))
        if mode & 0x80:
            result.extend(int(fields["parameter_80"]).to_bytes(2, "big"))
    elif opcode == 5:
        mode = fields["mode"]
        result = bytearray((opcode, mode))
        for name, mask in (("x", 3), ("y", 0xC), ("z", 0x30)):
            if mode & mask:
                result.extend(int(fields[name]).to_bytes(2, "big", signed=True))
    elif opcode == 7:
        mode = fields["mode"]
        result = bytearray((opcode, mode))
        for name, mask in (("x", 1), ("y", 4), ("z", 0x10)):
            if mode & mask:
                result.append(fields[name])
    elif opcode == 1:
        put_u16(1, fields["ticks"])
    elif opcode in BYTE_VALUE_OPCODES or (opcode in (9, 10, 11) and len(payload) == 2):
        result[1] = fields["value"]
    elif opcode in RGB_OPCODES:
        result[1:4] = bytes((fields["red"], fields["green"], fields["blue"]))
    elif opcode == 25:
        result[1:5] = bytes(
            (fields["red"], fields["green"], fields["blue"], fields["alpha"])
        )
    elif opcode in U16_OPCODES:
        put_u16(1, fields["value"])
    elif opcode == 19:
        result[1:4] = _pack_signed24(fields["value"])
    elif opcode == 20:
        result[1:4] = _pack_signed24(fields["x"])
        result[4:7] = _pack_signed24(fields["y"])
    elif opcode in PAIR_OPCODES:
        put_u16(1, fields["first"])
        put_u16(3, fields["second"])
    elif opcode == 23:
        put_u16(1, fields["first"])
        put_u16(3, fields["second"])
        put_u16(5, fields["value"])
    elif opcode == 24:
        result[1:5] = bytes(
            (fields["red"], fields["green"], fields["blue"], fields["alpha"])
        )
        put_u16(5, fields["value"])
    elif opcode == 27:
        result[1] = fields["alpha"]
        put_u16(2, fields["value"])
    elif opcode == 40:
        result[1:3] = int(fields["x"]).to_bytes(2, "big", signed=True)
        result[3:5] = int(fields["y"]).to_bytes(2, "big", signed=True)
    else:
        raise ValueError(f"opcode {opcode} has no editable fixed-field schema")
    return bytes(result)


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
            command["raw_hex"] = data[offset : offset + size].hex()
            fields = decode_command_fields(opcode, data[offset : offset + size])
            if fields:
                command["fields"] = fields
            elif opcode in NO_PARAMETER_OPCODES:
                command["fields"] = {}
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


def build_script(script: dict) -> bytes:
    """Rebuild one script from the lossless JSON returned by ``parse_script``."""

    streams = script["streams"]
    if len(streams) != script["stream_count"]:
        raise ValueError("script stream count disagrees with stream list")
    rebuilt = bytearray(
        (script["scene_type"], script["scene_setting"])
    ) + len(streams).to_bytes(2, "big")
    for expected_index, stream in enumerate(streams):
        if stream["index"] != expected_index:
            raise ValueError("script stream indices are not contiguous")
        for command in stream["commands"]:
            payload = bytes.fromhex(command["raw_hex"])
            if command["opcode"] == 21 and "text" in command:
                payload = bytes((21,)) + command["text"].encode("ascii") + b"\0"
            else:
                payload = encode_command_fields(command, payload)
            if not payload or payload[0] != command["opcode"]:
                raise ValueError("script command payload disagrees with opcode")
            rebuilt.extend(payload)
        if not stream["commands"] or stream["commands"][-1]["opcode"] != 0:
            raise ValueError(f"script stream {expected_index} has no terminator")
    # Reparse the result to enforce the loader-derived command sizes and exact
    # stream count, including any edited variable-length text command.
    parse_script(bytes(rebuilt))
    return bytes(rebuilt)


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
