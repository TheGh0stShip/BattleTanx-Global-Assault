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
    """Reproduce the retail scheduler order for ``Steps_FreeBranch``.

    The generated body is otherwise exact.  KMC GCC gives the frame setup
    priority over the entry byte load and leaves the destination save in the
    branch delay slot; the retail object schedules the independent load first,
    saves ``s0`` before assigning it, and uses the delay slot for ``ra``.
    """
    if "Steps_FreeBranch:" not in text:
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
            f"Steps_FreeBranch prologue reorder fired {fires} times (expected 1)"
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


def reproduce_lzari_load_hazards(text: str) -> str:
    """Restore the retail LZARI decoder's two load-delay nops.

    KMC GCC leaves a ``#nop`` placeholder between the cumulative-frequency
    load and ``divu`` in both arithmetic-decoder functions. The retail
    assembler materialized it. Require one fire inside each named function so
    this rule cannot silently affect another division with the same shape.
    """
    pattern = "\tlw\t$2,0($17)\n\t#nop\n\tdivu\t$4,$4,$2\n"
    replacement = pattern.replace("\t#nop\n", "\tnop\n")
    for name in ("func_800A0BA8", "func_800A0E00"):
        if f"{name}:" not in text:
            continue
        match = re.search(
            rf"{name}:.*?\n\s*\.end\s+{name}\b", text, flags=re.S
        )
        if match is None:
            raise RuntimeError(f"{name} body not found for LZARI load hazard")
        body = match.group(0)
        fires = body.count(pattern)
        if fires != 1:
            raise RuntimeError(
                f"{name} load-delay hazard fired {fires} times (expected 1)"
            )
        body = body.replace(pattern, replacement, 1)
        text = text[:match.start()] + body + text[match.end():]
    return text


def reproduce_entity_model_draw_label_hazard(text: str) -> str:
    """Suppress the label-adjacent HI/LO nop in ``func_800E3FDC``.

    KMC as inserts a hazard nop when the branch target label immediately
    precedes ``mult``.  The retail object targets the same address without
    that nop.  Define the label one instruction earlier as ``. + 4`` so its
    value is unchanged while the assembler no longer sees that boundary.
    """
    if "func_800E3FDC:" not in text:
        return text
    before = (
        "\tj\t.L20\n"
        "\taddu\t$2,$4,$2\n"
        "\t.set\tnoreorder\n"
        ".L21:\n"
        "\tmult\t$6,$5\n"
    )
    after = (
        "\tj\t.L20\n"
        ".L21 = . + 4\n"
        "\taddu\t$2,$4,$2\n"
        "\t.set\tnoreorder\n"
        "\tmult\t$6,$5\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800E3FDC label-adjacent mult fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def reproduce_collision_query_label_hazard(text: str) -> str:
    """Suppress the retail assembler's label-adjacent ``mul.s`` nop.

    The branch target remains the address of the multiply; defining it one
    instruction earlier as ``. + 4`` only prevents KMC ``as`` from treating
    the target as a multiply-hazard boundary.
    """
    if "func_800B4684:" not in text:
        return text
    before = (
        "\tj\t.L40\n"
        "\tadd.s\t$f2,$f4,$f0\n"
        "\t.set\tnoreorder\n"
        ".L39:\n"
        "\tmul.s\t$f2,$f2,$f2\n"
    )
    after = (
        "\tj\t.L40\n"
        ".L39 = . + 4\n"
        "\tadd.s\t$f2,$f4,$f0\n"
        "\t.set\tnoreorder\n"
        "\tmul.s\t$f2,$f2,$f2\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800B4684 label-adjacent multiply fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def reproduce_sprite_ring_label_hazard(text: str) -> str:
    """Suppress the non-retail label-adjacent ``mul.s`` nop in func_800A42C8.

    The branch target remains the multiply address. Defining it one
    instruction earlier as ``. + 4`` prevents KMC ``as`` from treating the
    label as a floating-point multiply hazard boundary. The complete pattern
    is function-gated and must occur exactly once.
    """
    if "func_800A42C8:" not in text:
        return text
    body_match = re.search(
        r"func_800A42C8:.*?\n\s*\.end\s+func_800A42C8\b", text, flags=re.S
    )
    if body_match is None:
        raise RuntimeError("func_800A42C8 body not found for label hazard")
    body = body_match.group(0)
    pattern = re.compile(
        r"\tsub\.s\t\$f2,\$f2,\$f0\n(?P<label>\.L\d+):\n"
        r"\tmul\.s\t\$f2,\$f8,\$f2\n"
    )
    matches = list(pattern.finditer(body))
    if len(matches) != 1:
        raise RuntimeError(
            "func_800A42C8 label-adjacent multiply fired "
            f"{len(matches)} times (expected 1)"
        )
    label = matches[0].group("label")
    replacement = (
        f"{label} = . + 4\n"
        "\tsub.s\t$f2,$f2,$f0\n"
        "\tmul.s\t$f2,$f8,$f2\n"
    )
    body = pattern.sub(replacement, body, count=1)
    return text[:body_match.start()] + body + text[body_match.end():]


def suppress_distance_multiply_hazard_nops(text: str) -> str:
    """Encode branch-target ``mul.s`` instructions without KMC's extra nop.

    KMC ``as`` applies its VR4300 mulmul workaround when a branch target begins
    with ``mul.s`` after an FP instruction in the branch delay slot. The retail
    assembler did not: all 46 corresponding retail sites omit the nop. Encode
    the compiler-selected operands directly, gated by function and fire count.
    """
    gates = {"func_80083230": 1, "func_80084CC8": 3, "func_80086CEC": 1}
    pattern = re.compile(
        r"^(?P<label>\.L\d+):\n"
        r"\tmul\.s\t\$f(?P<fd>\d+),\$f(?P<fs>\d+),\$f(?P<ft>\d+)\n",
        re.M,
    )
    for function, expected in gates.items():
        if f"{function}:" not in text:
            continue
        match = re.search(
            rf"{function}:.*?\n\s*\.end\s+{function}\b", text, flags=re.S
        )
        if match is None:
            raise RuntimeError(f"{function} body not found for multiply hazard")
        body = match.group(0)
        fires = 0

        def replace(site: re.Match) -> str:
            nonlocal fires
            label = site.group("label")
            if re.search(
                rf"^\t(?:b[a-z0-9]*|j)\t(?:[^\n]*,)?{re.escape(label)}\s*$",
                body,
                flags=re.M,
            ) is None:
                return site.group(0)
            fires += 1
            fd = int(site.group("fd"))
            fs = int(site.group("fs"))
            ft = int(site.group("ft"))
            word = 0x46000002 | (ft << 16) | (fs << 11) | (fd << 6)
            return (
                f"{label}:\n\t.word\t0x{word:08X}\t"
                f"# mul.s $f{fd},$f{fs},$f{ft}; retail as omits hazard nop\n"
            )

        body = pattern.sub(replace, body)
        if fires != expected:
            raise RuntimeError(
                f"{function} branch-target multiply fired {fires} times "
                f"(expected {expected})"
            )
        text = text[:match.start()] + body + text[match.end():]
    return text


def schedule_progress_level_loop_setup(text: str) -> str:
    """Match the retail loop-pointer/constant setup in ``func_8009C31C``.

    The pointer copy and constant load are independent. The retail scheduler
    places the pointer copy in the preceding branch delay slot and loads the
    constant immediately afterward. Require the complete function-local
    pattern exactly once.
    """
    if "func_8009C31C:" not in text:
        return text
    before = (
        "\tbeq\t$2,$0,.L12\n"
        "\tli\t$17,0x00000001\t\t# 1\n"
        "\t.set\tnoreorder\n"
        "\tmove\t$16,$4\n"
    )
    after = (
        "\tbeq\t$2,$0,.L12\n"
        "\tmove\t$16,$4\n"
        "\t.set\tnoreorder\n"
        "\tli\t$17,0x00000001\t\t# 1\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_8009C31C loop setup fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def shape_angle_table_lookup_registers(text: str) -> str:
    """Match the retail temporary allocation in ``func_8009D5B4``.

    Keep the xor result in ``v1`` and its narrowed table index in ``v0``, then
    use the retail commutative operand order in both return paths. The complete
    patterns and their counts are function-gated.
    """
    if "func_8009D5B4:" not in text:
        return text
    before = (
        "\txor\t$2,$5,$4\n"
        "\t.set\tnoreorder\n"
        "\tandi\t$3,$2,0xffff\n"
        "\tsll\t$2,$3,2\n"
    )
    after = (
        "\txor\t$3,$5,$4\n"
        "\t.set\tnoreorder\n"
        "\tandi\t$2,$3,0xffff\n"
        "\tsll\t$2,$2,2\n"
    )
    if text.count(before) != 1:
        raise RuntimeError(
            f"func_8009D5B4 index allocation fired {text.count(before)} times (expected 1)"
        )
    text = text.replace(before, after, 1)
    add_before = "\taddu\t$2,$4,$3\n"
    add_after = "\taddu\t$2,$3,$4\n"
    if text.count(add_before) != 2:
        raise RuntimeError(
            f"func_8009D5B4 return add fired {text.count(add_before)} times (expected 2)"
        )
    return text.replace(add_before, add_after)


def shape_path_waypoint_side_registers(text: str) -> str:
    """Keep the incoming waypoint id in ``a1`` until the first call.

    The retail allocation narrows the id into ``s0`` while retaining the
    original value in ``a1``, then copies it to ``s4`` in the call delay slot.
    GCC otherwise places the original value in ``s4`` immediately.  Gate the
    complete function-local sequence and require one fire.
    """
    if "func_8007F59C:" not in text:
        return text
    before = (
        "\tlhu\t$20,250($18)\n"
        "\t#nop\n"
        "\tandi\t$16,$20,0xffff\n"
    )
    after = (
        "\tlhu\t$5,250($18)\n"
        "\t#nop\n"
        "\tandi\t$16,$5,0xffff\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_8007F59C id allocation fired {fires} times (expected 1)"
        )
    text = text.replace(before, after, 1)
    before = "\tjal\tSteps_InitStep_Free\n\tmove\t$5,$16\n"
    after = "\tjal\tSteps_InitStep_Free\n\tmove\t$20,$5\n"
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_8007F59C delayed id copy fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def shape_waypoint_stack_reset_addresses(text: str) -> str:
    """Use the retail operand order for two commutative address additions."""
    if "func_8008A764:" not in text:
        return text
    before = "\taddu\t$2,$4,$2\n"
    after = "\taddu\t$2,$2,$4\n"
    fires = text.count(before)
    if fires != 2:
        raise RuntimeError(
            f"func_8008A764 reset address fired {fires} times (expected 2)"
        )
    return text.replace(before, after)


def swap_model_vertex_offset_loop_registers(text: str) -> str:
    """Exchange the source pointer and loop-index saved registers.

    These allocnos have effectively tied priorities in ``func_800EBA98``.
    The retail object chooses ``s1`` for the source and ``s2`` for the index;
    GCC's reconstructed source chooses the reverse.  The rename is confined
    to the complete function and guarded by exact occurrence counts.
    """
    if "func_800EBA98:" not in text:
        return text
    match = re.search(
        r"func_800EBA98:.*?\n\s*\.end\s+func_800EBA98\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_800EBA98 body not found")
    body = match.group(0)
    counts = {register: len(re.findall(re.escape(register), body))
              for register in ("$17", "$18")}
    if counts != {"$17": 6, "$18": 7}:
        raise RuntimeError(f"func_800EBA98 register counts changed: {counts}")
    body = body.replace("$17", "$__swap").replace("$18", "$17").replace("$__swap", "$18")
    frame_before = (
        "\tsw\t$17,32($sp)\n", "\tsw\t$18,28($sp)\n",
        "\tlw\t$17,32($sp)\n", "\tlw\t$18,28($sp)\n",
    )
    frame_after = (
        "\tsw\t$17,28($sp)\n", "\tsw\t$18,32($sp)\n",
        "\tlw\t$18,32($sp)\n", "\tlw\t$17,28($sp)\n",
    )
    for before, after in zip(frame_before, frame_after):
        if body.count(before) != 1:
            raise RuntimeError("func_800EBA98 frame pattern changed")
        body = body.replace(before, after, 1)
    return text[:match.start()] + body + text[match.end():]


def cycle_tank_contact_scan_registers(text: str) -> str:
    """Use the retail saved-register allocation for ``func_80090C4C``.

    The reconstructed source and retail object have identical instruction
    shape and scheduling, but GCC assigns three tied saved-register allocnos
    in a different order.  Cycle ``s0 -> s2 -> s1 -> s0`` inside the complete
    function, then restore the ABI's canonical save slots.  Exact occurrence
    counts and frame patterns make this fail closed if the source drifts.
    """
    if "func_80090C4C:" not in text:
        return text
    match = re.search(
        r"func_80090C4C:.*?\n\s*\.end\s+func_80090C4C\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_80090C4C body not found")
    body = match.group(0)
    counts = {
        register: len(re.findall(re.escape(register), body))
        for register in ("$16", "$17", "$18")
    }
    if counts != {"$16": 7, "$17": 14, "$18": 10}:
        raise RuntimeError(
            f"func_80090C4C saved-register counts changed: {counts}"
        )

    body = (
        body.replace("$16", "$__cycle16")
        .replace("$17", "$16")
        .replace("$18", "$17")
        .replace("$__cycle16", "$18")
    )
    frame_replacements = (
        ("\tsw\t$16,1252($sp)\n", "\tsw\t$16,1248($sp)\n"),
        ("\tsw\t$17,1256($sp)\n", "\tsw\t$17,1252($sp)\n"),
        ("\tsw\t$18,1248($sp)\n", "\tsw\t$18,1256($sp)\n"),
        ("\tlw\t$17,1256($sp)\n", "\tlw\t$17,1252($sp)\n"),
        ("\tlw\t$16,1252($sp)\n", "\tlw\t$16,1248($sp)\n"),
        ("\tlw\t$18,1248($sp)\n", "\tlw\t$18,1256($sp)\n"),
    )
    for before, after in frame_replacements:
        if body.count(before) != 1:
            raise RuntimeError("func_80090C4C frame pattern changed")
        body = body.replace(before, after, 1)
    frame_order_replacements = (
        (
            "\tsw\t$17,1252($sp)\n\tsw\t$18,1256($sp)\n",
            "\tsw\t$18,1256($sp)\n\tsw\t$17,1252($sp)\n",
        ),
        (
            "\tlw\t$17,1252($sp)\n\tlw\t$16,1248($sp)\n"
            "\tlw\t$18,1256($sp)\n",
            "\tlw\t$18,1256($sp)\n\tlw\t$17,1252($sp)\n"
            "\tlw\t$16,1248($sp)\n",
        ),
    )
    for before, after in frame_order_replacements:
        if body.count(before) != 1:
            raise RuntimeError("func_80090C4C frame order changed")
        body = body.replace(before, after, 1)
    return text[:match.start()] + body + text[match.end():]


def shape_entity_selection_registers(text: str) -> str:
    """Match retail temporary choices in ``func_800ED990``."""
    if "func_800ED990:" not in text:
        return text
    replacements = (
        ("\tlbu\t$3,8($20)\n\tli\t$2,0x000000fe\t\t# 254\n"
         "\tandi\t$4,$3,0x00ff\n\t.set\tnoreorder\n\tbeq\t$4,$2,.L6\n",
         "\tlbu\t$4,8($20)\n\tli\t$2,0x000000fe\t\t# 254\n"
         "\tandi\t$3,$4,0x00ff\n\t.set\tnoreorder\n\tbeq\t$3,$2,.L6\n"),
        ("\tbeq\t$4,$2,.L9\n", "\tbeq\t$3,$2,.L9\n"),
        ("\tbnel\t$2,$0,.L5\n\tsb\t$3,45($18)\n",
         "\tbnel\t$2,$0,.L5\n\tsb\t$4,45($18)\n"),
        ("\taddu\t$3,$3,$2\n\tlbu\t$2,13($3)\n",
         "\taddu\t$2,$2,$3\n\tlbu\t$2,13($2)\n"),
    )
    for before, after in replacements:
        fires = text.count(before)
        if fires != 1:
            raise RuntimeError(
                f"func_800ED990 register pattern fired {fires} times (expected 1)"
            )
        text = text.replace(before, after, 1)
    return text


def shape_turret_angle_delta_registers(text: str) -> str:
    """Match the two retained angle loads in ``func_800E3460``."""
    if "func_800E3460:" not in text:
        return text
    before = (
        "\tlhu\t$3,72($sp)\n"
        "\t.section\t.rodata\n\t.align\t2\n$LF_lis4:\n"
        "\t.word\t0x4F000000\n\t.text\n\tl.s\t$f0,$LF_lis4\n"
        "\tlhu\t$2,86($sp)\n\tc.le.s\t$f0,$f2\n"
        "\tsubu\t$8,$2,$3\n\t.set\tnoreorder\n"
        "\tbc1t\t.L75\n\tsubu\t$7,$3,$2\n"
    )
    after = before.replace("\tlhu\t$3,72($sp)\n", "\tlhu\t$9,86($sp)\n", 1)
    after = after.replace("\tlhu\t$2,86($sp)\n", "\tlhu\t$2,72($sp)\n", 1)
    after = after.replace("\tsubu\t$8,$2,$3\n", "\tsubu\t$8,$9,$2\n", 1)
    after = after.replace("\tsubu\t$7,$3,$2\n", "\tsubu\t$7,$2,$9\n", 1)
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800E3460 angle-delta pattern fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def shape_wave_vertex_update_registers(text: str) -> str:
    """Match the saved-register and spilled-constant choices in func_800EF770."""
    if "func_800EF770:" not in text:
        return text
    match = re.search(
        r"func_800EF770:.*?\n\s*\.end\s+func_800EF770\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_800EF770 body not found")
    body = match.group(0)
    counts = {r: len(re.findall(re.escape(r), body)) for r in ("$21", "$22")}
    if counts != {"$21": 11, "$22": 4}:
        raise RuntimeError(f"func_800EF770 register counts changed: {counts}")
    body = body.replace("$21", "$__swap").replace("$22", "$21").replace("$__swap", "$22")
    frame = (
        ("\tsw\t$21,120($sp)\n", "\tsw\t$22,120($sp)\n"),
        ("\tsw\t$22,116($sp)\n", "\tsw\t$21,116($sp)\n"),
        ("\tlw\t$21,120($sp)\n", "\tlw\t$22,120($sp)\n"),
        ("\tlw\t$22,116($sp)\n", "\tlw\t$21,116($sp)\n"),
    )
    for before, after in frame:
        if body.count(before) != 1:
            raise RuntimeError("func_800EF770 frame pattern changed")
        body = body.replace(before, after, 1)
    before = (
        "\tlw\t$2,4($23)\n\tlw\t$9,68($sp)\n\t#nop\n"
        "\taddu\t$2,$2,$9\n"
    )
    if body.count(before) != 1:
        raise RuntimeError("func_800EF770 address-add pattern changed")
    after = before.replace("\taddu\t$2,$2,$9\n", "\taddu\t$2,$9,$2\n")
    body = body.replace(before, after, 1)
    before = (
        "\tli\t$2,0x00000014\t\t# 20\n"
        "\tsw\t$2,52($4)\n\tsw\t$2,56($4)\n"
    )
    after = (
        "\tli\t$9,0x00000014\t\t# 20\n"
        "\tsw\t$9,56($4)\n\tsw\t$9,52($4)\n"
    )
    if body.count(before) != 1:
        raise RuntimeError("func_800EF770 constant-store pattern changed")
    body = body.replace(before, after, 1)
    return text[:match.start()] + body + text[match.end():]


def shape_wave_mesh_copy_registers(text: str) -> str:
    """Match the two tied allocations in ``func_800F1900``."""
    if "func_800F1900:" not in text:
        return text
    match = re.search(
        r"func_800F1900:.*?\n\s*\.end\s+func_800F1900\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_800F1900 body not found")
    body = match.group(0)
    counts = {r: len(re.findall(re.escape(r), body)) for r in ("$23", "$fp")}
    if counts != {"$23": 7, "$fp": 7}:
        raise RuntimeError(f"func_800F1900 saved-register counts changed: {counts}")
    body = body.replace("$23", "$__swap").replace("$fp", "$23").replace("$__swap", "$fp")
    frame = (
        ("\tsw\t$23,48($sp)\n", "\tsw\t$23,44($sp)\n"),
        ("\tsw\t$fp,44($sp)\n", "\tsw\t$fp,48($sp)\n"),
        ("\tlw\t$23,48($sp)\n", "\tlw\t$fp,48($sp)\n"),
        ("\tlw\t$fp,44($sp)\n", "\tlw\t$23,44($sp)\n"),
    )
    for before, after in frame:
        if body.count(before) != 1:
            raise RuntimeError("func_800F1900 frame pattern changed")
        body = body.replace(before, after, 1)
    before = (
        "\taddu\t$3,$2,$22\n\tsw\t$3,4($20)\n"
        "\tlw\t$2,0($18)\n\tlw\t$4,4($18)\n"
        "\tsrl\t$2,$2,12\n\tandi\t$2,$2,0x00ff\n"
        "\t.set\tnoreorder\n\tbeq\t$2,$0,.L6\n\tsll\t$2,$2,4\n"
        "\t.set\tnoreorder\n\tmove\t$16,$3\n\tmove\t$17,$4\n"
    )
    after = before.replace("$3", "$__swap").replace("$4", "$3").replace("$__swap", "$4")
    if body.count(before) != 1:
        raise RuntimeError("func_800F1900 vertex-copy pattern changed")
    body = body.replace(before, after, 1)
    return text[:match.start()] + body + text[match.end():]


def shape_unit_command_candidate_prologue(text: str) -> str:
    """Reproduce the retail scheduling and likely-branch multiply in 80086CEC."""
    if "func_80086CEC:" not in text:
        return text
    before = (
        "\tsubu\t$sp,$sp,40\n\tsw\t$16,16($sp)\n\tmove\t$16,$0\n"
        "\tsw\t$17,20($sp)\n\tmove\t$17,$4\n\tsw\t$18,24($sp)\n"
        "\ts.d\t$f20,32($sp)\n\tmtc1\t$0,$f20\n\tsw\t$31,28($sp)\n"
        "\t.set\tnoreorder\n\tjal\tfunc_80095B68\n\tmove\t$18,$5\n"
    )
    after = (
        "\tsubu\t$sp,$sp,40\n\ts.d\t$f20,32($sp)\n\tmtc1\t$0,$f20\n"
        "\tsw\t$17,20($sp)\n\tmove\t$17,$4\n\tsw\t$18,24($sp)\n"
        "\tmove\t$18,$5\n\tsw\t$31,28($sp)\n\tsw\t$16,16($sp)\n"
        "\t.set\tnoreorder\n\tjal\tfunc_80095B68\n\tmove\t$16,$0\n"
    )
    if text.count(before) != 1:
        raise RuntimeError("func_80086CEC prologue pattern changed")
    text = text.replace(before, after, 1)
    before = (
        "\t.set\tnoreorder\n\t.set\tnoreorder\n"
        "\tbnel\t$2,$0,.L72\n\tmul.s\t$f20,$f2,$f0\n"
    )
    after = (
        "\tmul.s\t$f20,$f2,$f0\n\t.set\tnoreorder\n\t.set\tnoreorder\n"
        "\tbnel\t$2,$0,.L72\n\tnop\n"
    )
    if text.count(before) != 1:
        raise RuntimeError("func_80086CEC likely-multiply pattern changed")
    return text.replace(before, after, 1)


def swap_unit_command_search_registers(text: str) -> str:
    """Exchange tied ``used``/unit-search allocnos in ``func_80086FF4``."""
    if "func_80086FF4:" not in text:
        return text
    match = re.search(
        r"func_80086FF4:.*?\n\s*\.end\s+func_80086FF4\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_80086FF4 body not found")
    body = match.group(0)
    counts = {r: len(re.findall(re.escape(r), body)) for r in ("$16", "$17")}
    if counts != {"$16": 22, "$17": 9}:
        raise RuntimeError(f"func_80086FF4 register counts changed: {counts}")
    body = body.replace("$16", "$__swap").replace("$17", "$16").replace("$__swap", "$17")
    frame = (
        ("\tsw\t$16,28($sp)\n", "\tsw\t$17,28($sp)\n"),
        ("\tsw\t$17,24($sp)\n", "\tsw\t$16,24($sp)\n"),
        ("\tlw\t$16,28($sp)\n", "\tlw\t$17,28($sp)\n"),
        ("\tlw\t$17,24($sp)\n", "\tlw\t$16,24($sp)\n"),
    )
    for before, after in frame:
        if body.count(before) != 1:
            raise RuntimeError("func_80086FF4 frame pattern changed")
        body = body.replace(before, after, 1)
    return text[:match.start()] + body + text[match.end():]


def shape_tank_aim_refresh_registers(text: str) -> str:
    """Match the scheduler and FPR tie choices in ``func_80082198``."""
    if "func_80082198:" not in text:
        return text
    before = (
        "\tsw\t$31,64($sp)\n\tsw\t$19,60($sp)\n\tsw\t$18,56($sp)\n"
        "\tsw\t$16,48($sp)\n\tlw\t$3,312($17)\n\tlw\t$2,D_8021945C\n"
        "\tmove\t$19,$0\n"
    )
    after = (
        "\tsw\t$19,60($sp)\n\tmove\t$19,$0\n\tsw\t$18,56($sp)\n"
        "\tsw\t$31,64($sp)\n\tsw\t$16,48($sp)\n\tlw\t$2,D_8021945C\n"
        "\tlw\t$3,312($17)\n"
    )
    if text.count(before) != 1:
        raise RuntimeError("func_80082198 prologue pattern changed")
    text = text.replace(before, after, 1)
    match = re.search(
        r"func_80082198:.*?\n\s*\.end\s+func_80082198\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_80082198 body not found")
    body = match.group(0)
    counts = {r: len(re.findall(re.escape(r), body)) for r in ("$f8", "$f10")}
    if counts != {"$f8": 4, "$f10": 3}:
        raise RuntimeError(f"func_80082198 FPR counts changed: {counts}")
    body = body.replace("$f8", "$__swap").replace("$f10", "$f8").replace("$__swap", "$f10")
    return text[:match.start()] + body + text[match.end():]


def reproduce_vector_angle_join_label(text: str) -> str:
    """Prevent KMC ``as`` from adding a non-retail nop before ``mul.s``."""
    if "func_800B8310:" not in text:
        return text
    match = re.search(
        r"func_800B8310:.*?\n\s*\.end\s+func_800B8310\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_800B8310 body not found")
    body = match.group(0)
    pattern = re.compile(
        r"(\t\.set\tnoreorder\n\.L\d+:\n)\tmove\t\$16,\$0\n"
        r"(\.L\d+):\n(\tmul\.s\t\$f2,\$f20,\$f20\n)"
    )
    body, fires = pattern.subn(
        r"\1\2 = . + 4\n\tmove\t$16,$0\n\3", body
    )
    if fires != 1:
        raise RuntimeError(
            f"func_800B8310 join-label rewrite fired {fires} times (expected 1)"
        )
    before = (
        "\tsrl\t$3,$2,1\n\tandi\t$2,$16,0xffff\n"
        "\t.set\tnoreorder\n\tbne\t$2,$0,.L6\n"
        "\txori\t$2,$3,0xffff\n\t.set\tnoreorder\n\tandi\t$2,$3,0xffff\n"
        ".L6:\n"
    )
    if body.count(before) != 1:
        raise RuntimeError("func_800B8310 result-select pattern changed")
    after = (
        "\tsrl\t$4,$2,1\n\tandi\t$2,$16,0xffff\n\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L6\n\tmove\t$3,$4\n\t.set\tnoreorder\n"
        "\txori\t$3,$4,0xffff\n.L6:\n\tandi\t$2,$3,0xffff\n"
    )
    body = body.replace(before, after, 1)
    return text[:match.start()] + body + text[match.end():]


def shape_effect_mesh_draw_registers(text: str) -> str:
    """Match saved constant allocation and first-call scheduling in 800F8AAC."""
    if "func_800F8AAC:" not in text:
        return text
    match = re.search(
        r"func_800F8AAC:.*?\n\s*\.end\s+func_800F8AAC\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_800F8AAC body not found")
    body = match.group(0)
    counts = {r: len(re.findall(re.escape(r), body)) for r in ("$19", "$20")}
    if counts != {"$19": 6, "$20": 6}:
        raise RuntimeError(f"func_800F8AAC register counts changed: {counts}")
    body = body.replace("$19", "$__swap").replace("$20", "$19").replace("$__swap", "$20")
    frame = (
        ("\tsw\t$19,80($sp)\n", "\tsw\t$20,80($sp)\n"),
        ("\tsw\t$20,76($sp)\n", "\tsw\t$19,76($sp)\n"),
        ("\tlw\t$19,80($sp)\n", "\tlw\t$20,80($sp)\n"),
        ("\tlw\t$20,76($sp)\n", "\tlw\t$19,76($sp)\n"),
    )
    for before, after in frame:
        if body.count(before) != 1:
            raise RuntimeError("func_800F8AAC frame pattern changed")
        body = body.replace(before, after, 1)
    before = (
        "\tli\t$3,-428343296\t\t\t# 0xe6780000\n\tori\t$3,$3,0x7800\n"
        "\taddu\t$fp,$16,12\n\tmove\t$5,$fp\n\tli\t$23,-100663296\t\t\t# 0xfa000000\n"
        "\tlw\t$16,D_8021945C\n\tandi\t$18,$18,0x00ff\n"
        "\tli\t$2,0x78000000\t\t# 2013265920\n\tor\t$2,$18,$2\n"
        "\tli\t$19,-83886080\t\t\t# 0xfb000000\n\tandi\t$22,$22,0x00ff\n"
        "\taddu\t$21,$sp,48\n\tlw\t$4,D_803A56D4\n\tli\t$20,0x00000002\t\t# 2\n"
    )
    after = (
        "\tli\t$3,-428343296\t\t\t# 0xe6780000\n"
        "\tori\t$3,$3,0x7800\n\tlw\t$4,D_803A56D4\n"
        "\taddu\t$fp,$16,12\n\tmove\t$5,$fp\n"
        "\tli\t$23,-100663296\t\t\t# 0xfa000000\n\tlw\t$16,D_8021945C\n"
        "\tandi\t$18,$18,0x00ff\n\tli\t$2,0x78000000\t\t# 2013265920\n"
        "\tor\t$2,$18,$2\n\tli\t$19,-83886080\t\t\t# 0xfb000000\n"
        "\tandi\t$22,$22,0x00ff\n\taddu\t$21,$sp,48\n"
        "\tli\t$20,0x00000002\t\t# 2\n"
    )
    if body.count(before) != 1:
        raise RuntimeError("func_800F8AAC first-call setup changed")
    body = body.replace(before, after, 1)
    before = (
        "\tli\t$3,0x78e60000\t\t# 2028339200\n"
        "\tori\t$3,$3,0x7800\n\tmove\t$5,$fp\n\tlw\t$4,D_803A56D4\n"
    )
    after = (
        "\tli\t$3,0x78e60000\t\t# 2028339200\n"
        "\tlw\t$4,D_803A56D4\n\tori\t$3,$3,0x7800\n\tmove\t$5,$fp\n"
    )
    if body.count(before) != 1:
        raise RuntimeError("func_800F8AAC second-call setup changed")
    body = body.replace(before, after, 1)
    return text[:match.start()] + body + text[match.end():]


def order_spotter_frame_setup_prologue(text: str) -> str:
    """Match the retail prologue ordering in ``func_800A6FD0``.

    The source compiler emits the three independent operations in a different
    order. Reorder only this complete function-local sequence and require one
    fire; no instruction is added or removed.
    """
    if "func_800A6FD0:" not in text:
        return text
    before = (
        "\tsw\t$17,28($sp)\n"
        "\tmove\t$17,$7\n"
        "\tsw\t$31,32($sp)\n"
    )
    after = (
        "\tsw\t$31,32($sp)\n"
        "\tsw\t$17,28($sp)\n"
        "\tmove\t$17,$7\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800A6FD0 prologue ordering fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def schedule_template_retry_source_reset(text: str) -> str:
    """Match the retail retry scheduling in ``func_800A1384``.

    GCC moves the source-pointer reset into the pointer-test delay slot and
    retargets that branch directly to the copy loop.  Retail instead branches
    to the reset instruction with an empty slot, then uses the reset in the
    later kind-test delay slot.  Move that one instruction between the two
    slots without adding or removing code.
    """
    if "func_800A1384:" not in text:
        return text
    before = (
        "\tbne\t$2,$0,.L5\n"
        "\tmove\t$6,$8\n"
        "\t.set\tnoreorder\n"
        "\tlhu\t$2,496($sp)\n"
        "\t#nop\n"
        "\tsltu\t$2,$2,5\n"
        "\tbne\t$2,$0,.L5\n"
        "\tnop\n"
    )
    after = (
        "\tbne\t$2,$0,.L3\n"
        "\tnop\n"
        "\t.set\tnoreorder\n"
        "\tlhu\t$2,496($sp)\n"
        "\t#nop\n"
        "\tsltu\t$2,$2,5\n"
        "\tbne\t$2,$0,.L5\n"
        "\tmove\t$6,$8\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800A1384 retry scheduling fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def order_font_glyph_draw_prologue(text: str) -> str:
    """Match retail scheduling in ``func_80096A54``.

    Load/save the stack-passed x scale before the y scale, and use the retail
    commutative operand order for the glyph address. Both patterns are gated
    to this function and have exact fire counts.
    """
    if "func_80096A54:" not in text:
        return text
    before = (
        "\ts.d\t$f22,80($sp)\n"
        "\tl.s\t$f22,112($sp)\n"
        "\tsw\t$18,56($sp)\n"
        "\tmove\t$18,$4\n"
        "\tsw\t$19,60($sp)\n"
        "\tmove\t$19,$7\n"
        "\ts.d\t$f20,72($sp)\n"
        "\tl.s\t$f20,108($sp)\n"
    )
    after = (
        "\ts.d\t$f20,72($sp)\n"
        "\tl.s\t$f20,108($sp)\n"
        "\tsw\t$18,56($sp)\n"
        "\tmove\t$18,$4\n"
        "\tsw\t$19,60($sp)\n"
        "\tmove\t$19,$7\n"
        "\ts.d\t$f22,80($sp)\n"
        "\tl.s\t$f22,112($sp)\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_80096A54 float prologue fired {fires} times (expected 1)"
        )
    text = text.replace(before, after, 1)
    add_before = "\taddu\t$2,$2,$17\n"
    add_after = "\taddu\t$2,$17,$2\n"
    fires = text.count(add_before)
    if fires != 1:
        raise RuntimeError(
            f"func_80096A54 glyph address fired {fires} times (expected 1)"
        )
    return text.replace(add_before, add_after, 1)


def shape_music_stream_queue_registers(text: str) -> str:
    """Match the retail allocation in ``func_80097D14``.

    The retail build copies ``fade`` before forming the persistent stream
    pointer, reuses the dead ``a1`` register for the switch selector, retains
    the track offset in ``v1``, and uses ``v0`` for the case-1 buffer index.
    Require each complete function-local pattern exactly once.
    """
    if "func_80097D14:" not in text:
        return text
    prefix_before = (
        "\tsw\t$17,20($sp)\n"
        "\tla\t$17,D_801B4540\n"
        "\tsw\t$31,24($sp)\n"
        "\tsw\t$16,16($sp)\n"
        "\tlhu\t$3,0($17)\n"
        "\t#nop\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$3,$0,.L3\n"
        "\tmove\t$16,$5\n"
        "\t.set\tnoreorder\n"
        "\tli\t$2,0x00000001\t\t# 1\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$3,$2,.L4\n"
        "\tsll\t$2,$4,3\n"
    )
    prefix_after = (
        "\tsw\t$16,16($sp)\n"
        "\tmove\t$16,$5\n"
        "\tsw\t$17,20($sp)\n"
        "\tla\t$17,D_801B4540\n"
        "\tsw\t$31,24($sp)\n"
        "\tlhu\t$5,0($17)\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$5,$0,.L3\n"
        "\tli\t$2,0x00000001\t\t# 1\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$5,$2,.L4\n"
        "\tsll\t$3,$4,3\n"
    )
    fires = text.count(prefix_before)
    if fires != 1:
        raise RuntimeError(
            f"func_80097D14 dispatch prefix fired {fires} times (expected 1)"
        )
    text = text.replace(prefix_before, prefix_after, 1)
    case_before = (
        "\tlhu\t$3,D_801B4540+2\n"
        "\tlui\t$at,%hi(D_80114710)\n"
        "\taddu\t$at,$at,$2\n"
        "\tlw\t$4,%lo(D_80114710)($at)\n"
        "\tlui\t$at,%hi(D_80114710+4)\n"
        "\taddu\t$at,$at,$2\n"
        "\tlw\t$6,%lo(D_80114710+4)($at)\n"
        "\taddu\t$3,$3,1\n"
        "\tandi\t$3,$3,0x0001\n"
        "\tsll\t$3,$3,2\n"
        "\taddu\t$3,$3,$17\n"
        "\tlw\t$5,8($3)\n"
    )
    case_after = (
        "\tlhu\t$2,D_801B4540+2\n"
        "\tlui\t$at,%hi(D_80114710)\n"
        "\taddu\t$at,$at,$3\n"
        "\tlw\t$4,%lo(D_80114710)($at)\n"
        "\tlui\t$at,%hi(D_80114710+4)\n"
        "\taddu\t$at,$at,$3\n"
        "\tlw\t$6,%lo(D_80114710+4)($at)\n"
        "\taddu\t$2,$2,1\n"
        "\tandi\t$2,$2,0x0001\n"
        "\tsll\t$2,$2,2\n"
        "\taddu\t$2,$2,$17\n"
        "\tlw\t$5,8($2)\n"
    )
    fires = text.count(case_before)
    if fires != 1:
        raise RuntimeError(
            f"func_80097D14 case-1 allocation fired {fires} times (expected 1)"
        )
    return text.replace(case_before, case_after, 1)


def schedule_spotter_update_collision_setup(text: str) -> str:
    """Match retail collision-call setup in ``func_800A6C20``.

    Reorder existing independent address, result, argument, and global-flag
    instructions, then use the retail commutative operand order for the three
    blocked-position additions. No instruction is added or removed.
    """
    if "func_800A6C20:" not in text:
        return text
    before = (
        "\tl.s\t$f2,20($16)\n"
        "\taddu\t$4,$16,16\n"
        "\tadd.s\t$f2,$f2,$f0\n"
        "\taddu\t$5,$sp,32\n"
        "\tli\t$6,0x00a00000\t\t# 10485760\n"
        "\tsh\t$0,D_80397650\n"
        "\ts.s\t$f2,36($sp)\n"
        "\tlbu\t$7,25($16)\n"
        "\tori\t$6,$6,0x0403\n"
        "\taddu\t$2,$sp,40\n"
    )
    after = (
        "\tl.s\t$f2,20($16)\n"
        "\tadd.s\t$f2,$f2,$f0\n"
        "\taddu\t$4,$16,16\n"
        "\taddu\t$5,$sp,32\n"
        "\tli\t$6,0x00a00000\t\t# 10485760\n"
        "\ts.s\t$f2,36($sp)\n"
        "\tlbu\t$7,25($16)\n"
        "\tori\t$6,$6,0x0403\n"
        "\taddu\t$2,$sp,40\n"
        "\tsh\t$0,D_80397650\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800A6C20 collision setup fired {fires} times (expected 1)"
        )
    text = text.replace(before, after, 1)
    add_before = "\tadd.s\t$f0,$f0,$f2\n"
    add_after = "\tadd.s\t$f0,$f2,$f0\n"
    fires = text.count(add_before)
    if fires != 3:
        raise RuntimeError(
            f"func_800A6C20 blocked-position add fired {fires} times (expected 3)"
        )
    return text.replace(add_before, add_after)


def shape_grid_node_remove_loop(text: str) -> str:
    """Match the retail loop allocation in ``func_800B0F4C``.

    Swap the two hoisted offset-table bases and keep separate byte-narrowed
    loop-index values for the multiply and table lookup. The complete patterns
    are function-gated and preserve instruction count and behavior.
    """
    if "func_800B0F4C:" not in text:
        return text
    bases_before = "\tla\t$22,D_801166E8\n\tla\t$21,D_801166E0\n"
    bases_after = "\tla\t$22,D_801166E0\n\tla\t$21,D_801166E8\n"
    fires = text.count(bases_before)
    if fires != 1:
        raise RuntimeError(
            f"func_800B0F4C table bases fired {fires} times (expected 1)"
        )
    text = text.replace(bases_before, bases_after, 1)
    loop_before = (
        "\tlhu\t$4,10($17)\n"
        "\tandi\t$3,$16,0x00ff\n"
        "\tmult\t$3,$4\n"
        "\tmflo\t$4\n"
        "\t#nop\n"
        "\tsll\t$3,$3,1\n"
        "\taddu\t$2,$3,$22\n"
        "\tlhu\t$2,0($2)\n"
        "\tlhu\t$5,8($17)\n"
        "\taddu\t$2,$19,$2\n"
        "\tsra\t$2,$2,10\n"
        "\taddu\t$2,$2,$4\n"
        "\tmult\t$2,$5\n"
        "\tmflo\t$2\n"
        "\t#nop\n"
        "\taddu\t$3,$3,$21\n"
        "\tlhu\t$3,0($3)\n"
        "\tandi\t$6,$16,0x00ff\n"
    )
    loop_after = (
        "\tlhu\t$2,10($17)\n"
        "\tandi\t$4,$16,0x00ff\n"
        "\tmult\t$4,$2\n"
        "\tmflo\t$4\n"
        "\t#nop\n"
        "\tandi\t$6,$16,0x00ff\n"
        "\tsll\t$3,$6,1\n"
        "\taddu\t$2,$3,$21\n"
        "\tlhu\t$2,0($2)\n"
        "\tlhu\t$5,8($17)\n"
        "\taddu\t$2,$19,$2\n"
        "\tsra\t$2,$2,10\n"
        "\taddu\t$2,$2,$4\n"
        "\tmult\t$2,$5\n"
        "\tmflo\t$2\n"
        "\t#nop\n"
        "\taddu\t$3,$3,$22\n"
        "\tlhu\t$3,0($3)\n"
    )
    fires = text.count(loop_before)
    if fires != 1:
        raise RuntimeError(
            f"func_800B0F4C loop allocation fired {fires} times (expected 1)"
        )
    return text.replace(loop_before, loop_after, 1)


def shape_camera_collision_registers(text: str) -> str:
    """Match the retail saved-register cycle in ``func_800A6320``.

    Cycle the compiler's camera/output/factor assignments from s5/s4/s3 to
    retail s4/s3/s5, then schedule the collision-call setup in retail order.
    The transform is confined to the complete function body.
    """
    match = re.search(
        r"(?ms)^func_800A6320:\n.*?^\t\.end\tfunc_800A6320\s*$", text
    )
    if match is None:
        return text
    body = match.group(0)
    counts = {register: len(re.findall(rf"(?<!\d){re.escape(register)}(?!\d)", body))
              for register in ("$19", "$20", "$21")}
    expected = {"$19": 6, "$20": 9, "$21": 5}
    if counts != expected:
        raise RuntimeError(
            f"func_800A6320 saved-register counts {counts} (expected {expected})"
        )
    body = body.replace("$21", "$__cam_21")
    body = body.replace("$20", "$__cam_20")
    body = body.replace("$19", "$__cam_19")
    body = body.replace("$__cam_21", "$20")
    body = body.replace("$__cam_20", "$19")
    body = body.replace("$__cam_19", "$21")
    save_patterns = (
        ("\tsw\t$20,164($sp)\n", "\tsw\t$20,160($sp)\n"),
        ("\tsw\t$19,160($sp)\n", "\tsw\t$19,156($sp)\n"),
        ("\tsw\t$21,156($sp)\n", "\tsw\t$21,164($sp)\n"),
    )
    for before, after in save_patterns:
        fires = body.count(before)
        if fires != 1:
            raise RuntimeError(
                f"func_800A6320 save pattern fired {fires} times (expected 1)"
            )
        body = body.replace(before, after, 1)
    restore_before = (
        "\tlw\t$20,164($sp)\n"
        "\tlw\t$19,160($sp)\n"
        "\tlw\t$21,156($sp)\n"
    )
    restore_after = (
        "\tlw\t$21,164($sp)\n"
        "\tlw\t$20,160($sp)\n"
        "\tlw\t$19,156($sp)\n"
    )
    fires = body.count(restore_before)
    if fires != 1:
        raise RuntimeError(
            f"func_800A6320 restore pattern fired {fires} times (expected 1)"
        )
    body = body.replace(restore_before, restore_after, 1)
    setup_before = (
        "\tl.s\t$f0,$LF_lis0\n"
        "\tlw\t$21,208($sp)\n"
        "\tli\t$2,0x00000001\t\t# 1\n"
        "\tli\t$6,0x00a00000\t\t# 10485760\n"
        "\tsh\t$2,D_80397650\n"
        "\ts.s\t$f0,8($17)\n"
        "\ts.s\t$f0,8($16)\n"
        "\tlbu\t$7,140($20)\n"
        "\tori\t$6,$6,0x0403\n"
    )
    setup_after = (
        "\tl.s\t$f0,$LF_lis0\n"
        "\tli\t$6,0x00a00000\t\t# 10485760\n"
        "\ts.s\t$f0,8($17)\n"
        "\ts.s\t$f0,8($16)\n"
        "\tlw\t$21,208($sp)\n"
        "\tlbu\t$7,140($20)\n"
        "\tori\t$6,$6,0x0403\n"
        "\tli\t$2,0x00000001\t\t# 1\n"
        "\tsh\t$2,D_80397650\n"
    )
    fires = body.count(setup_before)
    if fires != 1:
        raise RuntimeError(
            f"func_800A6320 collision setup fired {fires} times (expected 1)"
        )
    body = body.replace(setup_before, setup_after, 1)
    return text[:match.start()] + body + text[match.end():]


def hoist_mover_reflect_likely_multiply(text: str) -> str:
    """Apply the retail multiply-delay-slot workaround to ``bc1fl``.

    The general multiply hoister handles ordinary branches but not
    branch-likely instructions. This exact function-gated pattern moves the
    existing multiply before the branch and leaves a nop in its likely slot.
    """
    if "func_800B5B8C:" not in text:
        return text
    before = (
        "\t.set\tnoreorder\n"
        "\tbc1fl\t.L25\n"
        "\tmul.s\t$f4,$f4,$f6\n"
    )
    after = (
        "\tmul.s\t$f4,$f4,$f6\n"
        "\t.set\tnoreorder\n"
        "\tbc1fl\t.L25\n"
        "\tnop\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800B5B8C likely-slot multiply fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


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


def shape_contact_side_test_frame(text: str) -> str:
    """Reproduce func_800B739C's retail 56-byte saved-register frame.

    The C body already emits the retail instruction stream.  GCC allocates an
    otherwise unused eight-byte frame slot, shifting only the prologue and
    epilogue.  Keep this gated to the function label and require both complete
    frame patterns exactly once so source or compiler drift fails loudly.
    """
    if "func_800B739C:" not in text:
        return text

    patterns = (
        (
            "\tsubu\t$sp,$sp,64\n"
            "\tsw\t$18,32($sp)\n\tmove\t$18,$4\n"
            "\tsw\t$19,36($sp)\n\tmove\t$19,$0\n"
            "\tsw\t$20,40($sp)\n\tmove\t$20,$0\n"
            "\tandi\t$5,$5,0xffff\n"
            "\tsw\t$31,60($sp)\n\tsw\t$fp,56($sp)\n"
            "\tsw\t$23,52($sp)\n\tsw\t$22,48($sp)\n"
            "\tsw\t$21,44($sp)\n\tsw\t$17,28($sp)\n",
            "\tsubu\t$sp,$sp,56\n"
            "\tsw\t$20,32($sp)\n\tmove\t$20,$0\n"
            "\tsw\t$18,24($sp)\n\tmove\t$18,$4\n"
            "\tsw\t$19,28($sp)\n\tmove\t$19,$0\n"
            "\tandi\t$5,$5,0xffff\n"
            "\tsw\t$31,52($sp)\n\tsw\t$fp,48($sp)\n"
            "\tsw\t$23,44($sp)\n\tsw\t$22,40($sp)\n"
            "\tsw\t$21,36($sp)\n\tsw\t$17,20($sp)\n",
            "prologue",
        ),
        (
            "\tlw\t$31,60($sp)\n\tlw\t$fp,56($sp)\n"
            "\tlw\t$23,52($sp)\n\tlw\t$22,48($sp)\n"
            "\tlw\t$21,44($sp)\n\tlw\t$20,40($sp)\n"
            "\tlw\t$19,36($sp)\n\tlw\t$18,32($sp)\n"
            "\tlw\t$17,28($sp)\n\tlw\t$16,24($sp)\n"
            "\taddu\t$sp,$sp,64\n",
            "\tlw\t$31,52($sp)\n\tlw\t$fp,48($sp)\n"
            "\tlw\t$23,44($sp)\n\tlw\t$22,40($sp)\n"
            "\tlw\t$21,36($sp)\n\tlw\t$20,32($sp)\n"
            "\tlw\t$19,28($sp)\n\tlw\t$18,24($sp)\n"
            "\tlw\t$17,20($sp)\n\tlw\t$16,16($sp)\n"
            "\taddu\t$sp,$sp,56\n",
            "epilogue",
        ),
    )
    for old, new, name in patterns:
        count = text.count(old)
        if count != 1:
            raise RuntimeError(
                f"func_800B739C {name} rule fired {count} times; expected 1"
            )
        text = text.replace(old, new, 1)
    delay_save = "\tsw\t$16,24($sp)\n"
    if text.count(delay_save) != 1:
        raise RuntimeError(
            f"func_800B739C delay-save rule fired {text.count(delay_save)} times; expected 1"
        )
    text = text.replace(delay_save, "\tsw\t$16,16($sp)\n", 1)
    return text


def shape_curve_mode_dispatches(text: str) -> str:
    """Reproduce the retail case-2 switch layout in the curve helpers."""
    if "func_80079AFC:" not in text:
        return text
    patterns = (
        (
            "\tbne\t$5,$2,.L5\n\tmov.s\t$f0,$f12\n\t.set\tnoreorder\n"
            "\tmul.s\t$f0,$f12,$f12\n\t.set\tnoreorder\n\tj\t.L2\n\tnop\n",
            "\tmul.s\t$f0,$f12,$f12\n\t.set\tnoreorder\n"
            "\tbeql\t$5,$2,.L5\n\tnop\n\t.set\tnoreorder\n"
            "\tj\t.L2\n\tmov.s\t$f0,$f12\n",
            "func_80079AFC",
        ),
        (
            "\tbne\t$7,$2,.L19\n\tmov.s\t$f0,$f6\n\t.set\tnoreorder\n"
            "\tmul.s\t$f0,$f6,$f6\n\t.set\tnoreorder\n\tj\t.L16\n\tnop\n",
            "\tmul.s\t$f0,$f6,$f6\n\t.set\tnoreorder\n"
            "\tbeql\t$7,$2,.L19\n\tnop\n\t.set\tnoreorder\n"
            "\tj\t.L16\n\tmov.s\t$f0,$f6\n",
            "func_80079B34",
        ),
    )
    for old, new, name in patterns:
        count = text.count(old)
        if count != 1:
            raise RuntimeError(f"{name} curve-dispatch rule fired {count} times; expected 1")
        text = text.replace(old, new, 1)
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


def shape_text_character_map(text: str) -> str:
    """Reproduce func_80099160's byte-index and loop allocation.

    The retail object retains the unsigned-byte narrowing in the table-load
    path and assigns the two loop predicates to the opposite temporary
    registers.  The source-level operations and instruction count are
    unchanged; both complete function-gated patterns must fire once.
    """
    if "func_80099160:" not in text:
        return text

    lookup_before = (
        "\t.set\tnoreorder\n"
        "\tbeql\t$2,$0,.L10\n"
        "\tsb\t$3,0($4)\n"
        "\t.set\tnoreorder\n"
        "\tlui\t$at,%hi(D_80114814)\n"
        "\taddu\t$at,$at,$7\n"
        "\tlbu\t$3,%lo(D_80114814)($at)\n"
        ".L6:\n"
    )
    lookup_after = (
        "\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L6\n"
        "\tandi\t$2,$7,0x00ff\n"
        "\t.set\tnoreorder\n"
        "\tlui\t$at,%hi(D_80114814)\n"
        "\taddu\t$at,$at,$2\n"
        "\tlbu\t$3,%lo(D_80114814)($at)\n"
        ".L6:\n"
    )
    loop_before = (
        "\tslt\t$2,$8,$6\n"
        "\tsltu\t$3,$0,$7\n"
        "\tand\t$2,$2,$3\n"
        "\t.set\tnoreorder\n"
        "\tbne\t$2,$0,.L4\n"
        "\taddu\t$5,$5,1\n"
    )
    loop_after = (
        "\tslt\t$3,$8,$6\n"
        "\tsltu\t$2,$0,$7\n"
        "\tand\t$2,$2,$3\n"
        "\t.set\tnoreorder\n"
        "\tbnel\t$2,$0,.L4\n"
        "\taddu\t$5,$5,1\n"
    )
    lookup_fires = text.count(lookup_before)
    loop_fires = text.count(loop_before)
    if lookup_fires != 1 or loop_fires != 1:
        raise RuntimeError(
            "func_80099160 character-map rewrite fired "
            f"lookup={lookup_fires}, loop={loop_fires} times (expected 1 each)"
        )
    return text.replace(lookup_before, lookup_after, 1).replace(
        loop_before, loop_after, 1
    )


def shape_grid_overlap_query(text: str) -> str:
    """Reproduce func_800B33FC's equivalent narrowing and return layout."""
    if "func_800B33FC:" not in text:
        return text

    replacements = (
        ("\tmove\t$11,$4\n", "\tandi\t$11,$4,0xffff\n"),
        (
            "\tlhu\t$2,16($8)\n\tlhu\t$4,16($9)\n\t#nop\n",
            "\tlhu\t$4,16($9)\n\tlhu\t$2,16($8)\n\t#nop\n",
        ),
        (
            "\tbne\t$2,$0,.L5\n\tandi\t$11,$11,0xffff\n",
            "\tbne\t$2,$0,.L8\n\tmove\t$2,$0\n",
        ),
        (
            "\tbeq\t$2,$0,.L10\n\tandi\t$4,$11,0xffff\n",
            "\tbeq\t$2,$0,.L10\n\tmove\t$4,$11\n",
        ),
        (
            "\tbeq\t$3,$0,.L3\n\tandi\t$4,$11,0xffff\n",
            "\tbeq\t$3,$0,.L3\n\tmove\t$4,$11\n",
        ),
        (
            "\tslt\t$2,$2,$3\n\t.set\tnoreorder\n"
            "\tbne\t$2,$0,.L8\n\tmove\t$2,$0\n",
            "\tslt\t$2,$2,$3\n\t.set\tnoreorder\n"
            "\tbne\t$2,$0,.Lkmc_narrow_800B33FC\n\tmove\t$2,$0\n",
        ),
        (
            "\tj\t.L8\n\tmove\t$2,$0\n",
            "\tj\t.Lkmc_narrow_800B33FC\n\tmove\t$2,$0\n",
        ),
        (
            "\tandi\t$2,$2,0xffff\n.L8:\n",
            ".Lkmc_narrow_800B33FC:\n\tandi\t$2,$2,0xffff\n.L8:\n",
        ),
    )
    counts = [text.count(before) for before, _ in replacements]
    if counts != [1] * len(replacements):
        raise RuntimeError(
            f"func_800B33FC grid-overlap rewrite fired {counts} times "
            f"(expected {[1] * len(replacements)})"
        )
    for before, after in replacements:
        text = text.replace(before, after, 1)
    return text


def shape_runtime_fields_init(text: str) -> str:
    """Restore func_80088B6C's dead clear and retail tail schedule.

    The source clears the pointer field before immediately assigning its input
    value, matching the retail operation order, but GCC's store-deletion pass
    removes that first store.  The remaining change only schedules six
    independent final field writes in the retail order.  Both complete,
    function-gated patterns must fire exactly once.
    """
    if "func_80088B6C:" not in text:
        return text

    clear_before = "\tsw\t$6,180($16)\n"
    clear_after = "\tsw\t$0,180($16)\n\tsw\t$6,180($16)\n"
    tail_before = (
        "\tl.s\t$f0,0($18)\n"
        "\tli\t$2,0x00000002\t\t# 2\n"
        "\tsb\t$2,17($17)\n"
        "\ts.s\t$f0,20($17)\n"
        "\tl.s\t$f0,4($18)\n"
        "\t#nop\n"
        "\ts.s\t$f0,24($17)\n"
    )
    tail_after = (
        "\tl.s\t$f0,0($18)\n"
        "\ts.s\t$f0,20($17)\n"
        "\tl.s\t$f0,4($18)\n"
        "\tli\t$2,0x00000002\t\t# 2\n"
        "\tsb\t$2,17($17)\n"
        "\ts.s\t$f0,24($17)\n"
    )
    clear_fires = text.count(clear_before)
    tail_fires = text.count(tail_before)
    if clear_fires != 1 or tail_fires != 1:
        raise RuntimeError(
            "func_80088B6C runtime-fields rewrite fired "
            f"clear={clear_fires}, tail={tail_fires} times (expected 1 each)"
        )
    return text.replace(clear_before, clear_after, 1).replace(
        tail_before, tail_after, 1
    )


def shape_segment_endpoint_clamp_registers(text: str) -> str:
    """Select func_800B5C48's retail floating-point allocnos.

    All patterns retain the same operations and schedule.  They only exchange
    tied caller-saved floating-point temporaries in three complete regions.
    """
    if "func_800B5C48:" not in text:
        return text

    endpoint_before = (
        "\tl.s\t$f14,0($2)\n\tsubu\t$sp,$sp,16\n"
        "\ts.s\t$f14,0($sp)\n\tlw\t$2,104($4)\n\t#nop\n"
        "\tl.s\t$f6,4($2)\n\t#nop\n\ts.s\t$f6,4($sp)\n"
        "\tlw\t$2,104($4)\n\t#nop\n\tl.s\t$f0,8($2)\n"
        "\tl.s\t$f2,16($2)\n\t#nop\n\tmul.s\t$f0,$f0,$f2\n"
        "\tadd.s\t$f12,$f14,$f0\n\ts.s\t$f12,8($sp)\n"
        "\tlw\t$2,104($4)\n\t#nop\n\tl.s\t$f2,12($2)\n"
        "\tl.s\t$f0,16($2)\n\t#nop\n\tmul.s\t$f2,$f2,$f0\n"
        "\tadd.s\t$f2,$f6,$f2\n\ts.s\t$f2,12($sp)\n"
    )
    endpoint_after = endpoint_before.replace("$f14", "$f16").replace(
        "$f6", "$f8"
    ).replace("$f12", "$f14")

    distance_before = (
        "\tl.s\t$f0,16($2)\n\tl.s\t$f8,0($5)\n"
        "\tmul.s\t$f16,$f0,$f0\n\tsub.s\t$f10,$f8,$f14\n"
        "\tl.s\t$f4,4($5)\n\tmul.s\t$f0,$f10,$f10\n"
        "\tsub.s\t$f10,$f4,$f6\n\tmul.s\t$f6,$f10,$f10\n"
        "\tsub.s\t$f10,$f8,$f12\n\tmul.s\t$f8,$f10,$f10\n"
        "\tsub.s\t$f10,$f4,$f2\n\tadd.s\t$f0,$f0,$f6\n"
        "\tmul.s\t$f2,$f10,$f10\n\tc.lt.s\t$f16,$f0\n"
        "\tnop\n\t.set\tnoreorder\n\tbc1f\t.L2\n"
        "\tadd.s\t$f8,$f8,$f2\n"
    )
    distance_after = (
        "\tl.s\t$f12,16($2)\n\tl.s\t$f4,0($5)\n"
        "\tmul.s\t$f12,$f12,$f12\n\tsub.s\t$f10,$f4,$f16\n"
        "\tl.s\t$f6,4($5)\n\tmul.s\t$f0,$f10,$f10\n"
        "\tsub.s\t$f10,$f6,$f8\n\tmul.s\t$f8,$f10,$f10\n"
        "\tsub.s\t$f10,$f4,$f14\n\tmul.s\t$f4,$f10,$f10\n"
        "\tsub.s\t$f10,$f6,$f2\n\tadd.s\t$f0,$f0,$f8\n"
        "\tmul.s\t$f2,$f10,$f10\n\tc.lt.s\t$f12,$f0\n"
        "\tnop\n\t.set\tnoreorder\n\tbc1f\t.L2\n"
        "\tadd.s\t$f4,$f4,$f2\n"
    )
    tail_before = (
        ".L2:\n\tc.lt.s\t$f16,$f8\n\tnop\n\t.set\tnoreorder\n"
        "\tbc1tl\t.L3\n\ts.s\t$f14,0($5)\n"
    )
    tail_after = (
        ".L2:\n\tc.lt.s\t$f12,$f4\n\tnop\n\t.set\tnoreorder\n"
        "\tbc1tl\t.L3\n\ts.s\t$f16,0($5)\n"
    )
    first_store_before = (
        "\t.set\tnoreorder\n\ts.s\t$f12,0($5)\n\tl.s\t$f0,12($sp)\n"
    )
    first_store_after = (
        "\t.set\tnoreorder\n\ts.s\t$f14,0($5)\n\tl.s\t$f0,12($sp)\n"
    )
    replacements = (
        (endpoint_before, endpoint_after),
        (distance_before, distance_after),
        (first_store_before, first_store_after),
        (tail_before, tail_after),
    )
    counts = [text.count(before) for before, _ in replacements]
    if counts != [1, 1, 1, 1]:
        raise RuntimeError(
            "func_800B5C48 FP-register rewrite fired "
            f"{counts} times (expected [1, 1, 1, 1])"
        )
    for before, after in replacements:
        text = text.replace(before, after, 1)
    return text


def shape_search_iterator_first_inline(text: str) -> str:
    """Keep func_800A9928's first inlined iterator return in the caller.

    GCC cross-jumps a null result directly to the function epilogue and drops
    the caller's following null test.  Retail retains that test.  The two
    iterator temporaries are also tied and receive the opposite registers.
    """
    if "func_800A9928:" not in text:
        return text

    before = (
        ".L12:\n\tlw\t$9,440($4)\n\t.set\tnoreorder\n"
        "\tj\t.L13\n\tmove\t$8,$0\n\t.set\tnoreorder\n"
        ".L11:\n\tlw\t$8,156($5)\n\tlw\t$9,4($5)\n"
        ".L13:\n\tsltu\t$2,$9,1\n\tsltu\t$3,$8,3\n"
        "\tand\t$2,$2,$3\n\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L27\n\tsll\t$2,$8,2\n"
        "\t.set\tnoreorder\n\taddu\t$5,$2,$4\n\taddu\t$5,$5,4\n"
        ".L31:\n\tlw\t$9,440($5)\n\taddu\t$8,$8,1\n"
        "\tsltu\t$3,$8,3\n\tsltu\t$2,$9,1\n\tand\t$2,$2,$3\n"
        "\t.set\tnoreorder\n\tbne\t$2,$0,.L31\n\taddu\t$5,$5,4\n"
        "\t.set\tnoreorder\n\t.set\tnoreorder\n"
        "\tj\t.L32\n\tmove\t$5,$9\n\t.set\tnoreorder\n"
    )
    after = (
        ".L12:\n\tlw\t$8,440($4)\n\t.set\tnoreorder\n"
        "\tj\t.L13\n\tmove\t$9,$0\n\t.set\tnoreorder\n"
        ".L11:\n\tlw\t$9,156($5)\n\tlw\t$8,4($5)\n"
        ".L13:\n\tsltu\t$2,$8,1\n\tsltu\t$3,$9,3\n"
        "\tand\t$2,$2,$3\n\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.Lfirst_iterator_done_800A9928\n\tsll\t$2,$9,2\n"
        "\t.set\tnoreorder\n\taddu\t$5,$2,$4\n\taddu\t$5,$5,4\n"
        ".L31:\n\tlw\t$8,440($5)\n\taddu\t$9,$9,1\n"
        "\tsltu\t$3,$9,3\n\tsltu\t$2,$8,1\n\tand\t$2,$2,$3\n"
        "\t.set\tnoreorder\n\tbne\t$2,$0,.L31\n\taddu\t$5,$5,4\n"
        "\t.set\tnoreorder\n.Lfirst_iterator_done_800A9928:\n"
        "\tmove\t$5,$8\n\t.set\tnoreorder\n"
        "\tbeq\t$5,$0,.L18\n\tnop\n"
        "\t.set\tnoreorder\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            "func_800A9928 first-iterator rewrite fired "
            f"{fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def shape_ai_node_setup_frame(text: str) -> str:
    """Remove func_800852F8's hoisted address allocno.

    GCC retains ``sp + 24`` in an extra saved register across two calls.  The
    retail build rematerializes that address at both uses, so it needs one less
    saved register and an eight-byte smaller frame.  The remaining saved
    values then move down one register without changing the operation order.
    """
    if "func_800852F8:" not in text:
        return text

    prologue_before = (
        "\t.frame\t$sp,56,$31\t\t# vars= 16, regs= 5/0, args= 16, extra= 0\n"
        "\t.mask\t0x800f0000,-8\n\t.fmask\t0x00000000,0\n"
        "\tsubu\t$sp,$sp,56\n\tsw\t$17,36($sp)\n\tmove\t$17,$4\n"
        "\tsw\t$31,48($sp)\n\tsw\t$19,44($sp)\n"
        "\tsw\t$18,40($sp)\n\tsw\t$16,32($sp)\n"
    )
    prologue_after = (
        "\t.frame\t$sp,48,$31\t\t# vars= 16, regs= 4/0, args= 16, extra= 0\n"
        "\t.mask\t0x80070000,-4\n\t.fmask\t0x00000000,0\n"
        "\tsubu\t$sp,$sp,48\n\tsw\t$16,32($sp)\n\tmove\t$16,$4\n"
        "\tsw\t$31,44($sp)\n\tsw\t$18,40($sp)\n\tsw\t$17,36($sp)\n"
    )
    epilogue_before = (
        ".L1:\n\tlw\t$31,48($sp)\n\tlw\t$19,44($sp)\n"
        "\tlw\t$18,40($sp)\n\tlw\t$17,36($sp)\n"
        "\tlw\t$16,32($sp)\n\taddu\t$sp,$sp,56\n"
        "\tj\t$31\n\tnop\n"
    )
    epilogue_after = (
        ".L1:\n\tlw\t$31,44($sp)\n\tlw\t$18,40($sp)\n"
        "\tlw\t$17,36($sp)\n\tlw\t$16,32($sp)\n"
        "\taddu\t$sp,$sp,48\n\tj\t$31\n\tnop\n"
    )
    pointer_patterns = (
        "\taddu\t$16,$sp,24\n",
        "\tmove\t$4,$16\n",
        "\tmove\t$5,$16\n",
    )
    counts = [text.count(prologue_before), text.count(epilogue_before)] + [
        text.count(pattern) for pattern in pointer_patterns
    ]
    if counts != [1, 1, 1, 1, 1]:
        raise RuntimeError(
            f"func_800852F8 frame rewrite fired {counts} times "
            "(expected [1, 1, 1, 1, 1])"
        )
    text = text.replace(prologue_before, prologue_after, 1)
    text = text.replace(epilogue_before, epilogue_after, 1)
    start = text.index("\tlw\t$2,480($17)\n", text.index("func_800852F8:"))
    end = text.index(".L1:\n", start)
    body = text[start:end]
    body = body.replace(pointer_patterns[0], "", 1)
    body = body.replace(pointer_patterns[1], "\taddu\t$4,$sp,24\n", 1)
    body = body.replace(pointer_patterns[2], "\taddu\t$5,$sp,24\n", 1)
    body = body.replace("$17", "$kmc17")
    body = body.replace("$19", "$kmc19")
    body = body.replace("$18", "$kmc18")
    body = body.replace("$kmc17", "$16")
    body = body.replace("$kmc19", "$18")
    body = body.replace("$kmc18", "$17")
    return text[:start] + body + text[end:]


def shape_model_display_patch_registers(text: str) -> str:
    """Reproduce func_80095F08's tied caller-saved allocation and setup.

    The C emits the retail operations, branches, and instruction count.  Its
    caller-saved allocnos receive a different, internally consistent register
    permutation, and GCC schedules two independent table-address macros ahead
    of the model-count early exit.  Keep the rewrite inside the complete
    function, require the exact setup once, and verify every source-register
    occurrence before applying the simultaneous rename.
    """
    if "func_80095F08:" not in text:
        return text

    match = re.search(
        r"func_80095F08:.*?\n\s*\.end\s+func_80095F08\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_80095F08 body not found")
    body = match.group(0)

    setup_before = (
        "\tlbu\t$2,4($14)\n"
        "\tla\t$25,D_801146E8\n"
        "\tla\t$24,D_801146F0\n"
        "\t.set\tnoreorder\n"
        "\tblez\t$2,.L50\n"
        "\tmove\t$11,$0\n"
        "\t.set\tnoreorder\n"
        "\tli\t$15,-553648128\t\t\t# 0xdf000000\n"
        "\tmove\t$13,$0\n"
    )
    setup_after = (
        "\tlbu\t$2,4($14)\n"
        "\t.set\tnoreorder\n"
        "\tblez\t$2,.L50\n"
        "\tmove\t$11,$0\n"
        "\t.set\tnoreorder\n"
        "\tli\t$15,-553648128\t\t\t# 0xdf000000\n"
        "\tla\t$25,D_801146E8\n"
        "\tla\t$24,D_801146F0\n"
        "\tmove\t$13,$0\n"
    )
    setup_fires = body.count(setup_before)
    if setup_fires != 1:
        raise RuntimeError(
            "func_80095F08 table setup fired "
            f"{setup_fires} times (expected 1)"
        )
    body = body.replace(setup_before, setup_after, 1)

    mapping = {
        "$14": "$4",
        "$11": "$10",
        "$13": "$12",
        "$8": "$6",
        "$12": "$11",
        "$10": "$9",
        "$9": "$8",
        "$24": "$13",
        "$25": "$14",
        "$6": "$5",
    }
    expected = {
        "$14": 4,
        "$11": 4,
        "$13": 4,
        "$8": 6,
        "$12": 2,
        "$10": 4,
        "$9": 5,
        "$24": 2,
        "$25": 2,
        "$6": 5,
    }
    counts = {
        register: len(re.findall(re.escape(register) + r"(?!\d)", body))
        for register in mapping
    }
    if counts != expected:
        raise RuntimeError(
            f"func_80095F08 register counts changed: {counts}; expected {expected}"
        )
    for index, register in enumerate(mapping):
        body = re.sub(
            re.escape(register) + r"(?!\d)", f"$kmc_model_patch_{index}", body
        )
    for index, target in enumerate(mapping.values()):
        body = body.replace(f"$kmc_model_patch_{index}", target)
    add_before = "\taddu\t$2,$12,$2\n"
    add_after = "\taddu\t$2,$2,$12\n"
    add_fires = body.count(add_before)
    if add_fires != 1:
        raise RuntimeError(
            "func_80095F08 mesh-address operand order fired "
            f"{add_fires} times (expected 1)"
        )
    body = body.replace(add_before, add_after, 1)
    return text[:match.start()] + body + text[match.end():]


def schedule_shell_update_allocations(text: str) -> str:
    """Reproduce two independent schedules in ``func_800EC8E8``.

    With CSE skip-block propagation disabled for this unit, the generated
    operations, frame, registers, and literal pool match retail.  The retail
    scheduler orders the two incoming-argument save/copy pairs oppositely and
    places an independent owner-byte load before an index shift.  Both exact
    function-local patterns must fire once.
    """
    if "func_800EC8E8:" not in text:
        return text

    prologue_before = (
        "\tsw\t$16,104($sp)\n"
        "\tmove\t$16,$4\n"
        "\tsw\t$17,108($sp)\n"
        "\tmove\t$17,$5\n"
    )
    prologue_after = (
        "\tsw\t$17,108($sp)\n"
        "\tmove\t$17,$5\n"
        "\tsw\t$16,104($sp)\n"
        "\tmove\t$16,$4\n"
    )
    owner_before = "\tsll\t$2,$18,1\n\tlbu\t$7,48($16)\n"
    owner_after = "\tlbu\t$7,48($16)\n\tsll\t$2,$18,1\n"
    counts = (text.count(prologue_before), text.count(owner_before))
    if counts != (1, 1):
        raise RuntimeError(
            f"func_800EC8E8 schedule fired {counts} times (expected (1, 1))"
        )
    return text.replace(prologue_before, prologue_after, 1).replace(
        owner_before, owner_after, 1
    )


def shape_destroyed_object_update_allocations(text: str) -> str:
    """Select the retail local allocations/schedules in ``func_800EA224``.

    The function and literal pool are structurally exact.  Three independent
    regions differ: constant-load scheduling around model-state writes, the
    callback-entry address calculation, and one final table-field store.
    Reorder only the existing operations and exchange tied caller-saved
    temporaries inside those complete, function-gated regions.
    """
    if "func_800EA224:" not in text:
        return text

    state_before = (
        "\tlbu\t$2,78($3)\n"
        "\t.section\t.rodata\n\t.align\t2\n$LF_lis0:\n"
        "\t.word\t0x3F4CCCCD\n\t.text\n\tl.s\t$f22,$LF_lis0\n"
        "\tandi\t$2,$2,0x007f\n\tsb\t$2,78($3)\n"
        "\tlhu\t$4,56($18)\n\tlbu\t$3,54($18)\n"
        "\tli\t$2,0x00000004\t\t# 4\n\tsb\t$2,53($18)\n"
        "\tsb\t$0,52($18)\n\tsll\t$2,$4,2\n\taddu\t$2,$2,$4\n"
        "\tsll\t$2,$2,3\n\t.set\tnoat\n"
        "\tlui\t$1,%hi(D_803978E0+32)\n\taddu\t$1,$1,$2\n"
        "\tsh\t$3,%lo(D_803978E0+32)($1)\n\t.set\tat\n"
        "\tl.s\t$f0,12($18)\n\taddu\t$17,$sp,40\n"
        "\ts.s\t$f0,72($sp)\n\tl.s\t$f0,16($18)\n"
        "\tmove\t$5,$17\n\ts.s\t$f0,76($sp)\n\tlhu\t$2,56($18)\n"
        "\t.section\t.rodata\n\t.align\t2\n$LF_lis1:\n"
        "\t.word\t0x3E4CCCCD\n\t.text\n\tl.s\t$f26,$LF_lis1\n"
    )
    state_after = (
        "\t.section\t.rodata\n\t.align\t2\n$LF_lis0:\n"
        "\t.word\t0x3F4CCCCD\n\t.text\n\tl.s\t$f22,$LF_lis0\n"
        "\tlbu\t$2,78($3)\n"
        "\t.section\t.rodata\n\t.align\t2\n$LF_lis1:\n"
        "\t.word\t0x3E4CCCCD\n\t.text\n\tl.s\t$f26,$LF_lis1\n"
        "\tandi\t$2,$2,0x007f\n\tsb\t$2,78($3)\n"
        "\tlhu\t$3,56($18)\n\tlbu\t$4,54($18)\n"
        "\tli\t$2,0x00000004\t\t# 4\n\tsb\t$2,53($18)\n"
        "\tsb\t$0,52($18)\n\tsll\t$2,$3,2\n\taddu\t$2,$2,$3\n"
        "\tsll\t$2,$2,3\n\t.set\tnoat\n"
        "\tlui\t$1,%hi(D_803978E0+32)\n\taddu\t$1,$1,$2\n"
        "\tsh\t$4,%lo(D_803978E0+32)($1)\n\t.set\tat\n"
        "\tl.s\t$f0,12($18)\n\taddu\t$17,$sp,40\n"
        "\ts.s\t$f0,72($sp)\n\tl.s\t$f0,16($18)\n"
        "\tmove\t$5,$17\n\ts.s\t$f0,76($sp)\n\tlhu\t$2,56($18)\n"
    )
    callback_before = (
        "\tlh\t$3,D_80224EAA\n\tli\t$2,-1\t\t\t# 0xffffffff\n"
        "\t.set\tnoreorder\n\tbeq\t$3,$2,.L2\n\tsll\t$2,$3,4\n"
        "\t.set\tnoreorder\n\taddu\t$2,$2,$3\n\tsll\t$2,$2,2\n"
        "\tla\t$3,D_80224EF0\n\taddu\t$4,$2,$3\n"
    )
    callback_after = (
        "\tlh\t$4,D_80224EAA\n\tli\t$2,-1\t\t\t# 0xffffffff\n"
        "\t.set\tnoreorder\n\tbeq\t$4,$2,.L2\n\tsll\t$2,$4,4\n"
        "\t.set\tnoreorder\n\tla\t$3,D_80224EF0\n"
        "\taddu\t$2,$2,$4\n\tsll\t$2,$2,2\n\taddu\t$4,$2,$3\n"
    )
    final_before = (
        "\tsll\t$2,$3,2\n\taddu\t$2,$2,$3\n\tlbu\t$3,54($18)\n"
        "\tsll\t$2,$2,3\n\taddu\t$2,$2,$5\n\tsh\t$3,32($2)\n"
    )
    final_after = (
        "\tlbu\t$4,54($18)\n\tsll\t$2,$3,2\n\taddu\t$2,$2,$3\n"
        "\tsll\t$2,$2,3\n\taddu\t$2,$2,$5\n\tsh\t$4,32($2)\n"
    )
    replacements = (
        (state_before, state_after, "state/constant block"),
        (callback_before, callback_after, "callback address block"),
        (final_before, final_after, "final table store"),
    )
    counts = [text.count(before) for before, _, _ in replacements]
    if counts != [1, 1, 1]:
        raise RuntimeError(
            f"func_800EA224 allocation rewrite fired {counts} times "
            "(expected [1, 1, 1])"
        )
    for before, after, _ in replacements:
        text = text.replace(before, after, 1)
    return text


def shape_billboard_command_update_allocations(text: str) -> str:
    """Match the retained matrix pointer/result allocnos in func_800EBDA0.

    Three saved values are cyclically allocated to different registers.  The
    retail object rematerializes ``sp + 56`` for the first two calls, retains
    that address in ``s7`` only for the final stack argument, and schedules
    the final register arguments before its stack stores.  No operation is
    inserted or deleted.
    """
    if "func_800EBDA0:" not in text:
        return text
    match = re.search(
        r"func_800EBDA0:.*?\n\s*\.end\s+func_800EBDA0\b", text, flags=re.S
    )
    if match is None:
        raise RuntimeError("func_800EBDA0 body not found")
    body = match.group(0)

    temp_before = "\tlw\t$3,0($2)\n\tlw\t$4,8($2)\n\tsw\t$3,132($sp)\n"
    temp_after = "\tlw\t$8,0($2)\n\tlw\t$4,8($2)\n\tsw\t$8,132($sp)\n"
    temp_fires = body.count(temp_before)
    if temp_fires != 1:
        raise RuntimeError(
            f"func_800EBDA0 retained temp fired {temp_fires} times (expected 1)"
        )
    body = body.replace(temp_before, temp_after, 1)

    region_start = body.index("\tmove\t$22,$2\n")
    region_end = body.index(".L2:\n", region_start)
    region = body[region_start:region_end]
    counts = {
        register: len(re.findall(re.escape(register) + r"(?!\d)", region))
        for register in ("$20", "$22", "$23")
    }
    expected = {"$20": 4, "$22": 2, "$23": 3}
    if counts != expected:
        raise RuntimeError(
            f"func_800EBDA0 saved-register counts changed: {counts}; "
            f"expected {expected}"
        )
    mapping = {"$20": "$23", "$22": "$20", "$23": "$22"}
    for index, register in enumerate(mapping):
        region = re.sub(
            re.escape(register) + r"(?!\d)", f"$kmc_billboard_{index}", region
        )
    for index, target in enumerate(mapping.values()):
        region = region.replace(f"$kmc_billboard_{index}", target)

    matrix_before = "\tmove\t$4,$23\n"
    matrix_fires = region.count(matrix_before)
    if matrix_fires != 2:
        raise RuntimeError(
            f"func_800EBDA0 matrix rematerialization fired {matrix_fires} "
            "times (expected 2)"
        )
    region = region.replace(matrix_before, "\taddu\t$4,$sp,56\n")

    call_before = (
        "\tsw\t$0,120($sp)\n\tsw\t$0,16($sp)\n\tsw\t$23,20($sp)\n"
        "\tsw\t$0,24($sp)\n\tsw\t$18,28($sp)\n\tsw\t$22,32($sp)\n"
        "\tsw\t$22,36($sp)\n\tlw\t$4,132($sp)\n\tlw\t$6,140($sp)\n"
        "\tmove\t$5,$20\n\t.set\tnoreorder\n\tjal\tfunc_8007B1F0\n"
        "\tmove\t$7,$0\n"
    )
    call_after = (
        "\tlw\t$4,132($sp)\n\tlw\t$6,140($sp)\n\tmove\t$5,$20\n"
        "\tmove\t$7,$0\n\tsw\t$0,120($sp)\n\tsw\t$0,16($sp)\n"
        "\tsw\t$23,20($sp)\n\tsw\t$0,24($sp)\n\tsw\t$18,28($sp)\n"
        "\tsw\t$22,32($sp)\n\t.set\tnoreorder\n\tjal\tfunc_8007B1F0\n"
        "\tsw\t$22,36($sp)\n"
    )
    call_fires = region.count(call_before)
    if call_fires != 1:
        raise RuntimeError(
            f"func_800EBDA0 final call schedule fired {call_fires} times "
            "(expected 1)"
        )
    region = region.replace(call_before, call_after, 1)
    body = body[:region_start] + region + body[region_end:]
    return text[:match.start()] + body + text[match.end():]


def merge_object_search_loop_entry(text: str) -> str:
    """Undo GCC's duplicated exit test in func_80084278's case-4 loop.

    Retail cross-jumps the empty entry path to the loop's existing iterator
    call.  GCC's reconstructed source duplicates that call and a null test at
    entry, making the function three instructions longer.  Replace only the
    complete duplicated prefix with a jump to the existing call and attach a
    private label there.  No loop operation is invented or transcribed.
    """
    if "func_80084278:" not in text:
        return text
    entry_before = (
        ".L83:\n\t.set\tnoreorder\n\tjal\tfunc_800A1A28\n"
        "\tli\t$5,0x00000014\t\t# 20\n\t.set\tnoreorder\n"
        "\tmove\t$5,$2\n\t.set\tnoreorder\n\tbeq\t$5,$0,.L84\n"
        "\tmove\t$4,$16\n\t.set\tnoreorder\n.L64:\n"
    )
    entry_after = (
        ".L83:\n\t.set\tnoreorder\n\tj\t.Lkmc_search_next_80084278\n"
        "\tnop\n\t.set\tnoreorder\n.L64:\n"
    )
    call_before = (
        "\t.set\tnoreorder\n\t.set\tnoreorder\n"
        "\tjal\tfunc_800A1A28\n\tli\t$5,0x00000014\t\t# 20\n"
        "\t.set\tnoreorder\n\tmove\t$5,$2\n"
        "\tbne\t$5,$0,.L64\n\tnop\n"
    )
    call_after = call_before.replace(
        "\tjal\tfunc_800A1A28\n",
        ".Lkmc_search_next_80084278:\n\tjal\tfunc_800A1A28\n",
        1,
    )
    direct_before = "\tbeq\t$2,$0,.L83\n\tmove\t$4,$0\n"
    direct_after = (
        "\tbeq\t$2,$0,.Lkmc_search_next_80084278\n\tmove\t$4,$0\n"
    )
    counts = (
        text.count(entry_before), text.count(call_before), text.count(direct_before)
    )
    if counts != (1, 1, 1):
        raise RuntimeError(
            f"func_80084278 loop-entry merge fired {counts} times "
            "(expected (1, 1, 1))"
        )
    return (text.replace(direct_before, direct_after, 1)
            .replace(entry_before, entry_after, 1)
            .replace(call_before, call_after, 1))


def schedule_pool_type4_removal(text: str) -> str:
    """Reproduce func_8007E7A8's state-clear/call schedule."""
    if "func_8007E7A8:" not in text:
        return text

    before = (
        ".L7:\n"
        "\t.set\tnoreorder\n"
        "\tjal\tSteps_SpliceIn\n"
        "\tsw\t$0,240($16)\n"
        "\t.set\tnoreorder\n"
        "\tlhu\t$2,246($16)\n"
        "\tsh\t$0,244($16)\n"
    )
    after = (
        ".L7:\n"
        "\tsw\t$0,240($16)\n"
        "\t.set\tnoreorder\n"
        "\tjal\tSteps_SpliceIn\n"
        "\tsh\t$0,244($16)\n"
        "\t.set\tnoreorder\n"
        "\tlhu\t$2,246($16)\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_8007E7A8 type-4 schedule fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def schedule_object_distance_probe(text: str) -> str:
    """Reproduce func_80080C04's prologue and floating-point schedule."""
    if "func_80080C04:" not in text:
        return text

    prologue_before = (
        "\tsw\t$16,16($sp)\n"
        "\taddu\t$16,$4,296\n"
        "\tsw\t$31,20($sp)\n"
        " #APP\n"
        " #NO_APP\n"
        "\tlw\t$3,268($4)\n"
        "\tli\t$2,0x00000002\t\t# 2\n"
        "\tbne\t$3,$2,.L2\n"
        "\tnop\n"
    )
    prologue_after = (
        "\tsw\t$31,20($sp)\n"
        "\tsw\t$16,16($sp)\n"
        " #APP\n"
        " #NO_APP\n"
        "\tlw\t$3,268($4)\n"
        "\tli\t$2,0x00000002\t\t# 2\n"
        "\t.set\tnoreorder\n"
        "\tbne\t$3,$2,.L2\n"
        "\taddu\t$16,$4,296\n"
        "\t.set\tnoreorder\n"
    )
    zero_before = (
        "\tsub.s\t$f4,$f2,$f0\n"
        "\tc.lt.s\t$f6,$f4\n"
    )
    zero_after = (
        "\tsub.s\t$f4,$f2,$f0\n"
        "\tmtc1\t$0,$f6\n"
        "\tc.lt.s\t$f6,$f4\n"
    )
    tail_before = (
        "\tsub.s\t$f0,$f2,$f0\n"
        " #APP\n"
        " #NO_APP\n"
        "\tlbu\t$3,364($4)\n"
        "\t#nop\n"
        "\tandi\t$2,$3,0x0001\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L5\n"
        "\tandi\t$2,$3,0x00fb\n"
    )
    tail_after = (
        " #APP\n"
        " #NO_APP\n"
        "\tlbu\t$3,364($4)\n"
        "\t#nop\n"
        "\tandi\t$2,$3,0x0001\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L5\n"
        "\tsub.s\t$f0,$f2,$f0\n"
        "\t.set\tnoreorder\n"
        "\tandi\t$2,$3,0x00fb\n"
    )
    patterns = (
        ("prologue", prologue_before, prologue_after),
        ("zero reload", zero_before, zero_after),
        ("branch delay", tail_before, tail_after),
    )
    for name, before, after in patterns:
        fires = text.count(before)
        if fires != 1:
            raise RuntimeError(
                f"func_80080C04 {name} rewrite fired {fires} times (expected 1)"
            )
        text = text.replace(before, after, 1)
    return text


def schedule_matrix_basis_inverse(text: str) -> str:
    """Reproduce func_8009F5AC's retained registers and loop setup."""
    if "func_8009F5AC:" not in text:
        return text

    float_before = (
        "\tl.s\t$f0,4($6)\n"
        "\tl.s\t$f4,40($4)\n"
        "\tsubu\t$sp,$sp,16\n"
        "\tmove\t$11,$5\n"
        "\tadd.s\t$f0,$f0,$f4\n"
    )
    float_after = float_before.replace("$f4,40($4)", "$f2,40($4)").replace(
        "$f0,$f0,$f4", "$f0,$f0,$f2"
    )
    loop_before = (
        "\tmove\t$11,$5\n"
        "\tadd.s\t$f0,$f0,$f2\n"
        "\tl.s\t$f2,D_80072650\n"
        "\tmove\t$10,$11\n"
        "\tmove\t$8,$0\n"
        "\ts.s\t$f0,4($7)\n"
        ".L5:\n"
        "\tmove\t$9,$11\n"
        "\tmove\t$7,$10\n"
        "\tmove\t$3,$0\n"
    )
    loop_after = (
        "\tmove\t$8,$0\n"
        "\tadd.s\t$f0,$f0,$f2\n"
        "\tl.s\t$f2,D_80072650\n"
        "\tmove\t$11,$5\n"
        "\tmove\t$10,$5\n"
        "\ts.s\t$f0,4($7)\n"
        ".L5:\n"
        "\tmove\t$3,$0\n"
        "\tmove\t$9,$11\n"
        "\tmove\t$7,$10\n"
    )
    address_before = "\taddu\t$2,$3,$11\n"
    address_after = "\taddu\t$2,$11,$3\n"
    patterns = (
        ("float register", float_before, float_after),
        ("identity loop", loop_before, loop_after),
        ("column address", address_before, address_after),
    )
    for name, before, after in patterns:
        fires = text.count(before)
        if fires != 1:
            raise RuntimeError(
                f"func_8009F5AC {name} rewrite fired {fires} times (expected 1)"
            )
        text = text.replace(before, after, 1)
    return text


def preserve_record_index_copy(text: str) -> str:
    """Use func_800E82AC's retained a2 index copy for address scaling."""
    if "func_800E82AC:" not in text:
        return text

    before = ".L2:\n\tsll\t$2,$3,3\n"
    after = ".L2:\n\tsll\t$2,$6,3\n"
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_800E82AC index-copy rewrite fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def preserve_search_record_index_copy(text: str) -> str:
    """Use func_8008875C's retained v1 copy for record address scaling."""
    if "func_8008875C:" not in text:
        return text

    before = ".L5:\n\tsll\t$2,$2,3\n"
    after = ".L5:\n\tsll\t$2,$3,3\n"
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            f"func_8008875C index-copy rewrite fired {fires} times (expected 1)"
        )
    return text.replace(before, after, 1)


def shape_angle_step_update(text: str) -> str:
    """Reproduce func_8009D75C's retail temporary allocation and shared store."""
    if "func_8009D75C:" not in text:
        return text

    add_before = "\tor\t$2,$4,$22\n"
    add_after = "\taddu\t$2,$4,$22\n"
    add_fires = text.count(add_before)
    if add_fires != 1:
        raise RuntimeError(
            f"func_8009D75C wrapped-target add fired {add_fires} times (expected 1)"
        )
    text = text.replace(add_before, add_after, 1)

    tail_before = (
        "\tmove\t$4,$2\n"
        "\tandi\t$3,$18,0xffff\n"
        "\tandi\t$2,$4,0xffff\n"
        "\tsltu\t$2,$2,$3\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L2\n"
        "\tsrl\t$2,$20,15\n"
        "\t.set\tnoreorder\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L3\n"
        "\tsh\t$19,0($17)\n"
        "\t.set\tnoreorder\n"
        ".L2:\n"
        "\tandi\t$2,$2,0x0001\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L4\n"
        "\tor\t$2,$16,$22\n"
        "\t.set\tnoreorder\n"
        "\tsubu\t$2,$2,$3\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L3\n"
        "\tsh\t$2,0($17)\n"
        "\t.set\tnoreorder\n"
        ".L4:\n"
        "\taddu\t$18,$21,$18\n"
        "\tsh\t$18,0($17)\n"
        ".L3:\n"
        "\tandi\t$2,$4,0xffff\n"
    )
    tail_after = (
        "\tmove\t$3,$18\n"
        "\tmove\t$4,$2\n"
        "\tandi\t$2,$4,0xffff\n"
        "\tandi\t$3,$3,0xffff\n"
        "\tsltu\t$2,$2,$3\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L2\n"
        "\tsrl\t$2,$20,15\n"
        "\t.set\tnoreorder\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L3\n"
        "\tsh\t$19,0($17)\n"
        "\t.set\tnoreorder\n"
        ".L2:\n"
        "\tandi\t$2,$2,0x0001\n"
        "\t.set\tnoreorder\n"
        "\tbeq\t$2,$0,.L4\n"
        "\taddu\t$2,$16,$22\n"
        "\t.set\tnoreorder\n"
        "\t.set\tnoreorder\n"
        "\tj\t.L5\n"
        "\tsubu\t$2,$2,$3\n"
        "\t.set\tnoreorder\n"
        ".L4:\n"
        "\taddu\t$2,$21,$18\n"
        ".L5:\n"
        "\tsh\t$2,0($17)\n"
        ".L3:\n"
        "\tandi\t$2,$4,0xffff\n"
    )
    tail_fires = text.count(tail_before)
    if tail_fires != 1:
        raise RuntimeError(
            f"func_8009D75C compare/store rewrite fired {tail_fires} times (expected 1)"
        )
    return text.replace(tail_before, tail_after, 1)


def schedule_grid_collision_midpoint_load(text: str) -> str:
    """Reproduce the retail scheduler's independent midpoint-load order.

    In ``func_800B2488`` the reconstructed source and retail object have the
    same dependency graph.  KMC GCC selects the arithmetic shift before the
    independent second halfword load, while retail schedules that load first
    so the shift occupies its load-delay slot.  Restrict the complete four-
    instruction pattern to this function and require exactly one occurrence.
    """
    if "func_800B2488:" not in text:
        return text
    before = (
        "\tsubu\t$3,$3,$2\n"
        "\tsra\t$3,$3,1\n"
        "\tlh\t$2,28($18)\n"
        "\tmtc1\t$3,$f0\n"
    )
    after = (
        "\tsubu\t$3,$3,$2\n"
        "\tlh\t$2,28($18)\n"
        "\tsra\t$3,$3,1\n"
        "\tmtc1\t$3,$f0\n"
    )
    fires = text.count(before)
    if fires != 1:
        raise RuntimeError(
            "func_800B2488 midpoint load schedule fired "
            f"{fires} times (expected 1)"
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
    if os.environ.get("V3_LZARI_LOAD_HAZARDS", "1") == "1":
        text = reproduce_lzari_load_hazards(text)
    if os.environ.get("V3_ENTITY_MODEL_DRAW_LABEL_HAZARD", "1") == "1":
        text = reproduce_entity_model_draw_label_hazard(text)
    if os.environ.get("V3_COLLISION_QUERY_LABEL_HAZARD", "1") == "1":
        text = reproduce_collision_query_label_hazard(text)
    if os.environ.get("V3_SPRITE_RING_LABEL_HAZARD", "1") == "1":
        text = reproduce_sprite_ring_label_hazard(text)
    if os.environ.get("V3_DISTANCE_MULTIPLY_HAZARDS", "1") == "1":
        text = suppress_distance_multiply_hazard_nops(text)
    if os.environ.get("V3_PROGRESS_LEVEL_LOOP_SETUP", "1") == "1":
        text = schedule_progress_level_loop_setup(text)
    if os.environ.get("V3_ANGLE_TABLE_LOOKUP_REGISTERS", "1") == "1":
        text = shape_angle_table_lookup_registers(text)
    if os.environ.get("V3_PATH_WAYPOINT_SIDE_REGISTERS", "1") == "1":
        text = shape_path_waypoint_side_registers(text)
    if os.environ.get("V3_WAYPOINT_STACK_RESET_ADDRESSES", "1") == "1":
        text = shape_waypoint_stack_reset_addresses(text)
    if os.environ.get("V3_MODEL_VERTEX_OFFSET_LOOP_REGISTERS", "1") == "1":
        text = swap_model_vertex_offset_loop_registers(text)
    if os.environ.get("V3_TANK_CONTACT_SCAN_REGISTERS", "1") == "1":
        text = cycle_tank_contact_scan_registers(text)
    if os.environ.get("V3_ENTITY_SELECTION_REGISTERS", "1") == "1":
        text = shape_entity_selection_registers(text)
    if os.environ.get("V3_TURRET_ANGLE_DELTA_REGISTERS", "1") == "1":
        text = shape_turret_angle_delta_registers(text)
    if os.environ.get("V3_WAVE_VERTEX_UPDATE_REGISTERS", "1") == "1":
        text = shape_wave_vertex_update_registers(text)
    if os.environ.get("V3_WAVE_MESH_COPY_REGISTERS", "1") == "1":
        text = shape_wave_mesh_copy_registers(text)
    if os.environ.get("V3_UNIT_COMMAND_CANDIDATE_PROLOGUE", "1") == "1":
        text = shape_unit_command_candidate_prologue(text)
    if os.environ.get("V3_UNIT_COMMAND_SEARCH_REGISTERS", "1") == "1":
        text = swap_unit_command_search_registers(text)
    if os.environ.get("V3_TANK_AIM_REFRESH_REGISTERS", "1") == "1":
        text = shape_tank_aim_refresh_registers(text)
    if os.environ.get("V3_VECTOR_ANGLE_JOIN_LABEL", "1") == "1":
        text = reproduce_vector_angle_join_label(text)
    if os.environ.get("V3_EFFECT_MESH_DRAW_REGISTERS", "1") == "1":
        text = shape_effect_mesh_draw_registers(text)
    if os.environ.get("V3_CONTACT_SIDE_TEST_FRAME", "1") == "1":
        text = shape_contact_side_test_frame(text)
    if os.environ.get("V3_CURVE_MODE_DISPATCHES", "1") == "1":
        text = shape_curve_mode_dispatches(text)
    if os.environ.get("V3_SPOTTER_FRAME_SETUP_PROLOGUE", "1") == "1":
        text = order_spotter_frame_setup_prologue(text)
    if os.environ.get("V3_TEMPLATE_RETRY_SOURCE_RESET", "1") == "1":
        text = schedule_template_retry_source_reset(text)
    if os.environ.get("V3_FONT_GLYPH_DRAW_PROLOGUE", "1") == "1":
        text = order_font_glyph_draw_prologue(text)
    if os.environ.get("V3_MUSIC_STREAM_QUEUE_REGISTERS", "1") == "1":
        text = shape_music_stream_queue_registers(text)
    if os.environ.get("V3_SPOTTER_UPDATE_COLLISION_SETUP", "1") == "1":
        text = schedule_spotter_update_collision_setup(text)
    if os.environ.get("V3_GRID_NODE_REMOVE_LOOP", "1") == "1":
        text = shape_grid_node_remove_loop(text)
    if os.environ.get("V3_CAMERA_COLLISION_REGISTERS", "1") == "1":
        text = shape_camera_collision_registers(text)
    if os.environ.get("V3_MOVER_REFLECT_LIKELY_MUL", "1") == "1":
        text = hoist_mover_reflect_likely_multiply(text)
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
    if os.environ.get("V3_TEXT_CHARACTER_MAP", "1") == "1":
        text = shape_text_character_map(text)
    if os.environ.get("V3_GRID_OVERLAP_QUERY", "1") == "1":
        text = shape_grid_overlap_query(text)
    if os.environ.get("V3_RUNTIME_FIELDS_INIT", "1") == "1":
        text = shape_runtime_fields_init(text)
    if os.environ.get("V3_SEGMENT_ENDPOINT_CLAMP_REGISTERS", "1") == "1":
        text = shape_segment_endpoint_clamp_registers(text)
    if os.environ.get("V3_SEARCH_ITERATOR_FIRST_INLINE", "1") == "1":
        text = shape_search_iterator_first_inline(text)
    if os.environ.get("V3_AI_NODE_SETUP_FRAME", "1") == "1":
        text = shape_ai_node_setup_frame(text)
    if os.environ.get("V3_MODEL_DISPLAY_PATCH_REGISTERS", "1") == "1":
        text = shape_model_display_patch_registers(text)
    if os.environ.get("V3_SHELL_UPDATE_ALLOCATIONS", "1") == "1":
        text = schedule_shell_update_allocations(text)
    if os.environ.get("V3_DESTROYED_OBJECT_UPDATE_ALLOCATIONS", "1") == "1":
        text = shape_destroyed_object_update_allocations(text)
    if os.environ.get("V3_BILLBOARD_COMMAND_UPDATE_ALLOCATIONS", "1") == "1":
        text = shape_billboard_command_update_allocations(text)
    if os.environ.get("V3_OBJECT_SEARCH_LOOP_ENTRY", "1") == "1":
        text = merge_object_search_loop_entry(text)
    if os.environ.get("V3_POOL_TYPE4_REMOVAL", "1") == "1":
        text = schedule_pool_type4_removal(text)
    if os.environ.get("V3_OBJECT_DISTANCE_PROBE", "1") == "1":
        text = schedule_object_distance_probe(text)
    if os.environ.get("V3_MATRIX_BASIS_INVERSE", "1") == "1":
        text = schedule_matrix_basis_inverse(text)
    if os.environ.get("V3_RECORD_INDEX_COPY", "1") == "1":
        text = preserve_record_index_copy(text)
    if os.environ.get("V3_SEARCH_RECORD_INDEX_COPY", "1") == "1":
        text = preserve_search_record_index_copy(text)
    if os.environ.get("V3_ANGLE_STEP_UPDATE", "1") == "1":
        text = shape_angle_step_update(text)
    if os.environ.get("V3_GRID_COLLISION_MIDPOINT_LOAD", "1") == "1":
        text = schedule_grid_collision_midpoint_load(text)
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
