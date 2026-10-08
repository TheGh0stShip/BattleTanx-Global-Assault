#!/usr/bin/env python3
"""Mark a 32-bit big-endian MIPS ELF object as o32/32-bit mode."""

import pathlib
import struct
import sys


EF_MIPS_ABI_O32 = 0x00001000
EF_MIPS_32BITMODE = 0x00000100


def main() -> None:
    path = pathlib.Path(sys.argv[1])
    data = bytearray(path.read_bytes())

    if data[:6] != b"\x7fELF\x01\x02":
        raise SystemExit(f"{path}: expected a 32-bit big-endian ELF file")
    if struct.unpack_from(">H", data, 18)[0] != 8:
        raise SystemExit(f"{path}: expected a MIPS ELF file")

    flags = struct.unpack_from(">I", data, 36)[0]
    struct.pack_into(">I", data, 36, flags | EF_MIPS_ABI_O32 | EF_MIPS_32BITMODE)
    path.write_bytes(data)


if __name__ == "__main__":
    main()
