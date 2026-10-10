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
FUNCTION_RECORDS_START = 0xB9292
FUNCTION_RECORD_CLASS = 0x0C
TYPE_ONLY_RECORD_CLASS = 0x0E
FUNCTION_TEXT_BASE = 0x8007D720
IDENTIFIER = re.compile(rb"[A-Za-z_.$][A-Za-z0-9_.$]*")


def inventory_function_records(rom: bytes) -> list[dict]:
    """Decode the one proven fixed-width record chain in the stale table.

    The table contains several record classes with different tails.  At
    FUNCTION_RECORDS_START, class 0x0c records have the unambiguous layout
    below.  Stop at the first other class rather than guessing its format.

        u8 name_size; char name[name_size];
        u8 record_class; u16 type_index; u16 section; u32 value;

    The numeric fields are little-endian even though the N64 ROM is otherwise
    big-endian.  These records are compiler/debugger metadata, not target data.
    """

    records = []
    offset = FUNCTION_RECORDS_START
    while offset < END:
        record_offset = offset
        name_size = rom[offset]
        name_start = offset + 1
        name_end = name_start + name_size
        if not 3 <= name_size <= 127 or name_end + 9 > END:
            break
        encoded = rom[name_start:name_end]
        if not IDENTIFIER.fullmatch(encoded):
            break
        record_class = rom[name_end]
        if record_class != FUNCTION_RECORD_CLASS:
            break
        records.append(
            {
                "record_offset": record_offset,
                "name_offset": name_start,
                "name": encoded.decode("ascii"),
                "record_class": record_class,
                "type_index": int.from_bytes(rom[name_end + 1 : name_end + 3], "little"),
                "section": int.from_bytes(rom[name_end + 3 : name_end + 5], "little"),
                "value": int.from_bytes(rom[name_end + 5 : name_end + 9], "little"),
            }
        )
        records[-1]["retail_address"] = FUNCTION_TEXT_BASE + records[-1]["value"]
        records[-1]["legacy_symbol"] = f"func_{records[-1]['retail_address']:08X}"
        offset = name_end + 9
    return records


def inventory_type_only_records(rom: bytes, offset: int) -> list[dict]:
    """Decode the short class-0x0e chain following the function records.

    These records prove a name-to-type-index association, but contain no
    section or value.  Keep the deliberately neutral name "type-only" until
    the producer's exact class semantics are established.
    """

    records = []
    while offset < END:
        record_offset = offset
        name_size = rom[offset]
        name_start = offset + 1
        name_end = name_start + name_size
        if not 3 <= name_size <= 127 or name_end + 3 > END:
            break
        encoded = rom[name_start:name_end]
        if not IDENTIFIER.fullmatch(encoded):
            break
        record_class = rom[name_end]
        if record_class != TYPE_ONLY_RECORD_CLASS:
            break
        records.append(
            {
                "record_offset": record_offset,
                "name_offset": name_start,
                "name": encoded.decode("ascii"),
                "record_class": record_class,
                "type_index": int.from_bytes(rom[name_end + 1 : name_end + 3], "little"),
            }
        )
        offset = name_end + 3
    return records


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
    function_records = inventory_function_records(rom)
    expected_functions = {
        "Steps_PruneFork",
        "Steps_InitStep_Leg",
        "Steps_NewStep",
        "Steps_InitStepPool",
        "Steps_FreeBranch",
        "Steps_CopyObstacleRef",
        "Steps_GetNextStepId",
        "Steps_PruneForksBelow",
        "Steps_GetNextLegPtr",
        "Steps_InitStep_Free",
        "Steps_StepPtrFromId",
        "Steps_SpliceIn",
        "Steps_FreeStep",
        "Steps_InitStep_Fork",
        "Steps_InitStep",
        "Steps_CropLinearBranch",
    }
    actual_functions = {item["name"] for item in function_records}
    if actual_functions != expected_functions:
        raise ValueError(
            "unexpected stale debug function-record chain: "
            f"expected {sorted(expected_functions)}, got {sorted(actual_functions)}"
        )
    function_chain_end = function_records[-1]["record_offset"]
    function_chain_end += 1 + len(function_records[-1]["name"]) + 9
    type_only_records = inventory_type_only_records(rom, function_chain_end)
    expected_type_only = {
        "Steps_InitStep_Arrival": 27,
        "Obstacles_InitObstacleRef": 29,
        "Obstacles_CopyObstacleRef": 28,
    }
    if {item["name"]: item["type_index"] for item in type_only_records} != expected_type_only:
        raise ValueError("unexpected stale debug class-0x0e record chain")
    return {
        "format": "BattleTanx Global Assault stale debug-name inventory v1",
        "rom_start": START,
        "rom_end": END,
        "size": END - START,
        "sha256": hashlib.sha256(rom[START:END]).hexdigest(),
        "record_count": len(records),
        "unique_name_count": len(unique),
        "function_record_format": {
            "start": FUNCTION_RECORDS_START,
            "record_class": FUNCTION_RECORD_CLASS,
            "endianness": "little",
            "fields": [
                "u8 name_size",
                "char name[name_size]",
                "u8 record_class",
                "u16 type_index",
                "u16 section",
                "u32 value",
            ],
            "scope": "proven contiguous class-0x0c chain only",
            "retail_text_base": FUNCTION_TEXT_BASE,
            "anchor_evidence": (
                "all 16 section-relative values uniquely coincide with catalogued "
                "retail function boundaries when based at 0x8007d720"
            ),
        },
        "function_record_count": len(function_records),
        "function_records": function_records,
        "type_only_record_format": {
            "record_class": TYPE_ONLY_RECORD_CLASS,
            "endianness": "little",
            "fields": [
                "u8 name_size",
                "char name[name_size]",
                "u8 record_class",
                "u16 type_index",
            ],
            "scope": "proven contiguous class-0x0e chain only; semantics unknown",
        },
        "type_only_record_count": len(type_only_records),
        "type_only_records": type_only_records,
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
