#!/usr/bin/env python3
"""Build and compress a seven-component BattleTanx asset bundle."""

from __future__ import annotations

import argparse
import struct
from pathlib import Path

from inventory_lzari_assets import bundle_layout
from lzari import compress


def pack_bundle(components: list[bytes]) -> bytes:
    if len(components) != 7:
        raise ValueError("a BattleTanx bundle requires exactly seven components")
    offsets = [0x20]
    for component in components:
        offsets.append(offsets[-1] + len(component))
    decoded = struct.pack(">8I", *offsets) + b"".join(components)
    if bundle_layout(decoded) is None:
        raise ValueError("components do not satisfy the proven BattleTanx layout")
    return decoded


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("components", type=Path, nargs=7)
    parser.add_argument("output", type=Path, help="compressed LZARI output")
    parser.add_argument("--decoded-output", type=Path)
    args = parser.parse_args()

    decoded = pack_bundle([path.read_bytes() for path in args.components])
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(compress(decoded))
    if args.decoded_output:
        args.decoded_output.parent.mkdir(parents=True, exist_ok=True)
        args.decoded_output.write_bytes(decoded)


if __name__ == "__main__":
    main()
