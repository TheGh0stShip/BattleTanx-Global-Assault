#!/usr/bin/env python3
"""Verify the stale, unlinked Steps object fragment retained in the ROM."""

from __future__ import annotations

import hashlib


STALE_START = 0xB8400
STALE_CODE_START = 0xB8402
STALE_CODE_END = 0xB8BF6
LIVE_START = 0xD96E
LIVE_CODE_START = 0xD970
DEBUG_LINES_START = STALE_CODE_END
DEBUG_SYMBOLS_START = 0xB9292
END = 0xC0000


def inventory_stale_object(rom: bytes) -> dict:
    if rom[STALE_START:STALE_CODE_START] != rom[LIVE_START:LIVE_CODE_START]:
        raise ValueError("stale Steps fragment prefix no longer matches live text")

    exact_words = 0
    relocation_words = []
    live_offset = LIVE_CODE_START
    for stale_offset in range(STALE_CODE_START, STALE_CODE_END, 4):
        stale_word = int.from_bytes(rom[stale_offset : stale_offset + 4], "big")
        live_word = int.from_bytes(rom[live_offset : live_offset + 4], "big")
        if stale_word == live_word:
            exact_words += 1
        else:
            stale_opcode = stale_word >> 26
            live_opcode = live_word >> 26
            if (
                stale_opcode in (2, 3)
                and live_opcode == stale_opcode
                and stale_word & 0x03FFFFFF == 0
            ):
                kind = "jump-target"
            elif stale_word >> 16 == live_word >> 16 and stale_word & 0xFFFF == 0:
                kind = "immediate"
            else:
                raise ValueError(
                    "unrecognised stale Steps text difference at "
                    f"ROM {stale_offset:#x}: {stale_word:08x} != {live_word:08x}"
                )
            relocation_words.append(
                {
                    "stale_rom_offset": stale_offset,
                    "live_rom_offset": live_offset,
                    "kind": kind,
                    "stale_word": f"{stale_word:08x}",
                    "live_word": f"{live_word:08x}",
                }
            )
        live_offset += 4

    if exact_words != 438 or len(relocation_words) != 71:
        raise ValueError(
            "unexpected stale Steps comparison totals: "
            f"{exact_words} exact, {len(relocation_words)} relocations"
        )
    return {
        "format": "BattleTanx Global Assault stale relocatable object inventory v1",
        "stale_text_start": STALE_START,
        "stale_text_end": STALE_CODE_END,
        "stale_text_size": STALE_CODE_END - STALE_START,
        "live_text_start": LIVE_START,
        "live_text_end": LIVE_CODE_START + (STALE_CODE_END - STALE_CODE_START),
        "leading_exact_bytes": STALE_CODE_START - STALE_START,
        "instruction_words": exact_words + len(relocation_words),
        "exact_instruction_words": exact_words,
        "relocation_words": relocation_words,
        "debug_lines_start": DEBUG_LINES_START,
        "debug_lines_end": DEBUG_SYMBOLS_START,
        "debug_lines_size": DEBUG_SYMBOLS_START - DEBUG_LINES_START,
        "debug_symbols_start": DEBUG_SYMBOLS_START,
        "debug_symbols_end": END,
        "debug_symbols_size": END - DEBUG_SYMBOLS_START,
        "sha256": hashlib.sha256(rom[STALE_START:END]).hexdigest(),
    }
