#!/usr/bin/env python3
"""Splice fixed-VMA unit .rodata sections from the linked ELF into the code image."""

from __future__ import annotations

import argparse
import struct
from pathlib import Path

import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from place_unit_rodata import MAIN_ROM, MAIN_VRAM, load_table  # noqa: E402


def sections(data: bytes) -> dict[str, tuple[int, int, int, int]]:
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2:
        raise SystemExit("expected a big-endian ELF32 file")
    shoff = struct.unpack_from(">I", data, 0x20)[0]
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)
    def hdr(i: int) -> tuple:
        return struct.unpack_from(">IIIIIIIIII", data, shoff + i * shentsize)
    names = hdr(shstrndx)[4]
    out = {}
    for i in range(shnum):
        name, _type, flags, addr, offset, size, *_ = hdr(i)
        end = data.index(0, names + name)
        out[data[names + name:end].decode()] = (addr, offset, size, flags)
    return out


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("elf", type=Path)
    parser.add_argument("table", type=Path)
    parser.add_argument("image", type=Path)
    args = parser.parse_args()

    elf = args.elf.read_bytes()
    found = sections(elf)
    # Replacement for ld's section overlap check (disabled for this link):
    # every allocated section other than the aliasing unit rodata must have a
    # disjoint VMA range.
    alloc = sorted(
        (addr, addr + size, name)
        for name, (addr, _off, size, flags) in found.items()
        if flags & 2 and size and not name.startswith(".unit_rodata_")
    )
    for (s0, e0, n0), (s1, _e1, n1) in zip(alloc, alloc[1:]):
        if e0 > s1:
            raise SystemExit(f"section {n0} overlaps {n1}")
    image = bytearray(args.image.read_bytes())
    for unit, vram, size in load_table(args.table):
        name = f".unit_rodata_{unit}"
        if name not in found:
            raise SystemExit(f"missing {name} in {args.elf}")
        addr, offset, actual, _flags = found[name]
        if addr != vram or actual != size:
            raise SystemExit(
                f"{name}: linked at 0x{addr:08X} size 0x{actual:X}, "
                f"expected 0x{vram:08X} size 0x{size:X}"
            )
        rom = vram - MAIN_VRAM + MAIN_ROM
        image[rom:rom + size] = elf[offset:offset + size]
    args.image.write_bytes(image)


if __name__ == "__main__":
    main()
