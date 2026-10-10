#!/usr/bin/env python3
"""Rebuild and LZARI-compress a world emitted by extract_lzari_worlds.py."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from btga_world import build_world
from lzari import compress


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", type=Path, help="world.json to rebuild")
    parser.add_argument("output", type=Path, help="compressed LZARI output")
    parser.add_argument("--decoded-output", type=Path)
    args = parser.parse_args()

    decoded = build_world(json.loads(args.manifest.read_text()))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(compress(decoded))
    if args.decoded_output:
        args.decoded_output.parent.mkdir(parents=True, exist_ok=True)
        args.decoded_output.write_bytes(decoded)


if __name__ == "__main__":
    main()
