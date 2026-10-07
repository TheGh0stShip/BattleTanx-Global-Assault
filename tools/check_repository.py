#!/usr/bin/env python3
"""Fail when private/generated artifacts or host-specific paths are tracked."""

from __future__ import annotations

import re
import subprocess

FORBIDDEN_SUFFIXES = {
    ".z64", ".n64", ".v64", ".elf", ".vpk", ".self", ".velf", ".psp2dmp"
}
FORBIDDEN_PATTERNS = (
    re.compile(rb"/home/[A-Za-z0-9._-]+/"),
    re.compile(rb"[A-Za-z]:\\Users\\[A-Za-z0-9._-]+\\"),
    re.compile(rb"gh[opsu]_[A-Za-z0-9_]+"),
)


def main() -> None:
    tracked = subprocess.check_output(["git", "ls-files", "-z"]).split(b"\0")
    failures: list[str] = []
    for raw_name in filter(None, tracked):
        name = raw_name.decode()
        if any(name.lower().endswith(suffix) for suffix in FORBIDDEN_SUFFIXES):
            failures.append(f"forbidden tracked artifact: {name}")
            continue
        data = open(name, "rb").read()
        if any(pattern.search(data) for pattern in FORBIDDEN_PATTERNS):
            failures.append(f"private path or token pattern: {name}")
    if failures:
        raise SystemExit("\n".join(failures))
    print(f"Repository safety check passed ({len(tracked)} tracked files)")


if __name__ == "__main__":
    main()
