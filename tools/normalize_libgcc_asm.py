#!/usr/bin/env python3
"""Reproduce the retail assembler's unfilled libgcc return delay slots."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


EPILOGUE = re.compile(
    r"(?m)^(\s*addu\s+\$sp,\$sp,(?:8|32|40)\s*\n)(\s*j\s+\$31\s*\n)"
)

SUPPORTED = ("__floatdisf", "__udivdi3", "__udivmoddi4", "__umoddi3")


def expand_floatdisf_double_loads(source: str) -> str:
    """Use the retail big-endian two-word expansion of three ``l.d`` macros."""
    pattern = re.compile(r"(?m)^\s*l\.d\s+\$f0,(\$LF_lis[012])\s*$")
    output, fires = pattern.subn(
        lambda match: (
            f"\tlui\t$1,%hi({match.group(1)})\n"
            f"\tlwc1\t$f1,%lo({match.group(1)})($1)\n"
            f"\tlwc1\t$f0,%lo({match.group(1)}+4)($1)"
        ),
        source,
    )
    if fires != 3:
        raise RuntimeError(
            f"__floatdisf double-load expansion fired {fires} times (expected 3)"
        )
    return output


def normalize(source: str) -> str:
    labels = [name for name in SUPPORTED if name + ":" in source]
    if len(labels) != 1:
        raise RuntimeError("expected exactly one supported libgcc function label")

    if labels[0] == "__floatdisf":
        return expand_floatdisf_double_loads(source)

    output, fires = EPILOGUE.subn(
        r"\1\t.set\tnoreorder\n\2\tnop\n\t.set\treorder\n",
        source,
    )
    if fires != 1:
        raise RuntimeError(
            f"{labels[0]} return rewrite fired {fires} times (expected 1)"
        )
    return output


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    args.output.write_text(normalize(args.input.read_text()))


if __name__ == "__main__":
    main()
