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
    parser.add_argument("--manual", type=Path)
    args = parser.parse_args()

    document = tomllib.loads(args.boundaries.read_text())
    functions = {
        function["name"]: (function["vram"], function["size"])
        for section in document["section"]
        for function in section.get("functions", ())
    }
    manual: dict[str, dict[str, object]] = {}
    if args.manual is not None:
        manual_document = tomllib.loads(args.manual.read_text())
        manual = {entry["name"]: entry for entry in manual_document["symbol"]}
    symbols: dict[str, int] = {}
    for raw_line in args.symbols.read_text().splitlines():
        match = SYMBOL_RE.fullmatch(raw_line.strip())
        if match is not None:
            symbols[match.group(1)] = int(match.group(2), 16)
    symbols.update({name: values[0] for name, values in functions.items()})
    symbols.update({name: int(entry["vram"]) for name, entry in manual.items()})

    names_by_address: dict[int, list[str]] = {}
    for name, address in symbols.items():
        names_by_address.setdefault(address, []).append(name)

    output: list[str] = []
    selected: list[tuple[str, int]] = []
    for address, names in names_by_address.items():
        manual_names = [name for name in names if name in manual]
        function_names = [name for name in names if name in functions]
        candidates = manual_names or function_names or names
        name = min(candidates, key=lambda value: (value.startswith("func_"), value))
        selected.append((name, address))

    for name, address in sorted(selected, key=lambda item: (item[1], item[0])):
        line = f"{name} = 0x{address:08X};"
        if name in functions:
            line += f" // type:func size:0x{functions[name][1]:X}"
        elif name in manual:
            entry = manual[name]
            line += f" // type:{entry['type']} size:0x{int(entry['size']):X}"
        output.append(line)

    args.output.write_text("\n".join(output) + "\n")
    print(f"Wrote {len(output)} symbols ({len(functions)} functions)")


if __name__ == "__main__":
    main()
