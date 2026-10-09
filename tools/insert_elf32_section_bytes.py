#!/usr/bin/env python3
"""Insert zero bytes before a symbol inside a fixed-size ELF32 section.

Some IDO-era objects were assembled with stricter inter-function alignment
than the assembler used by the reconstructed toolchain.  This shifts the
section contents in place, consuming zero tail padding, and updates symbols
and relocations without changing the section's total size.
"""

from __future__ import annotations

import argparse
import struct
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("elf", type=Path)
    parser.add_argument("section")
    parser.add_argument("before_symbol")
    parser.add_argument("count", type=lambda value: int(value, 0))
    parser.add_argument(
        "--unschedule-return",
        action="store_true",
        help="move the final delay-slot instruction before jr ra",
    )
    args = parser.parse_args()
    if args.count <= 0 or args.count % 4:
        raise SystemExit("count must be a positive multiple of four")

    data = bytearray(args.elf.read_bytes())
    if data[:4] != b"\x7fELF" or data[4] != 1:
        raise SystemExit("expected an ELF32 object")
    endian = ">" if data[5] == 2 else "<"
    shoff = struct.unpack_from(endian + "I", data, 0x20)[0]
    shentsize, shnum, shstrndx = struct.unpack_from(endian + "HHH", data, 0x2E)

    def shdr(index: int) -> int:
        return shoff + index * shentsize

    def word(offset: int) -> int:
        return struct.unpack_from(endian + "I", data, offset)[0]

    def put_word(offset: int, value: int) -> None:
        struct.pack_into(endian + "I", data, offset, value)

    names_offset = word(shdr(shstrndx) + 0x10)

    def section_name(index: int) -> str:
        start = names_offset + word(shdr(index))
        return data[start : data.index(0, start)].decode()

    matches = [index for index in range(shnum) if section_name(index) == args.section]
    if len(matches) != 1:
        raise SystemExit(f"expected one {args.section} section, found {len(matches)}")
    target_index = matches[0]
    target = shdr(target_index)
    target_offset = word(target + 0x10)
    target_size = word(target + 0x14)

    symbol_hits: list[tuple[int, int, int]] = []
    symbol_tables: dict[int, tuple[int, int, int, int]] = {}
    for index in range(shnum):
        current = shdr(index)
        if word(current + 0x04) != 2:  # SHT_SYMTAB
            continue
        table_offset = word(current + 0x10)
        table_size = word(current + 0x14)
        entry_size = word(current + 0x24) or 16
        strings_index = word(current + 0x18)
        strings_offset = word(shdr(strings_index) + 0x10)
        symbol_tables[index] = (table_offset, table_size, entry_size, strings_offset)
        for symbol_index, relative in enumerate(range(0, table_size, entry_size)):
            entry = table_offset + relative
            name_start = strings_offset + word(entry)
            name = data[name_start : data.index(0, name_start)].decode()
            if name == args.before_symbol:
                symbol_hits.append((index, symbol_index, entry))

    if len(symbol_hits) != 1:
        raise SystemExit(
            f"expected one symbol named {args.before_symbol}, found {len(symbol_hits)}"
        )
    _, _, boundary_entry = symbol_hits[0]
    boundary = word(boundary_entry + 0x04)
    boundary_section = struct.unpack_from(endian + "H", data, boundary_entry + 0x0E)[0]
    if boundary_section != target_index or boundary + args.count > target_size:
        raise SystemExit("boundary symbol is outside the requested section")
    if any(data[target_offset + target_size - args.count : target_offset + target_size]):
        raise SystemExit("insertion would discard nonzero section tail bytes")

    original = bytes(data[target_offset : target_offset + target_size])
    if args.unschedule_return:
        if boundary < 8 or word(target_offset + boundary - 8) != 0x03E00008:
            raise SystemExit("expected jr ra immediately before its delay slot")
        original = (
            original[: boundary - 8]
            + original[boundary - 4 : boundary]
            + original[boundary - 8 : boundary - 4]
            + original[boundary:]
        )
    data[target_offset : target_offset + boundary] = original[:boundary]
    data[target_offset + boundary + args.count : target_offset + target_size] = (
        original[boundary : target_size - args.count]
    )
    data[target_offset + boundary : target_offset + boundary + args.count] = (
        b"\0" * args.count
    )

    # Move symbols defined at or after the insertion point.
    for table_offset, table_size, entry_size, _ in symbol_tables.values():
        for relative in range(0, table_size, entry_size):
            entry = table_offset + relative
            symbol_section = struct.unpack_from(endian + "H", data, entry + 0x0E)[0]
            value = word(entry + 0x04)
            if symbol_section == target_index and value >= boundary:
                put_word(entry + 0x04, value + args.count)

    # Move relocation sites in the shifted section.  Also adjust R_MIPS_32
    # addends that refer to the target section (for example switch tables).
    for index in range(shnum):
        current = shdr(index)
        section_type = word(current + 0x04)
        if section_type not in (4, 9):  # SHT_RELA / SHT_REL
            continue
        relocated_index = word(current + 0x1C)
        relocation_offset = word(current + 0x10)
        relocation_size = word(current + 0x14)
        entry_size = word(current + 0x24) or (12 if section_type == 4 else 8)
        symtab_index = word(current + 0x18)
        symtab_offset, _, symbol_size, _ = symbol_tables[symtab_index]
        relocated_data = word(shdr(relocated_index) + 0x10)
        for relative in range(0, relocation_size, entry_size):
            entry = relocation_offset + relative
            site = word(entry)
            info = word(entry + 0x04)
            symbol_index = info >> 8
            relocation_type = info & 0xFF
            symbol_entry = symtab_offset + symbol_index * symbol_size
            symbol_section = struct.unpack_from(
                endian + "H", data, symbol_entry + 0x0E
            )[0]
            if relocated_index == target_index and site >= boundary:
                site += args.count
                put_word(entry, site)
            if symbol_section != target_index:
                continue
            # Named symbols carry their moved value in the symbol table.  A
            # section-symbol R_MIPS_32 relocation carries its offset as an
            # in-place addend, which must move too.
            symbol_type = data[symbol_entry + 0x0C] & 0x0F
            if symbol_type == 3 and relocation_type == 2:
                addend_location = relocated_data + site
                addend = word(addend_location)
                if addend >= boundary:
                    put_word(addend_location, addend + args.count)
            elif symbol_type == 3 and relocation_type not in (4, 5, 6):
                raise SystemExit(
                    f"unsupported relocation type {relocation_type} against target section"
                )

    args.elf.write_bytes(data)


if __name__ == "__main__":
    main()
