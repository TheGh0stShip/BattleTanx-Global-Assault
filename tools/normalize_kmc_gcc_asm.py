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
from pathlib import Path


TRANSFER = re.compile(
    r"^(?:b(?:al|eq|ne|gez|gezal|gtz|lez|ltz|ltzal|c1[ft]|c1fl|c1tl)?|"
    r"j|jal|jr|jalr)\s+"
)


def normalize(source: str) -> str:
    # GNU as starts in reorder mode. Merely adding an explicit NOP does not
    # prevent it from moving an earlier instruction into that slot.
    output: list[str] = ["\t.set\tnoreorder\n"]
    compiler_reorder = True

    for line in source.splitlines(keepends=True):
        stripped = line.strip()

        if stripped == ".set\tnoreorder" or stripped == ".set noreorder":
            compiler_reorder = False
            output.append(line)
            continue

        if stripped == ".set\treorder" or stripped == ".set reorder":
            compiler_reorder = True
            newline = "\n" if line.endswith("\n") else ""
            output.append("\t.set\tnoreorder" + newline)
            continue

        output.append(line)
        instruction = stripped.split("#", 1)[0].strip()
        # GCC's own instructions are tab-indented. Continuation lines from an
        # inline-assembly block are not; those blocks already spell out their
        # intended delay slots and must remain byte-for-byte intact.
        if compiler_reorder and line.startswith("\t") and TRANSFER.match(instruction):
            newline = "\n" if line.endswith("\n") else ""
            output.append("\tnop" + newline)

    return "".join(output)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    args.output.write_text(normalize(args.input.read_text()))


if __name__ == "__main__":
    main()
