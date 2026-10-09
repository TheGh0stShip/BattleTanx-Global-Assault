#!/usr/bin/env python3
"""Reproduce the retail assembler's unfilled libgcc return delay slot."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


EPILOGUE = re.compile(
    r"(?m)^(\s*addu\s+\$sp,\$sp,8\s*\n)(\s*j\s+\$31\s*\n)"
)


def normalize(source: str) -> str:
    if "__udivmoddi4:" not in source:
        raise RuntimeError("expected __udivmoddi4 label")

    output, fires = EPILOGUE.subn(
        r"\1\t.set\tnoreorder\n\2\tnop\n\t.set\treorder\n",
        source,
    )
    if fires != 1:
        raise RuntimeError(
            f"__udivmoddi4 return rewrite fired {fires} times (expected 1)"
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
