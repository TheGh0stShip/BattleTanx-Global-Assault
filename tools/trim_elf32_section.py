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

    target_index = None
    for index in range(section_count):
        current = header(index)
        name_offset = struct.unpack_from(endian + "I", data, current)[0]
        start = names_offset + name_offset
        end = data.index(0, start)
        if data[start:end].decode() != args.section:
            continue
        target_index = index
        old_size = struct.unpack_from(endian + "I", data, current + 0x14)[0]
        if args.size > old_size:
            raise SystemExit("new section size exceeds the existing size")
        struct.pack_into(endian + "I", data, current + 0x14, args.size)
        if args.alignment is not None:
            struct.pack_into(endian + "I", data, current + 0x20, args.alignment)
        break

    if target_index is None:
        raise SystemExit(f"section not found: {args.section}")

    # KMC includes trailing alignment in function sizes. Once that padding is
    # removed, keep symbols inside the shortened section or modern ld may walk
    # beyond its bounds (binutils 2.42 can segfault here).
    for index in range(section_count):
        current = header(index)
        section_type = struct.unpack_from(endian + "I", data, current + 0x04)[0]
        if section_type != 2:  # SHT_SYMTAB
            continue
        symbols_offset = struct.unpack_from(endian + "I", data, current + 0x10)[0]
        symbols_size = struct.unpack_from(endian + "I", data, current + 0x14)[0]
        entry_size = struct.unpack_from(endian + "I", data, current + 0x24)[0] or 16
        for offset in range(symbols_offset, symbols_offset + symbols_size, entry_size):
            value, size = struct.unpack_from(endian + "II", data, offset + 0x04)
            symbol_section = struct.unpack_from(endian + "H", data, offset + 0x0E)[0]
            if symbol_section == target_index and value + size > args.size:
                struct.pack_into(endian + "I", data, offset + 0x08, max(0, args.size - value))

    # Relocations are ordered by offset. Trimming the target section can leave
    # tail relocations pointing beyond its new end; binutils 2.42 dereferences
    # those invalid offsets during final linking instead of rejecting them.
    # Shorten each associated REL/RELA section at the first removed byte.
    for index in range(section_count):
        current = header(index)
        section_type = struct.unpack_from(endian + "I", data, current + 0x04)[0]
        relocated_index = struct.unpack_from(endian + "I", data, current + 0x1C)[0]
        if section_type not in (4, 9) or relocated_index != target_index:
            continue
        relocations_offset = struct.unpack_from(endian + "I", data, current + 0x10)[0]
        relocations_size = struct.unpack_from(endian + "I", data, current + 0x14)[0]
        default_entry_size = 12 if section_type == 4 else 8
        entry_size = (
            struct.unpack_from(endian + "I", data, current + 0x24)[0]
            or default_entry_size
        )
        kept_size = relocations_size
        for relative in range(0, relocations_size, entry_size):
            relocation_offset = struct.unpack_from(
                endian + "I", data, relocations_offset + relative
            )[0]
            if relocation_offset >= args.size:
                kept_size = relative
                break
        struct.pack_into(endian + "I", data, current + 0x14, kept_size)

    args.elf.write_bytes(data)
    return


if __name__ == "__main__":
    main()
