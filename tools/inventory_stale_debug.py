#!/usr/bin/env python3
"""Inventory the compact debug-name table left in the retail ROM."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path

from inventory_lzari_assets import ROM_SHA1


START = 0xB8400
END = 0xC0000
IDENTIFIER = re.compile(rb"[A-Za-z_.$][A-Za-z0-9_.$]*")


def inventory_debug_names(rom: bytes) -> dict:
    records = []
    for offset in range(START, END):
        size = rom[offset]
        if not 3 <= size <= 127 or offset + 1 + size > END:
            continue
        encoded = rom[offset + 1 : offset + 1 + size]
        if IDENTIFIER.fullmatch(encoded):
            records.append(
                {"rom_offset": offset + 1, "name": encoded.decode("ascii")}
            )
    unique = sorted({item["name"] for item in records})
    required = {
        "Steps_PruneFork",
        "Steps_InitStepPool",
        "Obstacles_InitObstacleRef",
        ".0fake",
        ".eos",
        "OSThread_s",
        "AL_MIDI_Continue",
    }
    missing = required - set(unique)
    if missing:
        raise ValueError(f"stale debug table is missing expected names: {sorted(missing)}")
    return {
        "format": "BattleTanx Global Assault stale debug-name inventory v1",
        "rom_start": START,
        "rom_end": END,
        "size": END - START,
        "sha256": hashlib.sha256(rom[START:END]).hexdigest(),
        "record_count": len(records),
        "unique_name_count": len(unique),
        "records": records,
        "unique_names": unique,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument("--output", type=Path, help="write inventory JSON")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    digest = hashlib.sha1(rom).hexdigest()
    if digest != ROM_SHA1:
        raise SystemExit(f"base ROM SHA-1 mismatch: expected {ROM_SHA1}, got {digest}")
    report = {"rom_sha1": digest, **inventory_debug_names(rom)}
    encoded = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
