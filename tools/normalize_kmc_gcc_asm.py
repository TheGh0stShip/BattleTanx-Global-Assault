#!/usr/bin/env python3
"""Make KMC GCC 2.7.2 assembly scheduling deterministic.

GCC already wraps transfers with intentionally filled delay slots in explicit
``.set noreorder`` regions.  KMC GNU as 2.6 otherwise schedules nearby
instructions into delay slots, while the game objects being matched retain an
empty slot.  Keep the compiler-owned regions intact, disable assembler
reordering everywhere else, and add the missing delay-slot nop after transfers
that GCC emitted while reorder mode was active.
"""

from __future__ import annotations

import argparse
import re
import struct
from pathlib import Path


TRANSFER = re.compile(
    r"^(?:b(?:al|eq|ne|gez|gezal|gtz|lez|ltz|ltzal|c1[ft]|c1fl|c1tl)?|"
    r"j|jal|jr|jalr)\s+"
)

# The assembler used for the retail objects expands GCC's internal ``b 3f``
# in its 64-bit variable-shift sequence as ``bgez $zero,3f``.  GNU as 2.6's
# default ``b`` expansion is the equivalent ``beq $zero,$zero,3f`` instead.
# Keep this deliberately limited to GCC's numeric helper label so ordinary
# source-level unconditional branches retain their normal encoding.
LONG_SHIFT_BRANCH = re.compile(r"^(\s*)b(\s+)3f(\s*(?:#.*)?(?:\r?\n)?)$")
FP_COMPARE = re.compile(r"^c\.(?:eq|lt|le)\.[sd]\s+")
INDEXED_SYMBOL_LOAD = re.compile(
    r"^(\s*)(lb|lbu|lh|lhu|lw)(\s+)(\$\w+),(D_[0-9A-Fa-f]+(?:[+-][0-9]+)?)\((\$\w+)\)"
    r"(\s*(?:#.*)?(?:\r?\n)?)$"
)


FCMP = re.compile(r"^c\.[a-z]+\.[sd]\s")
BC1 = re.compile(r"^bc1[ft]l?\s")
# `op rD,SYM[+-off]($rB)`: GCC leaves the indexed-global macro to the
# assembler.  KMC as 2.6 builds the address in rD for loads (rD != rB);
# the retail assembler always used $at.  Expand it explicitly.
INDEXED = re.compile(
    r"^(lw|lh|lhu|lb|lbu|ld|lwc1|ldc1|sw|sh|sb|sd|swc1|sdc1)\s+"
    r"(\$\w+),([A-Za-z_.$][\w.$]*(?:[+-]\d+)?)\((\$\w+)\)$"
)


LARGE_OFFSET = re.compile(
    r"^(lw|lh|lhu|lb|lbu|ld|lwc1|ldc1|sw|sh|sb|sd|swc1|sdc1)\s+"
    r"(\$\w+),(-?\d+)\((\$\w+)\)$"
)


def expand_indexed(instruction: str, newline: str) -> str | None:
    # Out-of-range constant offsets get the same $at macro expansion.
    large = LARGE_OFFSET.match(instruction)
    if large and not -0x8000 <= int(large.group(3)) <= 0x7FFF:
        op, rd, offset, rb = large.groups()
        value = int(offset) & 0xFFFFFFFF
        low = value & 0xFFFF
        low = low - 0x10000 if low >= 0x8000 else low
        high = ((value - low) >> 16) & 0xFFFF
        return (
            f"\t.set\tnoat{newline}\tlui\t$1,{high}{newline}" +
            (f"\taddu\t$1,{rb},$1{newline}" if int(offset) > 0 else f"\taddu\t$1,$1,{rb}{newline}")
            + f"\t{op}\t{rd},{low}($1){newline}"
            f"\t.set\tat{newline}"
        )
    match = INDEXED.match(instruction)
    if not match or match.group(4) in ("$sp", "$fp", "$0", "$29", "$30"):
        return None
    op, rd, sym, rb = match.groups()
    return (
        f"\t.set\tnoat{newline}\tlui\t$1,%hi({sym}){newline}"
        f"\taddu\t$1,$1,{rb}{newline}\t{op}\t{rd},%lo({sym})($1){newline}"
        f"\t.set\tat{newline}"
    )


LI_FLOAT = re.compile(r"^(\s*)li\.([sd])\s+(\$f\d+)\s*,\s*(\S+)\s*$")


def expand_float_literal(line: str, counter: list[int]) -> list[str] | None:
    """Rewrite ``li.s``/``li.d`` into an explicit per-use .rodata literal.

    The retail objects load every FP constant from .rodata with lui/lwc1 in
    source order, one literal per use (no merging).  KMC as 2.6 instead
    expands constants whose low halfword is zero into ``lui; mtc1``.
    """
    m = LI_FLOAT.match(line.rstrip("\n"))
    if not m:
        return None
    indent, kind, reg, value = m.groups()
    label = f"$LF_lis{counter[0]}"
    counter[0] += 1
    if kind == "s":
        bits = struct.unpack(">I", struct.pack(">f", float(value)))[0]
        data = [f"\t.align\t2\n", f"{label}:\n", f"\t.word\t0x{bits:08X}\n"]
    else:
        hi, lo = struct.unpack(">II", struct.pack(">d", float(value)))
        data = [f"\t.align\t3\n", f"{label}:\n",
                f"\t.word\t0x{hi:08X}\n", f"\t.word\t0x{lo:08X}\n"]
    return (["\t.section\t.rodata\n"] + data + ["\t.text\n",
            f"{indent}l.{kind}\t{reg},{label}\n"])


def normalize(source: str) -> str:
    # GNU as starts in reorder mode. Merely adding an explicit NOP does not
    # prevent it from moving an earlier instruction into that slot.
    output: list[str] = ["\t.set\tnoreorder\n"]
    compiler_reorder = True
    after_fcmp = False
    counter = [0]
    previous_instruction = ""

    for line in source.splitlines(keepends=True):
        line = LONG_SHIFT_BRANCH.sub(r"\1bgez\2$zero,3f\3", line)
        indexed = INDEXED_SYMBOL_LOAD.match(line)
        if indexed:
            indent, op, spacing, dest, symbol, base, suffix = indexed.groups()
            newline = "\n" if line.endswith("\n") else ""
            output.append(f"{indent}lui{spacing}$at,%hi({symbol}){newline}")
            output.append(f"{indent}addu{spacing}$at,$at,{base}{newline}")
            line = f"{indent}{op}{spacing}{dest},%lo({symbol})($at){suffix}"
        stripped = line.strip()

        # GCC leaves hazard placeholders as comments.  The retail assembler
        # materialized the one-cycle FP compare hazard, while our global
        # noreorder mode cannot do so itself.
        if stripped == "#nop" and FP_COMPARE.match(previous_instruction):
            newline = "\n" if line.endswith("\n") else ""
            line = "\tnop" + newline
            stripped = "nop"

        if stripped == ".set\tnoreorder" or stripped == ".set noreorder":
            compiler_reorder = False
            output.append(line)
            continue

        if stripped == ".set\treorder" or stripped == ".set reorder":
            compiler_reorder = True
            newline = "\n" if line.endswith("\n") else ""
            output.append("\t.set\tnoreorder" + newline)
            continue

        lis = expand_float_literal(line, counter)
        if lis is not None:
            output.extend(lis)
            continue
        instruction = stripped.split("#", 1)[0].strip()
        newline = "\n" if line.endswith("\n") else ""
        if after_fcmp and instruction and not instruction.startswith("."):
            # The retail assembler kept the FP compare -> bc1x hazard nop
            # that KMC as only inserts in reorder mode.
            if BC1.match(instruction):
                output.append("\tnop" + newline)
            after_fcmp = False
        if compiler_reorder and FCMP.match(instruction):
            after_fcmp = True
        if instruction == "b\t3f" or instruction == "b 3f":
            # libgcc-style 64-bit shift template: the retail assembler
            # encoded the unconditional branch as bgez $0 (0x0401xxxx).
            line = line.replace("b\t3f", "bgez\t$0,3f").replace("b 3f", "bgez $0,3f")
        expanded = expand_indexed(instruction, newline)
        output.append(expanded if expanded is not None else line)
        if compiler_reorder and TRANSFER.match(instruction):
            newline = "\n" if line.endswith("\n") else ""
            output.append("\tnop" + newline)
        if instruction:
            previous_instruction = instruction

    # KMC as 2.6 inserts a hazard nop after a blank line that directly
    # follows ".set noreorder"; drop such blank lines.
    lines = "".join(output).split("\n")
    kept: list[str] = []
    for index, item in enumerate(lines):
        # The final empty item only carries the input's trailing newline.
        if (item.strip() == "" and index != len(lines) - 1 and kept
                and kept[-1].split() == [".set", "noreorder"]):
            continue
        kept.append(item)
    return "\n".join(kept)



# --- v3 additions -------------------------------------------------------
# (1) Retail code never places mul.s/mul.d in a branch/jump delay slot
# (VR4300 multiply errata workaround in the original toolchain); KMC GCC
# 2.7.2 does.  Hoist the mul above the transfer and leave a nop in the slot.
MUL_SLOT = re.compile(
    r"(\t\.set\tnoreorder\n\t\.set\tnomacro\n)"
    r"(\t(?:b(?:eq|ne|gez|gtz|lez|ltz|c1[ft])|j|jal|jr|jalr)\t[^\n]*\n)"
    r"(\tmul\.[sd]\t[^\n]*\n)")


def hoist_mul(source: str) -> str:
    return MUL_SLOT.sub(lambda m: m.group(3) + m.group(1) + m.group(2) + "\tnop\n", source)


# (2) GCC's '#nop' hi/lo hazard placeholders after mflo/mfhi become real
# nops when the next instruction is mult/div (KMC as -mips3 omits them).
HILO_USE = ("mult", "multu", "div", "divu", "dmult", "dmultu", "ddiv", "ddivu")


def hilo_nops(text: str) -> str:
    lines = text.split("\n")
    op = lambda s: s.split()[0] if s.split() else ""
    for i, line in enumerate(lines):
        if op(line) in ("mflo", "mfhi"):
            j = i + 1
            while j < len(lines) and lines[j].strip() == "#nop":
                j += 1
            if j > i + 1 and op(lines[j]) in HILO_USE:
                for k in range(i + 1, j):
                    lines[k] = "\tnop"
    return "\n".join(lines)


# (3) KMC as 2.6 inserts a spurious nop after `.set macro` in some FP
# store/op sequences; drop the macro/nomacro toggles and blank lines.
def strip_macro(text: str) -> str:
    text = re.sub(r"\n\s*\n", "\n", text)
    return re.sub(r"^\t\.set\t(no)?macro\n", "", text, flags=re.M)


def normalize_v3(source: str) -> str:
    import os
    if os.environ.get("V3_MUL", "1") == "1":
        source = hoist_mul(source)
    text = normalize(source)
    if os.environ.get("V3_HILO", "1") == "1":
        text = hilo_nops(text)
    if os.environ.get("V3_MACRO", "1") == "1":
        text = strip_macro(text)
    return text


def normalize_legacy(source: str) -> str:
    """Normalize pre-0x800C4000 units with their proven assembler rules."""
    output: list[str] = ["\t.set\tnoreorder\n"]
    compiler_reorder = True
    previous_instruction = ""
    for line in source.splitlines(keepends=True):
        line = LONG_SHIFT_BRANCH.sub(r"\1bgez\2$zero,3f\3", line)
        indexed = INDEXED_SYMBOL_LOAD.match(line)
        if indexed:
            indent, op, spacing, dest, symbol, base, suffix = indexed.groups()
            newline = "\n" if line.endswith("\n") else ""
            output.append(f"{indent}lui{spacing}$at,%hi({symbol}){newline}")
            output.append(f"{indent}addu{spacing}$at,$at,{base}{newline}")
            line = f"{indent}{op}{spacing}{dest},%lo({symbol})($at){suffix}"
        stripped = line.strip()
        if stripped == "#nop" and FP_COMPARE.match(previous_instruction):
            newline = "\n" if line.endswith("\n") else ""
            line = "\tnop" + newline
            stripped = "nop"
        if stripped in (".set\tnoreorder", ".set noreorder"):
            compiler_reorder = False
            output.append(line)
            continue
        if stripped in (".set\treorder", ".set reorder"):
            compiler_reorder = True
            newline = "\n" if line.endswith("\n") else ""
            output.append("\t.set\tnoreorder" + newline)
            continue
        output.append(line)
        instruction = stripped.split("#", 1)[0].strip()
        if compiler_reorder and line.startswith("\t") and TRANSFER.match(instruction):
            newline = "\n" if line.endswith("\n") else ""
            output.append("\tnop" + newline)
        if instruction:
            previous_instruction = instruction
    return "".join(output)

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--legacy", action="store_true")
    args = parser.parse_args()
    source = args.input.read_text()
    args.output.write_text(normalize_legacy(source) if args.legacy else normalize_v3(source))


if __name__ == "__main__":
    main()
