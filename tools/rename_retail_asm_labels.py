#!/usr/bin/env python3
"""Rename generated asm definitions shadowed by typed C rodata objects."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


SYMBOL_RE = re.compile(r"(?:D|jtbl)_[0-9A-Fa-f]{8}")
LABEL_RE = re.compile(
    r"^(glabel|dlabel|endlabel|enddlabel) ((?:D|jtbl)_[0-9A-Fa-f]{8})$",
    re.MULTILINE,
)


def manifest_symbols(manifest: Path, source_dir: Path) -> set[str]:
    symbols: set[str] = set()
    for raw_line in manifest.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        unit = line.split("\t", 1)[0]
        if unit == "unit":
            continue
        source = source_dir / f"{unit}.c"
        symbols.update(SYMBOL_RE.findall(source.read_text()))
    return symbols


def rewrite_file(path: Path, symbols: set[str]) -> bool:
    original = path.read_text()

    def replace(match: re.Match[str]) -> str:
        directive, symbol = match.groups()
        if symbol not in symbols:
            return match.group(0)
        return f"{directive} __retail_{symbol}"

    rewritten = LABEL_RE.sub(replace, original)
    if rewritten == original:
        return False
    path.write_text(rewritten)
    return True


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("manifest", type=Path)
    parser.add_argument("source_dir", type=Path)
    parser.add_argument("asm_dir", type=Path)
    args = parser.parse_args()

    symbols = manifest_symbols(args.manifest, args.source_dir)
    changed = 0
    for path in sorted(args.asm_dir.glob("*.s")):
        changed += rewrite_file(path, symbols)
    print(f"Renamed typed-rodata definitions in {changed} generated asm files")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
