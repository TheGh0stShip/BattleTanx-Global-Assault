#!/usr/bin/env python3
"""Rebuild split BattleTanx: Global Assault script/cutscene assets."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

from inventory_script_assets import parse_script


def rebuild_script(directory: Path, metadata: dict) -> bytes:
    header = directory.joinpath("header.bin").read_bytes()
    if len(header) != 4:
        raise ValueError(f"{directory}/header.bin must be exactly four bytes")
    expected_count = metadata["stream_count"]
    if int.from_bytes(header[2:4], "big") != expected_count:
        raise ValueError(f"{directory}/header.bin stream count changed unexpectedly")
    streams = []
    for index in range(expected_count):
        streams.append(directory.joinpath(f"stream_{index:03d}.bin").read_bytes())
    payload = header + b"".join(streams)
    parsed = parse_script(payload)
    if parsed["stream_count"] != expected_count:
        raise ValueError(f"{directory} rebuilt with the wrong stream count")
    return payload


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", type=Path, help="script inventory JSON")
    parser.add_argument("source", type=Path, help="directory of split scripts")
    parser.add_argument("output", type=Path, help="directory for rebuilt scripts")
    parser.add_argument(
        "--require-original",
        action="store_true",
        help="reject rebuilt scripts that differ from their retail hashes",
    )
    args = parser.parse_args()

    report = json.loads(args.manifest.read_text())
    if report.get("format") != "BattleTanx Global Assault script inventory v1":
        raise SystemExit("unsupported script manifest")
    args.output.mkdir(parents=True, exist_ok=True)
    rebuilt = []
    for script in report["scripts"]:
        name = f"script_{script['index']:02d}_{script['rom_start']:06X}"
        payload = rebuild_script(args.source / name, script)
        digest = hashlib.sha256(payload).hexdigest()
        if args.require_original and digest != script["sha256"]:
            raise SystemExit(f"rebuilt {name} differs from the retail hash")
        filename = f"{name}.bin"
        args.output.joinpath(filename).write_bytes(payload)
        rebuilt.append({"name": name, "file": filename, "size": len(payload), "sha256": digest})
    args.output.joinpath("manifest.json").write_text(
        json.dumps({"format": "BattleTanx Global Assault rebuilt scripts v1", "scripts": rebuilt}, indent=2)
        + "\n"
    )


if __name__ == "__main__":
    main()
