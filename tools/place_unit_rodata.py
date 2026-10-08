#!/usr/bin/env python3
"""Move listed C units' .rodata out of the splat main segment into fixed-VMA sections.

Reads the splat-generated linker script and config/us/unit_rodata.tsv and writes
a derived script. Each listed unit's `<obj>(.rodata);` line is removed from the
.main output section and re-emitted as its own output section at the original
rodata VRAM. Those sections get distinct LMAs far outside the ROM image so they
neither overlap .main's LMA nor reach the objcopy binary (they are removed with
-R and spliced back by splice_unit_rodata.py).
"""

from __future__ import annotations

import argparse
from pathlib import Path

MAIN_VRAM = 0x80071000
MAIN_ROM = 0x1000
CODE_END = 0x101000
LMA_PARK = 0x10000000


def load_table(path: Path) -> list[tuple[str, int, int]]:
    rows = []
    for line in path.read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        unit, vram, size = line.split()
        rows.append((unit, int(vram, 0), int(size, 0)))
    rows.sort(key=lambda row: row[1])
    for (u0, v0, s0), (u1, v1, _) in zip(rows, rows[1:]):
        if v0 + s0 > v1:
            raise SystemExit(f"rodata ranges overlap: {u0} and {u1}")
    for unit, vram, size in rows:
        rom = vram - MAIN_VRAM + MAIN_ROM
        if not (MAIN_ROM <= rom and rom + size <= CODE_END) or vram % 4 or size <= 0:
            raise SystemExit(f"bad rodata placement for {unit}")
    return rows


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("linker_script", type=Path)
    parser.add_argument("table", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    rows = load_table(args.table)
    text = args.linker_script.read_text().splitlines(keepends=True)
    extra = []
    for index, (unit, vram, _size) in enumerate(rows):
        needle = f"build/us/src/code/{unit}.c.o(.rodata);"
        hits = [i for i, line in enumerate(text) if line.strip() == needle]
        if len(hits) != 1:
            raise SystemExit(f"expected one rodata line for {unit}, found {len(hits)}")
        del text[hits[0]]
        extra.append(
            # This section deliberately aliases .main's VMA range, so the link
            # uses --no-check-sections; splice_unit_rodata.py re-runs the
            # overlap check for every other allocated section.
            f"    .unit_rodata_{unit} 0x{vram:08X} : AT(0x{LMA_PARK + index * 0x10000:08X})\n"
            f"    {{\n        {needle}\n    }}\n"
        )
    # Insert before /DISCARD/, which would otherwise swallow the sections.
    hits = [i for i, line in enumerate(text) if line.strip().startswith("/DISCARD/")]
    if len(hits) != 1:
        raise SystemExit("expected exactly one /DISCARD/ statement")
    text[hits[0]:hits[0]] = extra
    args.output.write_text("".join(text))


if __name__ == "__main__":
    main()
