#!/usr/bin/env python3
"""Rebuild raw world pools from extracted chunks and unreferenced gaps."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

from inventory_world_pools import chunk_filename, gap_filename


class PoolPackError(ValueError):
    """Raised when split pool inputs cannot form one unambiguous byte stream."""


def rebuild_pool(pool: dict, source: Path) -> bytes:
    output = bytearray(pool["size"])
    written = bytearray(pool["size"])

    def insert(path: Path, offset: int, size: int) -> None:
        payload = path.read_bytes()
        if len(payload) != size:
            raise PoolPackError(f"{path} is {len(payload)} bytes, expected {size}")
        if offset < 0 or offset + size > len(output):
            raise PoolPackError(f"{path} lies outside the {pool['name']} pool")
        for index, value in enumerate(payload, offset):
            if written[index] and output[index] != value:
                raise PoolPackError(
                    f"{path} conflicts with another input at pool offset 0x{index:X}"
                )
            output[index] = value
            written[index] = 1

    for chunk in pool["chunks"]:
        insert(source / chunk_filename(chunk), chunk["pool_offset"], chunk["size"])
    gap_source = source / "unreferenced"
    for index, gap in enumerate(pool["gaps"]):
        insert(gap_source / gap_filename(index, gap), gap["pool_offset"], gap["size"])
    try:
        missing = written.index(0)
    except ValueError:
        missing = -1
    if missing >= 0:
        raise PoolPackError(
            f"split inputs leave the {pool['name']} pool uncovered at 0x{missing:X}"
        )
    return bytes(output)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", type=Path, help="world-pool inventory JSON")
    parser.add_argument("source", type=Path, help="directory of extracted split pools")
    parser.add_argument("output", type=Path, help="directory for rebuilt pool files")
    parser.add_argument(
        "--require-original",
        action="store_true",
        help="reject rebuilt bytes that differ from the retail pool hashes",
    )
    args = parser.parse_args()

    document = json.loads(args.manifest.read_text())
    if document.get("format") != "BattleTanx Global Assault world pool inventory v1":
        raise SystemExit("unsupported world-pool manifest")
    args.output.mkdir(parents=True, exist_ok=True)
    results = []
    for pool in document["pools"]:
        payload = rebuild_pool(pool, args.source / pool["name"])
        digest = hashlib.sha256(payload).hexdigest()
        if args.require_original and digest != pool["sha256"]:
            raise SystemExit(f"rebuilt {pool['name']} pool differs from the retail hash")
        filename = f"{pool['name']}.bin"
        args.output.joinpath(filename).write_bytes(payload)
        results.append(
            {"name": pool["name"], "file": filename, "size": len(payload), "sha256": digest}
        )
    args.output.joinpath("manifest.json").write_text(
        json.dumps({"format": "BattleTanx Global Assault rebuilt world pools v1", "pools": results}, indent=2)
        + "\n"
    )


if __name__ == "__main__":
    main()
