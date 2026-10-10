#!/usr/bin/env python3
"""Rebuild a script/cutscene asset from inventory_script_assets JSON."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from inventory_script_assets import build_script


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", type=Path, help="single script JSON document")
    parser.add_argument("output", type=Path, help="rebuilt binary script")
    args = parser.parse_args()

    payload = build_script(json.loads(args.manifest.read_text()))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(payload)


if __name__ == "__main__":
    main()
