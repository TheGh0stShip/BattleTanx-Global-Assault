#!/usr/bin/env python3
"""Annotate seed symbols using btgarecomp's refined function boundaries."""

from __future__ import annotations

import argparse
import re
import tomllib
from pathlib import Path

SYMBOL_RE = re.compile(r"^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+);$")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("boundaries", type=Path)
    parser.add_argument("symbols", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    document = tomllib.loads(args.boundaries.read_text())
    functions = {
        function["name"]: (function["vram"], function["size"])
        for section in document["section"]
        for function in section.get("functions", ())
    }

    symbols: dict[str, int] = {}
    for raw_line in args.symbols.read_text().splitlines():
        match = SYMBOL_RE.fullmatch(raw_line.strip())
        if match is not None:
            symbols[match.group(1)] = int(match.group(2), 16)
    symbols.update({name: values[0] for name, values in functions.items()})

    names_by_address: dict[int, list[str]] = {}
    for name, address in symbols.items():
        names_by_address.setdefault(address, []).append(name)

    output: list[str] = []
    selected: list[tuple[str, int]] = []
    for address, names in names_by_address.items():
        function_names = [name for name in names if name in functions]
        candidates = function_names or names
        name = min(candidates, key=lambda value: (value.startswith("func_"), value))
        selected.append((name, address))

    for name, address in sorted(selected, key=lambda item: (item[1], item[0])):
        line = f"{name} = 0x{address:08X};"
        if name in functions:
            line += f" // type:func size:0x{functions[name][1]:X}"
        output.append(line)

    args.output.write_text("\n".join(output) + "\n")
    print(f"Wrote {len(output)} symbols ({len(functions)} functions)")


if __name__ == "__main__":
    main()
