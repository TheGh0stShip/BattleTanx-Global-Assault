#!/usr/bin/env python3
"""Make IDO 5.3 assembler objects acceptable to GNU ld: put LOCAL symbols before GLOBAL ones in .symtab.

usage: fix_ido_symtab.py OBJ.o            (rewrites in place)

IDO's as1 emits local labels after global symbols in .symtab, so sh_info (index of the first non-local symbol)
is smaller than some local indices and GNU ld rejects the object ("local symbol at index N (>= sh_info)").
This script stably partitions the symbol table (index 0 stays first, then all STB_LOCAL symbols, then the rest),
sets .symtab sh_info to the first non-local index, and rewrites the symbol index of every relocation in
SHT_REL/SHT_RELA sections. Section contents (.text etc.) are not modified, so the comparison stays honest.
ELF32 big-endian only (MIPS o32).
"""
import struct, sys

SHT_SYMTAB, SHT_REL, SHT_RELA = 2, 9, 4


def main(path, mips3_32=False):
    d = bytearray(open(path, "rb").read())
    assert d[:4] == b"\x7fELF" and d[4] == 1 and d[5] == 2, "ELF32 big-endian expected"
    if mips3_32:
        flags, = struct.unpack_from(">I", d, 0x24)
        # IDO omits the GNU EF_MIPS_32BITMODE and ABI_O32 metadata from
        # assembler objects built with `-mips3 -32`. The instructions and ABI
        # are already 32-bit; add only the missing ELF attributes so GNU ld
        # does not mistake the object for 64-bit MIPS III code.
        struct.pack_into(">I", d, 0x24, flags | 0x1100)
    e_shoff, = struct.unpack_from(">I", d, 0x20)
    e_shentsize, e_shnum = struct.unpack_from(">HH", d, 0x2E)
    shs = []
    for i in range(e_shnum):
        o = e_shoff + i * e_shentsize
        shs.append((o,) + struct.unpack_from(">IIIIIIIIII", d, o))
    # (hdr_off, name, type, flags, addr, offset, size, link, info, addralign, entsize)
    symtab_idx = next(i for i, s in enumerate(shs) if s[2] == SHT_SYMTAB)
    so, _, _, _, _, off, size, link, info, _, ent = shs[symtab_idx]
    n = size // ent
    syms = [bytes(d[off + k * ent: off + (k + 1) * ent]) for k in range(n)]
    bind = lambda s: s[12] >> 4
    order = [0] + [k for k in range(1, n) if bind(syms[k]) == 0] + [k for k in range(1, n) if bind(syms[k]) != 0]
    new_index = {old: new for new, old in enumerate(order)}
    first_global = 1 + sum(1 for k in range(1, n) if bind(syms[k]) == 0)
    for new, old in enumerate(order):
        d[off + new * ent: off + (new + 1) * ent] = syms[old]
    struct.pack_into(">I", d, so + 0x1C, first_global)   # sh_info
    fixed = 0
    for s in shs:
        if s[2] in (SHT_REL, SHT_RELA) and s[7] == symtab_idx:
            roff, rsize, rent = s[5], s[6], s[10]
            for k in range(rsize // rent):
                p = roff + k * rent + 4
                r_info, = struct.unpack_from(">I", d, p)
                struct.pack_into(">I", d, p, (new_index[r_info >> 8] << 8) | (r_info & 0xFF))
                fixed += 1
    open(path, "wb").write(d)
    return n, first_global, fixed


if __name__ == "__main__":
    mips3_32 = "--mips3-32" in sys.argv[2:]
    n, fg, fx = main(sys.argv[1], mips3_32)
    print("symbols %d, first global %d, relocations remapped %d" % (n, fg, fx))
