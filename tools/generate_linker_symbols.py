#!/usr/bin/env python3
"""Convert splat symbol files into non-overriding linker-script symbols."""

from __future__ import annotations

import argparse
import re
from pathlib import Path

SYMBOL_RE = re.compile(
    r"^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+|[0-9]+);"
)
ADDRESS_NAME_RE = re.compile(r"^(?:D|func)_([0-9A-Fa-f]{8})$")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("output", type=Path)
    parser.add_argument("inputs", nargs="+", type=Path)
    parser.add_argument("--symbol", action="append", default=[])
    parser.add_argument("--undefined-list", type=Path)
    args = parser.parse_args()

    symbols: dict[str, int] = {}
    for path in args.inputs:
        for line in path.read_text().splitlines():
            match = SYMBOL_RE.match(line.strip())
            if match is not None:
                symbols[match.group(1)] = int(match.group(2), 0)
    for definition in args.symbol:
        name, value = definition.split("=", 1)
        symbols[name] = int(value, 0)
    if args.undefined_list is not None:
        for line in args.undefined_list.read_text().splitlines():
            if not line.split():
                continue
            name = line.split()[-1]
            match = ADDRESS_NAME_RE.fullmatch(name)
            if match is not None:
                symbols[name] = int(match.group(1), 16)

    lines = [
        f"PROVIDE({name} = 0x{value:08X});"
        for name, value in sorted(symbols.items())
    ]
    args.output.write_text("\n".join(lines) + "\n")
    print(f"Wrote {len(lines)} fallback linker symbols")


if __name__ == "__main__":
    main()
