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
            hazard_use = j < len(lines) and op(lines[j]) in HILO_USE
            scheduled_gap = (not hazard_use and j + 1 < len(lines)
                    and op(lines[j]) and not op(lines[j]).startswith(".")
                    and op(lines[j + 1]) in HILO_USE)
            if scheduled_gap:
                # MIPS requires two instructions between mfhi/mflo and the
                # next mult/div.  The retail assembler schedules the one
                # independent instruction into GCC's placeholder, then emits
                # the remaining nop immediately before the mult/div.
                lines[i + 1] = lines[j]
                for k in range(i + 2, j):
                    lines[k] = ""
                lines[j] = "\tnop"
            elif j > i + 1 and hazard_use:
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


def schedule_owner_search_prologue(text: str) -> str:
    """Reproduce the two independent prologue stores in ``func_80083DF0``.

    The C body and register allocation are otherwise exact.  The retail
    scheduler saves ``s1`` before ``ra``; this compiler build chooses the
    reverse order.  Gate the rewrite to the function and require one fire so
    later source drift cannot silently broaden it.
    """
    if "func_80083DF0:" not in text:
        return text
    before = "\tsw\t$31,24($sp)\n\tsw\t$17,20($sp)\n"
    after = "\tsw\t$17,20($sp)\n\tsw\t$31,24($sp)\n"
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_80083DF0 prologue-store reorder fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def select_region_early_exit_likely(text: str) -> str:
    """Use the retail branch-likely encoding in ``func_800AA5D0``.

    GCC emits an ordinary ``bc1t`` for the first early exit even though its
    delay-slot zero is dead on the fallthrough path.  The retail object uses
    ``bc1tl`` at this site.  The surrounding FP comparison gates this single
    opcode rename; the later, similar exit remains an ordinary ``bc1t``.
    """
    if "func_800AA5D0:" not in text:
        return text
    before = (
        "\tc.le.s\t$f4,$f6\n"
        "\tnop\n"
        "\t.set\tnoreorder\n"
        "\tbc1t\t.L7\n"
        "\tmove\t$2,$0\n"
    )
    after = before.replace("\tbc1t\t.L7\n", "\tbc1tl\t.L7\n")
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800AA5D0 early-exit branch rename fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def schedule_vector_angle_prologue(text: str) -> str:
    """Reproduce the retail save schedule in ``func_8009DFAC``.

    All arithmetic and control flow match from C.  The retail scheduler moves
    the independent ``f20`` save below the first multiply and fills the NaN
    check's branch delay slot with the ``ra`` save.
    """
    if "func_8009DFAC:" not in text:
        return text
    early_ra = "\tsw\t$31,16($sp)\n"
    early_f20 = "\ts.d\t$f20,24($sp)\n"
    first_mul = "\tmul.s\t$f2,$f22,$f22\n"
    nan_branch = "\tbc1t\t.L2\n\tnop\n"
    counts = {
        "ra save": text.count(early_ra),
        "f20 save": text.count(early_f20),
        "first multiply": text.count(first_mul),
        "NaN branch": text.count(nan_branch),
    }
    for name, count in counts.items():
        if count != 1:
            raise RuntimeError(
                f"func_8009DFAC {name} schedule fired {count} times (expected 1)"
            )
    text = text.replace(early_ra, "", 1).replace(early_f20, "", 1)
    text = text.replace(first_mul, first_mul + early_f20, 1)
    return text.replace(nan_branch, "\tbc1t\t.L2\n" + early_ra, 1)


def shape_selection_state_dispatch(text: str) -> str:
    """Reproduce the compact retail dispatch in ``func_8009ACDC``.

    GCC lowers the three recognized states into an equivalent inverted branch
    tree with two extra local jumps.  Replace only that label-gated tree with
    the retail ordering; the scan, register allocation, and return values come
    directly from the C source.
    """
    if "func_8009ACDC:" not in text:
        return text
    before = (
        "\tbne\t$3,$11,.L8\n"
        "\tsltu\t$2,$3,3\n"
        "\t.set\tnoreorder\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L9\n"
        "\tli\t$2,0x00000001\t\t# 1\n"
        "\t.set\tnoreorder\n"
        ".L8:\n #APP\n #NO_APP\n"
        "\tbeq\t$2,$0,.L10\n"
        "\tnop\n"
        "\t.set\tnoreorder\n"
        "\tbnel\t$3,$10,.L17\n"
        "\taddu\t$8,$8,1\n"
        "\t.set\tnoreorder\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L18\n"
        "\tmove\t$2,$0\n"
        "\t.set\tnoreorder\n"
        ".L10:\n\t.set\tnoreorder\n"
        "\tbnel\t$3,$9,.L7\n"
        "\taddu\t$8,$8,1\n"
        "\t.set\tnoreorder\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L9\n"
        "\tli\t$2,0x00000002\t\t# 2\n"
        "\t.set\tnoreorder\n"
        ".L7:\n"
    )
    after = (
        "\tbeq\t$3,$11,.Lkmc_one_8009ACDC\n"
        "\tsltu\t$2,$3,3\n"
        "\tbeq\t$2,$0,.Lkmc_ge_8009ACDC\n"
        "\tnop\n"
        "\tbeq\t$3,$10,.L9\n"
        "\tmove\t$2,$0\n"
        "\tj\t.L17\n"
        "\taddu\t$8,$8,1\n"
        ".Lkmc_ge_8009ACDC:\n"
        "\tbeq\t$3,$9,.L9\n"
        "\tli\t$2,0x00000002\t\t# 2\n"
        "\tj\t.L17\n"
        "\taddu\t$8,$8,1\n"
        ".Lkmc_one_8009ACDC:\n"
        "\tj\t.L9\n"
        "\tli\t$2,0x00000001\t\t# 1\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_8009ACDC state-dispatch rewrite fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def shape_snapshot_record_address(text: str) -> str:
    """Reproduce the retail address tree in ``func_80079FF0``.

    The unoptimized C and every surrounding instruction are exact.  GCC
    reassociates ``base + (index * 16 + 0x90)`` as
    ``(base + 0x90) + index * 16``; the retail object retains the source
    grouping.  Rewrite this single label-gated six-instruction tree and
    require exactly one occurrence so source drift cannot broaden the rule.
    """
    if "func_80079FF0:" not in text:
        return text
    before = (
        "\tmove\t$4,$3\n"
        "\tsll\t$2,$4,4\n"
        "\tlw\t$4,0($fp)\n"
        "\taddu\t$3,$4,144\n"
        "\taddu\t$2,$2,$3\n"
        "\tsw\t$2,8($fp)\n"
    )
    after = (
        "\tmove\t$2,$3\n"
        "\tsll\t$3,$2,4\n"
        "\taddu\t$2,$3,144\n"
        "\tlw\t$3,0($fp)\n"
        "\taddu\t$2,$3,$2\n"
        "\tsw\t$2,8($fp)\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_80079FF0 record-address rewrite fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def schedule_large_search_prologue(text: str) -> str:
    """Reproduce independent argument setup before the local-buffer address.

    In ``func_800A2B9C`` the C body, frame, and loop are exact.  The retail
    scheduler prepares ``a0``/``a1`` before saving and forming ``s0``; GCC's
    emitted order is the reverse.  Keep this label-gated and exactly-once.
    """
    if "func_800A2B9C:" not in text:
        return text
    before = (
        "\tsw\t$16,1184($sp)\n"
        "\taddu\t$16,$sp,24\n"
        "\tandi\t$4,$4,0xffff\n"
        "\tli\t$5,0x00400000\t\t# 4194304\n"
        "\tori\t$5,$5,0x1100\n"
    )
    after = (
        "\tandi\t$4,$4,0xffff\n"
        "\tli\t$5,0x00400000\t\t# 4194304\n"
        "\tori\t$5,$5,0x1100\n"
        "\tsw\t$16,1184($sp)\n"
        "\taddu\t$16,$sp,24\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800A2B9C search-prologue reorder fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def schedule_region_query_call(text: str) -> str:
    """Reproduce independent save and call-argument scheduling in 800ACFE0."""
    if "func_800ACFE0:" not in text:
        return text
    patterns = (
        (
            "\tsw\t$31,52($sp)\n"
            "\tsw\t$22,48($sp)\n"
            "\tsw\t$20,40($sp)\n"
            "\tsw\t$19,36($sp)\n"
            "\tsw\t$18,32($sp)\n"
            "\tsw\t$17,28($sp)\n"
            "\tsw\t$16,24($sp)\n"
            " #APP\n #NO_APP\n"
            "\tmove\t$18,$0\n"
            "\tla\t$6,D_802194A5\n",
            "\tsw\t$18,32($sp)\n"
            "\tmove\t$18,$0\n"
            "\tla\t$6,D_802194A5\n"
            "\tsw\t$31,52($sp)\n"
            "\tsw\t$22,48($sp)\n"
            "\tsw\t$20,40($sp)\n"
            "\tsw\t$19,36($sp)\n"
            "\tsw\t$17,28($sp)\n"
            "\tsw\t$16,24($sp)\n"
            " #APP\n #NO_APP\n",
            "prologue",
        ),
        (
            "\tmove\t$6,$17\n"
            "\tmove\t$4,$21\n"
            "\t.set\tnoreorder\n"
            "\tjal\tfunc_800AA8A8\n"
            "\tandi\t$5,$20,0x00ff\n",
            "\tmove\t$4,$21\n"
            "\tandi\t$5,$20,0x00ff\n"
            "\t.set\tnoreorder\n"
            "\tjal\tfunc_800AA8A8\n"
            "\tmove\t$6,$17\n",
            "call arguments",
        ),
    )
    for before, after, name in patterns:
        fires = text.count(before)
        if fires != 1:
            raise RuntimeError(
                f"func_800ACFE0 {name} schedule fired {fires} times (expected 1)"
            )
        text = text.replace(before, after, 1)
    return text


def normalize_object_phase_lookup(text: str) -> str:
    """Reproduce register allocation and expression order in ``func_800A8E84``."""
    if "func_800A8E84:" not in text:
        return text
    replacements = (
        ("\tlw\t$6,268($4)\n", "\tlw\t$5,268($4)\n", "link register"),
        ("\tlw\t$3,4($6)\n", "\tlw\t$3,4($5)\n", "link kind base"),
        ("\taddu\t$7,$4,120\n", "\taddu\t$6,$4,120\n", "object register"),
        ("\tlw\t$2,12($6)\n", "\tlw\t$2,12($5)\n", "child base"),
        ("\tsll\t$6,$2,4\n", "\tsll\t$5,$2,4\n", "child offset register"),
        ("\tsll\t$5,$2,3\n", "\tsll\t$4,$2,3\n", "phase register"),
        ("\tmove\t$4,$7\n", "\tmove\t$4,$6\n", "call object register"),
    )
    for before, after, name in replacements:
        fires = text.count(before)
        if fires != 1:
            raise RuntimeError(
                f"func_800A8E84 {name} rewrite fired {fires} times (expected 1)"
            )
        text = text.replace(before, after, 1)

    before_arms = (
        "\tla\t$2,D_80121D90\n"
        "\taddu\t$2,$6,$2\n"
        "\taddu\t$2,$5,$2\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L6\n"
        "\taddu\t$5,$2,720\n"
        "\t.set\tnoreorder\n"
        ".L5:\n"
        "\tla\t$2,D_80121D90\n"
        "\taddu\t$2,$6,$2\n"
        "\taddu\t$5,$5,$2\n"
    )
    after_arms = (
        "\taddu\t$3,$5,720\n"
        "\tla\t$2,D_80121D90\n"
        "\taddu\t$2,$4,$2\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L6\n"
        "\taddu\t$5,$3,$2\n"
        "\t.set\tnoreorder\n"
        ".L5:\n"
        "\tla\t$2,D_80121D90\n"
        "\taddu\t$2,$4,$2\n"
        "\taddu\t$5,$5,$2\n"
    )
    fires = text.count(before_arms)
    if fires != 1:
        raise RuntimeError(
            f"func_800A8E84 expression-order rewrite fired {fires} times (expected 1)"
        )
    return text.replace(before_arms, after_arms, 1)


def normalize_display_slot_wait(text: str) -> str:
    """Keep the invalid-slot value live through the first inlined search."""
    if "func_8007A818:" not in text:
        return text
    patterns = (
        (
            "\tli\t$3,-1\t\t\t# 0xffffffff\n"
            "\tlh\t$2,176($5)\n"
            ".L11:\n"
            "\t.set\tnoreorder\n"
            "\tbne\t$2,$3,.L11\n",
            "\tli\t$7,-1\t\t\t# 0xffffffff\n"
            "\tlh\t$8,176($5)\n"
            ".L11:\n"
            "\t.set\tnoreorder\n"
            "\tbne\t$8,$7,.L11\n",
            "entry invalid/work registers",
        ),
        (
            "\tbne\t$3,$2,.L32\n"
            "\tsll\t$2,$3,16\n"
            "\t.set\tnoreorder\n"
            "\taddu\t$2,$4,1\n"
            ".L31:\n",
            "\tbeq\t$3,$2,.L31\n"
            "\taddu\t$2,$4,1\n"
            "\t.set\tnoreorder\n"
            "\tj\t.L32\n"
            "\tmove\t$2,$3\n"
            "\t.set\tnoreorder\n"
            ".L31:\n",
            "first-search success branch",
        ),
        (
            "\tli\t$3,-1\t\t\t# 0xffffffff\n"
            "\tsll\t$2,$3,16\n"
            ".L32:\n"
            "\tsra\t$2,$2,16\n"
            "\tli\t$3,-1\t\t\t# 0xffffffff\n"
            "\t.set\tnoreorder\n"
            "\tbeq\t$2,$3,.L34\n",
            "\tli\t$2,-1\t\t\t# 0xffffffff\n"
            ".L32:\n"
            "\tsll\t$2,$2,16\n"
            "\tsra\t$2,$2,16\n"
            "\t.set\tnoreorder\n"
            "\tbeq\t$2,$7,.L11\n",
            "first-search failure and retry",
        ),
    )
    for before, after, name in patterns:
        fires = text.count(before)
        if fires != 1:
            raise RuntimeError(
                f"func_8007A818 {name} rewrite fired {fires} times (expected 1)"
            )
        text = text.replace(before, after, 1)
    return text


def schedule_display_record_prefix(text: str) -> str:
    """Reproduce the retail pre-call schedule in ``func_8007A8F0``."""
    if "func_8007A8F0:" not in text:
        return text
    before = (
        "\tsubu\t$sp,$sp,32\n"
        "\tsw\t$18,24($sp)\n"
        "\tmove\t$18,$5\n"
        "\tmove\t$3,$18\n"
        "\tsw\t$16,16($sp)\n"
        "\tlw\t$16,D_80114500\n"
        "\taddu\t$18,$18,8\n"
        "\tli\t$2,-385875968\t\t\t# 0xe9000000\n"
        "\tsw\t$31,28($sp)\n"
        "\tsw\t$17,20($sp)\n"
        "\tsw\t$2,0($3)\n"
        "\tsw\t$0,4($3)\n"
        "\tmove\t$3,$18\n"
        "\tli\t$2,-553648128\t\t\t# 0xdf000000\n"
        "\tsw\t$2,0($3)\n"
        "\tsw\t$0,4($3)\n"
        "\tlhu\t$3,192($16)\n"
        "\tmove\t$17,$4\n"
        "\tsll\t$2,$3,3\n"
        "\taddu\t$2,$2,$3\n"
        "\tsll\t$2,$2,3\n"
    )
    after = (
        "\tsubu\t$sp,$sp,32\n"
        "\tsw\t$16,16($sp)\n"
        "\tlw\t$16,D_80114500\n"
        "\tsw\t$17,20($sp)\n"
        "\tmove\t$17,$4\n"
        "\tsw\t$18,24($sp)\n"
        "\tmove\t$18,$5\n"
        "\tmove\t$3,$18\n"
        "\tsw\t$31,28($sp)\n"
        "\tlhu\t$4,192($16)\n"
        "\taddu\t$18,$18,8\n"
        "\tli\t$2,-385875968\t\t\t# 0xe9000000\n"
        "\tsw\t$2,0($3)\n"
        "\tsw\t$0,4($3)\n"
        "\tmove\t$3,$18\n"
        "\tli\t$2,-553648128\t\t\t# 0xdf000000\n"
        "\tsw\t$2,0($3)\n"
        "\tsw\t$0,4($3)\n"
        "\tsll\t$2,$4,3\n"
        "\taddu\t$2,$2,$4\n"
        "\tsll\t$2,$2,3\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_8007A8F0 prefix schedule fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def schedule_free_list_rebucket(text: str) -> str:
    """Reproduce two retail scheduling choices in ``func_800A1B44``.

    The C emits the exact instruction set and registers.  Retail KMC ``as``
    moves the frame allocation ahead of the early-exit branch and hoists the
    independent bucket-index reload ahead of the free-list-head store.  Keep
    both reorders label-gated and require each complete pattern exactly once.
    """
    if "func_800A1B44:" not in text:
        return text
    patterns = (
        (
            "\tlh\t$3,D_80235EF0\n"
            "\tli\t$2,-1\t\t\t# 0xffffffff\n"
            "\t.set\tnoreorder\n"
            "\tbeq\t$3,$2,.L2\n"
            "\tsubu\t$sp,$sp,8\n",
            "\tlh\t$3,D_80235EF0\n"
            "\tsubu\t$sp,$sp,8\n"
            "\tli\t$2,-1\t\t\t# 0xffffffff\n"
            "\t.set\tnoreorder\n"
            "\tbeq\t$3,$2,.L2\n"
            "\tmove\t$6,$3\n",
            "prologue",
        ),
        (
            "\tsh\t$5,D_80235EF0\n"
            "\tlui\t$at,%hi(D_80224EF4)\n"
            "\taddu\t$at,$at,$3\n"
            "\tlw\t$2,%lo(D_80224EF4)($at)\n",
            "\tlui\t$at,%hi(D_80224EF4)\n"
            "\taddu\t$at,$at,$3\n"
            "\tlw\t$2,%lo(D_80224EF4)($at)\n"
            "\tsh\t$5,D_80235EF0\n",
            "bucket reload",
        ),
    )
    for before, after, name in patterns:
        fires = text.count(before)
        if fires != 1:
            raise RuntimeError(
                f"func_800A1B44 {name} reorder fired {fires} times (expected 1)"
            )
        text = text.replace(before, after, 1)
    # The prologue replacement fills the branch slot itself; remove the old
    # post-branch copy that immediately follows GCC's second noreorder marker.
    duplicate = "\t.set\tnoreorder\n\tmove\t$6,$3\n\tla\t$7,D_80224E68\n"
    fires = text.count(duplicate)
    if fires != 1:
        raise RuntimeError(
            f"func_800A1B44 duplicate prologue copy fired {fires} times (expected 1)"
        )
    return text.replace(duplicate, "\t.set\tnoreorder\n\tla\t$7,D_80224E68\n", 1)


def reproduce_turret_sweep_assembler_hazards(text: str) -> str:
    """Reproduce two retail-assembler hazards in ``func_800E1BB0``.

    The first preserves the retail second FP-result hazard nop after a call.
    The second gives the branch target after a constant-pool load its retail
    address without presenting the label immediately after ``lwc1`` to KMC
    ``as``, which would insert an unwanted load-delay nop.
    """
    if "func_800E1BB0:" not in text:
        return text
    call_hazard = (
        "\tmul.s\t$f20,$f0,$f2\n"
        "\t.set\tnoreorder\n"
        "\tjal\tfunc_8009D4B0\n"
        "\tnop\n"
        "\t.set\tnoreorder\n"
        "\tmul.s\t$f0,$f20,$f0\n"
    )
    call_hazard_retail = call_hazard.replace(
        "\tnop\n\t.set\tnoreorder\n", "\tnop\n\tnop\n\t.set\tnoreorder\n", 1
    )
    fires = text.count(call_hazard)
    if fires != 1:
        raise RuntimeError(
            f"func_800E1BB0 FP call hazard fired {fires} times (expected 1)"
        )
    text = text.replace(call_hazard, call_hazard_retail, 1)

    labeled_load = "\tl.s\t$f0,$LF_lis4\n.L37:\n"
    labeled_load_retail = (
        "\t.set\tnoat\n"
        "\tlui\t$1,%hi($LF_lis4)\n"
        ".L37 = . + 4\n"
        "\tlwc1\t$f0,%lo($LF_lis4)($1)\n"
        "\t.set\tat\n"
    )
    fires = text.count(labeled_load)
    if fires != 1:
        raise RuntimeError(
            f"func_800E1BB0 labeled FP load fired {fires} times (expected 1)"
        )
    return text.replace(labeled_load, labeled_load_retail, 1)


def reproduce_crate_burst_hilo_hazard(text: str) -> str:
    """Restore the two retail HI/LO hazard nops in ``func_800E7768``."""
    if "func_800E7768:" not in text:
        return text
    before = "\tdiv\t$16,$21,$18\n\tmult\t$16,$20\n"
    after = "\tdiv\t$16,$21,$18\n\tnop\n\tnop\n\tmult\t$16,$20\n"
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800E7768 div/mult hazard fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def reproduce_color_interpolate_load_hazard(text: str) -> str:
    """Restore the retail load-use hazard nop in ``func_800F3B80``.

    GCC emits a ``#nop`` placeholder between the stack-argument load and the
    first divide.  The retail assembler materialized it; global noreorder mode
    does not.  Gate the exact sequence to this function and require one fire.
    """
    if "func_800F3B80:" not in text:
        return text
    before = "\tlw\t$8,16($sp)\n\t#nop\n\tdiv\t$2,$2,$8\n"
    after = "\tlw\t$8,16($sp)\n\tnop\n\tdiv\t$2,$2,$8\n"
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800F3B80 load hazard fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def preserve_structure_slot_index_copy(text: str) -> str:
    """Use the retail register copy in ``func_800DE374``.

    The source's byte-narrowed index creates the required independent pseudo,
    but KMC GCC emits a redundant zero extension even though the preceding
    ``lbu`` already established its range.  The retail object uses a plain
    copy at both switch arms.  Require exactly those two branch delay-slot
    patterns so unrelated ``andi`` instructions cannot be changed.
    """
    if "func_800DE374:" not in text:
        return text
    pattern = re.compile(
        r"(\t(?:beq|bne)\t\$3,\$2,\.L\d+\n)"
        r"\tandi\t\$4,\$3,0x00ff\n"
    )
    text, fires = pattern.subn(r"\1\tmove\t$4,$3\n", text)
    if fires != 2:
        raise RuntimeError(
            f"func_800DE374 slot-index copy fired {fires} times (expected 2)"
        )
    return text


def schedule_crate_list_head_store(text: str) -> str:
    """Restore the retail store/load order in ``func_800E66A8``.

    KMC GCC's second scheduler moves the independent slot-byte load and its
    comparison constant above the list-head store.  Reorder only the complete
    three-instruction sequence, gated by the function label and one fire.
    """
    if "func_800E66A8:" not in text:
        return text
    before = (
        "\tlbu\t$3,29($17)\n"
        "\tli\t$2,0x0000007f\t\t# 127\n"
        "\tsw\t$16,12($17)\n"
    )
    after = (
        "\tsw\t$16,12($17)\n"
        "\tlbu\t$3,29($17)\n"
        "\tli\t$2,0x0000007f\t\t# 127\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800E66A8 list-head store reorder fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def swap_script_pair_registers(text: str) -> str:
    """Reproduce the retail allocno choice in ``func_800D12B0``.

    In the handlers shared by opcodes 22, 23, and 33, the reconstructed C
    gives the two-byte accumulator to ``$4`` and the saved opcode to ``$3``.
    The retail object assigns those two independent quantities in the opposite
    order.  Replace the complete contiguous sequence once; the surrounding
    pointer updates, shifts, constant, stores, and branch make this much
    narrower than a general register rename.
    """
    if "func_800D12B0:" not in text:
        return text
    before = (
        "\tlbu\t$2,0($6)\n"
        "\taddu\t$6,$6,1\n"
        "\tlbu\t$4,0($6)\n"
        "\taddu\t$6,$6,1\n"
        "\tlbu\t$3,D_803A6A04\n"
        "\tsll\t$2,$2,8\n"
        "\tor\t$4,$4,$2\n"
        "\tsh\t$4,D_803A666E\n"
        "\tlbu\t$2,0($6)\n"
        "\taddu\t$6,$6,1\n"
        "\tlbu\t$4,0($6)\n"
        "\tsll\t$2,$2,8\n"
        "\tor\t$4,$4,$2\n"
        "\tli\t$2,0x00000017\t\t# 23\n"
        "\tsh\t$4,D_803A6670\n"
        "\t.set\tnoreorder\n"
        "\tbne\t$3,$2,.L2\n"
        "\taddu\t$6,$6,1\n"
    )
    after = (
        "\tlbu\t$2,0($6)\n"
        "\taddu\t$6,$6,1\n"
        "\tlbu\t$3,0($6)\n"
        "\taddu\t$6,$6,1\n"
        "\tlbu\t$4,D_803A6A04\n"
        "\tsll\t$2,$2,8\n"
        "\tor\t$3,$3,$2\n"
        "\tsh\t$3,D_803A666E\n"
        "\tlbu\t$2,0($6)\n"
        "\taddu\t$6,$6,1\n"
        "\tlbu\t$3,0($6)\n"
        "\tsll\t$2,$2,8\n"
        "\tor\t$3,$3,$2\n"
        "\tli\t$2,0x00000017\t\t# 23\n"
        "\tsh\t$3,D_803A6670\n"
        "\t.set\tnoreorder\n"
        "\tbne\t$4,$2,.L2\n"
        "\taddu\t$6,$6,1\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800D12B0 pair-register swap fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def reproduce_projectile_segment_label_hazards(text: str) -> str:
    """Suppress two label-adjacent FP nops in ``func_800DB1B0``.

    KMC ``as`` inserts a load-delay nop when ``.L21`` follows the constant
    load and an FP-result nop when ``.L17`` follows the preceding ``add.s``.
    The retail object has neither.  As with the established turret-sweep
    workaround, define each branch target one instruction early as ``.+4`` so
    the symbol keeps its retail address without exposing a label boundary to
    the assembler's hazard pass.
    """
    if "func_800DB1B0:" not in text:
        return text
    labeled_load = "\tl.s\t$f0,$LF_lis4\n.L21:\n"
    labeled_load_retail = (
        "\t.set\tnoat\n"
        "\tlui\t$1,%hi($LF_lis4)\n"
        ".L21 = . + 4\n"
        "\tlwc1\t$f0,%lo($LF_lis4)($1)\n"
        "\t.set\tat\n"
    )
    fires = text.count(labeled_load)
    if fires != 1:
        raise RuntimeError(
            f"func_800DB1B0 labeled FP load fired {fires} times (expected 1)"
        )
    text = text.replace(labeled_load, labeled_load_retail, 1)

    labeled_add = (
        "\tdiv.s\t$f2,$f2,$f0\n"
        "\tadd.s\t$f4,$f4,$f2\n"
        ".L17:\n"
    )
    labeled_add_retail = (
        "\tdiv.s\t$f2,$f2,$f0\n"
        ".L17 = . + 4\n"
        "\tadd.s\t$f4,$f4,$f2\n"
    )
    fires = text.count(labeled_add)
    if fires != 1:
        raise RuntimeError(
            f"func_800DB1B0 labeled FP add fired {fires} times (expected 1)"
        )
    return text.replace(labeled_add, labeled_add_retail, 1)


def allocate_flag_dispatch_output_to_a3(text: str) -> str:
    """Reproduce the retail fifth-parameter allocation in ``func_800E4DA0``.

    GCC launches the stack-parameter load before the saved-register setup and
    assigns it to ``$8``.  Retail saves the incoming ``$7`` first, loads the
    parameter into the now-dead ``$7``, and uses that register for the five
    output stores.  Require the complete prologue and every exact store form.
    """
    if "func_800E4DA0:" not in text:
        return text
    prologue = (
        "\tsubu\t$sp,$sp,32\n"
        "\tlw\t$8,48($sp)\n"
        "\tsw\t$16,16($sp)\n"
        "\tmove\t$16,$4\n"
        "\tsw\t$31,28($sp)\n"
        "\tsw\t$18,24($sp)\n"
        "\tsw\t$17,20($sp)\n"
        "\tlbu\t$2,10($16)\n"
        "\tmove\t$18,$7\n"
        "\tandi\t$2,$2,0x0002\n"
        "\t.set\tnoreorder\n"
        "\tbne\t$2,$0,.L1\n"
        "\tmove\t$17,$5\n"
    )
    prologue_retail = (
        "\tsubu\t$sp,$sp,32\n"
        "\tsw\t$17,20($sp)\n"
        "\tsw\t$16,16($sp)\n"
        "\tmove\t$16,$4\n"
        "\tsw\t$31,28($sp)\n"
        "\tsw\t$18,24($sp)\n"
        "\tlbu\t$2,10($16)\n"
        "\tmove\t$18,$7\n"
        "\tlw\t$7,48($sp)\n"
        "\tandi\t$2,$2,0x0002\n"
        "\t.set\tnoreorder\n"
        "\tbne\t$2,$0,.L1\n"
        "\tmove\t$17,$5\n"
    )
    fires = text.count(prologue)
    if fires != 1:
        raise RuntimeError(
            f"func_800E4DA0 output prologue fired {fires} times (expected 1)"
        )
    text = text.replace(prologue, prologue_retail, 1)
    stores = (
        ("\tsw\t$2,0($8)\n", "\tsw\t$2,0($7)\n", 2, "word"),
        ("\tsb\t$2,0($8)\n", "\tsb\t$2,0($7)\n", 1, "byte"),
        ("\ts.s\t$f0,4($8)\n", "\ts.s\t$f0,4($7)\n", 1, "x"),
        ("\ts.s\t$f0,8($8)\n", "\ts.s\t$f0,8($7)\n", 1, "y"),
    )
    for before, after, expected, name in stores:
        fires = text.count(before)
        if fires != expected:
            raise RuntimeError(
                f"func_800E4DA0 output {name} store fired {fires} times "
                f"(expected {expected})"
            )
        text = text.replace(before, after)
    return text


def shape_value_decay_clamp(text: str) -> str:
    """Reproduce func_800B6934's retail floating-point clamp tail.

    GCC expresses the two-sided clamp with a likely branch and conditional
    move into ``$f0``.  The retail object instead stores the computed value in
    the branch delay slot and overwrites it with zero when the comparison is
    true.  Gate the complete tail by function label and require one fire.
    """
    if "func_800B6934:" not in text:
        return text

    before = (
        ".L10:\n"
        "\t.set\tnoreorder\n"
        "\tnop\n"
        "\tbc1tl\t.L8\n"
        "\tmov.s\t$f0,$f6\n"
        "\t.set\tnoreorder\n"
        ".L8:\n"
        "\ts.s\t$f0,28($4)\n"
        ".L4:\n"
    )
    after = (
        ".L10:\n"
        "\t.set\tnoreorder\n"
        "\tnop\n"
        "\tbc1f\t.L4\n"
        "\ts.s\t$f0,28($4)\n"
        "\t.set\tnoreorder\n"
        "\ts.s\t$f6,28($4)\n"
        ".L4:\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800B6934 value-clamp rewrite fired {fires} times (expected 1)"
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
    if os.environ.get("V3_OWNER_SEARCH_PROLOGUE", "1") == "1":
        text = schedule_owner_search_prologue(text)
    if os.environ.get("V3_REGION_EARLY_EXIT", "1") == "1":
        text = select_region_early_exit_likely(text)
    if os.environ.get("V3_VECTOR_ANGLE_PROLOGUE", "1") == "1":
        text = schedule_vector_angle_prologue(text)
    if os.environ.get("V3_SELECTION_STATE_DISPATCH", "1") == "1":
        text = shape_selection_state_dispatch(text)
    if os.environ.get("V3_SNAPSHOT_RECORD_ADDRESS", "1") == "1":
        text = shape_snapshot_record_address(text)
    if os.environ.get("V3_LARGE_SEARCH_PROLOGUE", "1") == "1":
        text = schedule_large_search_prologue(text)
    if os.environ.get("V3_REGION_QUERY_CALL", "1") == "1":
        text = schedule_region_query_call(text)
    if os.environ.get("V3_OBJECT_PHASE_LOOKUP", "1") == "1":
        text = normalize_object_phase_lookup(text)
    if os.environ.get("V3_DISPLAY_SLOT_WAIT", "1") == "1":
        text = normalize_display_slot_wait(text)
    if os.environ.get("V3_DISPLAY_RECORD_PREFIX", "1") == "1":
        text = schedule_display_record_prefix(text)
    if os.environ.get("V3_FREE_LIST_REBUCKET", "1") == "1":
        text = schedule_free_list_rebucket(text)
    if os.environ.get("V3_TURRET_SWEEP_HAZARDS", "1") == "1":
        text = reproduce_turret_sweep_assembler_hazards(text)
    if os.environ.get("V3_CRATE_BURST_HAZARD", "1") == "1":
        text = reproduce_crate_burst_hilo_hazard(text)
    if os.environ.get("V3_COLOR_INTERPOLATE_HAZARD", "1") == "1":
        text = reproduce_color_interpolate_load_hazard(text)
    if os.environ.get("V3_STRUCTURE_SLOT_INDEX", "1") == "1":
        text = preserve_structure_slot_index_copy(text)
    if os.environ.get("V3_CRATE_LIST_HEAD_STORE", "1") == "1":
        text = schedule_crate_list_head_store(text)
    if os.environ.get("V3_SCRIPT_PAIR_REGISTERS", "1") == "1":
        text = swap_script_pair_registers(text)
    if os.environ.get("V3_PROJECTILE_SEGMENT_LABELS", "1") == "1":
        text = reproduce_projectile_segment_label_hazards(text)
    if os.environ.get("V3_FLAG_DISPATCH_OUTPUT", "1") == "1":
        text = allocate_flag_dispatch_output_to_a3(text)
    if os.environ.get("V3_VALUE_DECAY_CLAMP", "1") == "1":
        text = shape_value_decay_clamp(text)
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
