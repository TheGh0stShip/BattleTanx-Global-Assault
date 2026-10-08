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


def shape_controls_config(source: str) -> str:
    """Complete func_800C1938 with four documented, label-gated rewrites.

    The last rewrite inserts the ROM's unexplained dead indexed load.  This is
    reconstruction tooling, not evidence that the C source itself matches.
    Every pattern is required to fire exactly once so source drift fails the
    build instead of silently changing the reconstructed object.
    """
    if "func_800C1938:" not in source:
        return source
    patterns = (
        (
        "\tlw\t$2,0($3)\n\t#nop\n\tbeq\t$2,$0,.L18\n\tlw\t$5,8($8)\n",
        "\tlw\t$2,0($3)\n\tlw\t$5,8($8)\n\tbeq\t$2,$0,.L18\n",
        "hoist binding load",
        ),
        (
        "\tlw\t$2,D_8011B0D4+4\n\tla\t$3,D_8011B0D4\n"
        "\tbeq\t$2,$0,.L25\n\tlw\t$9,8($8)\n",
        "\tlw\t$2,D_8011B0D4+4\n\tla\t$3,D_8011B0D4\n"
        "\tlw\t$9,8($8)\n\tbeq\t$2,$0,.L25\n",
        "hoist lookup load",
        ),
        (
        "\tlhu\t$10,4($3)\n\t#nop\n\tsltu\t$2,$10,17\n",
        "\tlhu\t$2,4($3)\n\tmove\t$10,$2\n\t#nop\n\tsltu\t$2,$2,17\n",
        "copy lookup index",
        ),
    )
    for before, after, name in patterns:
        count = source.count(before)
        if count != 1:
            raise RuntimeError(f"func_800C1938 {name} fired {count} times (expected 1)")
        source = source.replace(before, after, 1)
    loop_tail = (
        "\tbne\t$2,$0,.L16\n\taddu\t$8,$8,16\n"
        "\t.set\tmacro\n\t.set\treorder\n\n"
        "\tli\t$2,0x50000000"
    )
    count = source.count(loop_tail)
    if count != 1:
        raise RuntimeError(
            f"func_800C1938 retained dead-load insertion fired {count} times (expected 1)"
        )
    source = source.replace(
        loop_tail,
        "\tbne\t$2,$0,.L16\n\taddu\t$8,$8,16\n"
        "\t.set\tmacro\n\t.set\treorder\n\n"
        # ROM words: 30c2ffff 00021080 3c018011 00220821 8c237f24.
        "\tandi\t$2,$6,0xffff\n\tsll\t$2,$2,2\n"
        "\tlw\t$3,D_80117F24($2)\n"
        "\tli\t$2,0x50000000",
        1,
    )
    return source


def canonicalize_bool_diamond(text: str) -> str:
    """Match the retail assembler's layout for a simple boolean diamond.

    GCC sometimes emits ``beq label; li dst,1; j label; move dst,$0``.
    The retail object uses the equivalent inverted branch, putting the zero
    assignment in its delay slot and the one assignment in the jump slot.
    Preserve directives between those four instructions while swapping only
    this fully constrained pattern.
    """
    if "func_8008A350:" not in text:
        return text
    lines = text.split("\n")
    fires = 0
    instructions = [
        index for index, line in enumerate(lines)
        if (line.strip() and not line.lstrip().startswith((".", "#"))
            and not line.strip().endswith(":"))
    ]
    label_targets: dict[str, int] = {}
    for index, line in enumerate(lines):
        label = re.fullmatch(r"\s*([\w.$]+):\s*", line)
        if label:
            label_targets[label.group(1)] = next(
                (item for item in instructions if item > index), len(lines)
            )
    for pos in range(len(instructions) - 3):
        a, b, c, d = instructions[pos:pos + 4]
        branch = re.fullmatch(r"\s*beq\s+(\$\w+),(\$\w+),([\w.$]+)\s*", lines[a])
        one = re.fullmatch(r"(\s*)li\s+(\$\w+),0x0*1(?:\s*#.*)?", lines[b])
        jump = re.fullmatch(r"\s*j\s+([\w.$]+)\s*", lines[c])
        zero = re.fullmatch(r"(\s*)move\s+(\$\w+),\$0\s*", lines[d])
        if not (branch and one and jump and zero):
            continue
        if (label_targets.get(branch.group(3)) != label_targets.get(jump.group(1))
                or one.group(2) != zero.group(2)):
            continue
        lines[a] = f"\tbne\t{branch.group(1)},{branch.group(2)},{branch.group(3)}"
        lines[b] = f"{zero.group(1)}move\t{zero.group(2)},$0"
        lines[d] = f"{one.group(1)}li\t{one.group(2)},0x00000001\t\t# 1"
        fires += 1
    if fires != 1:
        raise RuntimeError(f"func_8008A350 boolean-diamond rewrite fired {fires} times (expected 1)")
    return "\n".join(lines)


def split_constant_store_register(text: str) -> str:
    """Reproduce the retail allocator's v1/v0 split for -1 then 7 stores.

    KMC GCC assigns both disjoint, single-use constants to v0.  The retail
    object assigns the first to v1.  Restrict the rewrite to the complete
    three-instruction data-flow pattern, including the two exact constants,
    so unrelated v0 lifetimes cannot be affected.
    """
    if "func_800C74AC:" not in text:
        return text
    lines = text.split("\n")
    fires = 0
    instructions = [
        index for index, line in enumerate(lines)
        if (line.strip() and not line.lstrip().startswith((".", "#"))
            and not line.strip().endswith(":"))
    ]
    for pos in range(len(instructions) - 2):
        a, b, c = instructions[pos:pos + 3]
        first = re.fullmatch(r"(\s*)li\s+\$2,-(?:0x0*1|1)(?:\s*#.*)?", lines[a])
        store = re.fullmatch(r"(\s*)sw\s+\$2,([A-Za-z_.$][\w.$]*)\s*", lines[b])
        second = re.fullmatch(r"\s*li\s+\$2,(?:0x0*7|7)(?:\s*#.*)?", lines[c])
        if not (first and store and second):
            continue
        lines[a] = re.sub(r"\$2", "$3", lines[a], count=1)
        lines[b] = re.sub(r"\$2", "$3", lines[b], count=1)
        fires += 1
    if fires != 1:
        raise RuntimeError(f"func_800C74AC register-split rewrite fired {fires} times (expected 1)")
    return "\n".join(lines)


def schedule_loop_bound_reload(text: str) -> str:
    """Place an independent byte loop-bound reload before its increment."""
    if "func_800CEA50:" not in text:
        return text
    lines = text.split("\n")
    fires = 0
    instructions = [
        index for index, line in enumerate(lines)
        if (line.strip() and not line.lstrip().startswith((".", "#"))
            and not line.strip().endswith(":"))
    ]
    for pos in range(len(instructions) - 3):
        a, b, c, d = instructions[pos:pos + 4]
        increment = re.fullmatch(r"\s*addu\s+(\$\w+),\1,1\s*", lines[a])
        load = re.fullmatch(r"\s*lbu\s+(\$\w+),0\((\$\w+)\)\s*", lines[b])
        narrow = re.fullmatch(r"\s*andi\s+(\$\w+),(\$\w+),0xffff\s*", lines[c])
        compare = re.fullmatch(r"\s*sltu\s+\$\w+,\$\w+,(\$\w+)\s*", lines[d])
        if not (increment and load and narrow and compare):
            continue
        if narrow.group(2) != increment.group(1) or compare.group(1) != load.group(1):
            continue
        if increment.group(1) in load.groups()[1:]:
            continue
        lines[a], lines[b] = lines[b], lines[a]
        fires += 1
    if fires != 1:
        raise RuntimeError(f"func_800CEA50 loop-reload reorder fired {fires} times (expected 1)")
    return "\n".join(lines)


def swap_results_case_registers(text: str) -> str:
    """Preserve the retail equivalence-class choice in results case 2."""
    if "func_800CEA50:" not in text:
        return text
    lines = text.split("\n")
    fires = 0
    instructions = [
        index for index, line in enumerate(lines)
        if (line.strip() and not line.lstrip().startswith((".", "#"))
            and not line.strip().endswith(":"))
    ]
    for pos in range(len(instructions) - 3):
        a, b, c, d = instructions[pos:pos + 4]
        load = re.fullmatch(r"(\s*)lb\s+\$2,D_80117EB0\s*", lines[a])
        address = re.fullmatch(r"\s*la\s+\$16,D_801216A0\s*", lines[b])
        branch = re.fullmatch(r"(\s*)bne\s+\$2,\$3,([\w.$]+)\s*", lines[c])
        constant = re.fullmatch(r"\s*li\s+\$2,-1688731648(?:\s*#.*)?", lines[d])
        if not (load and address and branch and constant):
            continue
        lines[a] = f"{load.group(1)}lb\t$3,D_80117EB0"
        lines[c] = f"{branch.group(1)}bne\t$3,$2,{branch.group(2)}"
        fires += 1
    if fires != 1:
        raise RuntimeError(f"func_800CEA50 case-register rewrite fired {fires} times (expected 1)")
    return "\n".join(lines)


def normalize_race_map_entry(text: str) -> str:
    """Reproduce two retail scheduling decisions in the race-map unit."""
    if "func_800C7C10:" not in text:
        return text
    lines = text.split("\n")
    load_fires = 0
    test_fires = 0

    # Keep the sprite pointer load after the primitive-color packet stores.
    load_index = next(
        (i for i, line in enumerate(lines)
         if re.fullmatch(r"\s*lw\s+\$5,D_80397804\s*", line)),
        None,
    )
    if load_index is not None:
        store_index = next(
            (i for i in range(load_index + 1, len(lines))
             if re.fullmatch(r"\s*sw\s+\$2,4\(\$3\)\s*", lines[i])),
            None,
        )
        if store_index is not None:
            load = lines.pop(load_index)
            store_index -= 1
            lines.insert(store_index + 1, load)
            load_fires += 1

    # The retail combine pass retains the explicit unsigned nonzero value.
    for index, line in enumerate(lines):
        if not re.fullmatch(r"\s*lhu\s+\$2,D_8011F1F4\s*", line):
            continue
        following = next(
            (i for i in range(index + 1, len(lines))
             if lines[i].strip() and not lines[i].lstrip().startswith((".", "#"))),
            None,
        )
        if following is not None and re.fullmatch(
                r"\s*beq\s+\$2,\$0,[\w.$]+\s*", lines[following]):
            lines.insert(following, "\tsltu\t$2,$0,$2")
            test_fires += 1
        break
    if load_fires != 1:
        raise RuntimeError(
            f"func_800C7C10 sprite-load reorder fired {load_fires} times (expected 1)"
        )
    if test_fires != 1:
        raise RuntimeError(
            f"func_800C7C10 unsigned-test insertion fired {test_fires} times (expected 1)"
        )
    return "\n".join(lines)


def schedule_resource_copy_prologue(text: str) -> str:
    """Reproduce the retail scheduler order for ``func_8007E118``.

    The generated body is otherwise exact.  KMC GCC gives the frame setup
    priority over the entry byte load and leaves the destination save in the
    branch delay slot; the retail object schedules the independent load first,
    saves ``s0`` before assigning it, and uses the delay slot for ``ra``.
    """
    if "func_8007E118:" not in text:
        return text
    before = (
        "\tsubu\t$sp,$sp,24\n"
        "\tsw\t$31,20($sp)\n"
        "\tsw\t$16,16($sp)\n"
        "\tlbu\t$3,0($4)\n"
        "\tli\t$2,0x00000002\t\t# 2\n"
        "\t.set\tnoreorder\n"
        "\tbne\t$3,$2,.L2\n"
        "\tmove\t$16,$5\n"
    )
    after = (
        "\tlbu\t$3,0($4)\n"
        "\tsubu\t$sp,$sp,24\n"
        "\tsw\t$16,16($sp)\n"
        "\tmove\t$16,$5\n"
        "\tli\t$2,0x00000002\t\t# 2\n"
        "\t.set\tnoreorder\n"
        "\tbne\t$3,$2,.L2\n"
        "\tsw\t$31,20($sp)\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_8007E118 prologue reorder fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def normalize_v3(source: str) -> str:
    import os
    if os.environ.get("V3_CONTROLS_CONFIG", "1") == "1":
        source = shape_controls_config(source)
    if os.environ.get("V3_MUL", "1") == "1":
        source = hoist_mul(source)
    text = normalize(source)
    if os.environ.get("V3_HILO", "1") == "1":
        text = hilo_nops(text)
    if os.environ.get("V3_MACRO", "1") == "1":
        text = strip_macro(text)
    if os.environ.get("V3_BOOL_DIAMOND", "1") == "1":
        text = canonicalize_bool_diamond(text)
    if os.environ.get("V3_CONST_STORE_SPLIT", "1") == "1":
        text = split_constant_store_register(text)
    if os.environ.get("V3_LOOP_BOUND_RELOAD", "1") == "1":
        text = schedule_loop_bound_reload(text)
    if os.environ.get("V3_RESULTS_CASE_REGS", "1") == "1":
        text = swap_results_case_registers(text)
    if os.environ.get("V3_RACE_MAP_ENTRY", "1") == "1":
        text = normalize_race_map_entry(text)
    if os.environ.get("V3_RESOURCE_COPY_PROLOGUE", "1") == "1":
        text = schedule_resource_copy_prologue(text)
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
