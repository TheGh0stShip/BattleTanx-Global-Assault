#!/usr/bin/env python3
"""Trim assembler-owned tail padding from a generated ELF32 section."""

from __future__ import annotations

import argparse
import struct
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("elf", type=Path)
    parser.add_argument("section")
    parser.add_argument("size", type=lambda value: int(value, 0))
    parser.add_argument("--alignment", type=lambda value: int(value, 0))
    args = parser.parse_args()

    data = bytearray(args.elf.read_bytes())
    if data[:4] != b"\x7fELF" or data[4] != 1:
        raise SystemExit("expected an ELF32 object")
    endian = ">" if data[5] == 2 else "<"
    section_offset = struct.unpack_from(endian + "I", data, 0x20)[0]
    section_size, section_count, names_index = struct.unpack_from(
        endian + "HHH", data, 0x2E
    )

    def header(index: int) -> int:
        return section_offset + index * section_size

    names_header = header(names_index)
    names_offset = struct.unpack_from(endian + "I", data, names_header + 0x10)[0]

    for index in range(section_count):
        current = header(index)
        name_offset = struct.unpack_from(endian + "I", data, current)[0]
        start = names_offset + name_offset
        end = data.index(0, start)
        if data[start:end].decode() != args.section:
            continue
        old_size = struct.unpack_from(endian + "I", data, current + 0x14)[0]
        if args.size > old_size:
            raise SystemExit("new section size exceeds the existing size")
        struct.pack_into(endian + "I", data, current + 0x14, args.size)
        if args.alignment is not None:
            struct.pack_into(endian + "I", data, current + 0x20, args.alignment)
        args.elf.write_bytes(data)
        return

    raise SystemExit(f"section not found: {args.section}")


if __name__ == "__main__":
    main()
