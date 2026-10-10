#!/usr/bin/env python3
"""Validate and inventory BattleTanx: Global Assault's libmus assets.

The program reads metadata only. It never writes sample or sequence payloads;
an optional JSON report records offsets, sizes, counts, and hashes so the ROM
layout can be audited without committing extracted copyrighted material.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path


ROM_SHA1 = "805248fb0a0ee694cad8d7dc927b631d860dd8cf"
FILE_TABLE_ROM = 0xA4710
FILE_COUNT = 26
CART_BASE = 0xB0000000
PTR_MAGIC = b"N64 PtrTablesV2\0"
WAVE_MAGIC = b"N64 WaveTables \0"


class AudioInventoryError(ValueError):
    """The supplied ROM does not contain the expected audio layout."""


def be_u16(data: bytes, offset: int) -> int:
    return struct.unpack_from(">H", data, offset)[0]


def be_u32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def require(condition: bool, message: str) -> None:
    if not condition:
        raise AudioInventoryError(message)


def relative_range(offset: int, size: int, length: int, what: str) -> None:
    require(offset >= 0 and size >= 0 and offset + size <= length,
            f"{what} lies outside its file")


def read_file_table(rom: bytes) -> list[dict]:
    files = []
    for index in range(FILE_COUNT):
        address = be_u32(rom, FILE_TABLE_ROM + index * 8)
        size = be_u32(rom, FILE_TABLE_ROM + index * 8 + 4)
        require(address & 0xF0000000 == CART_BASE,
                f"audio file {index} has a non-cartridge address")
        start = address & 0x0FFFFFFF
        relative_range(start, size, len(rom), f"audio file {index}")
        if files:
            require(start >= files[-1]["end"], "audio files overlap or regress")
            gap = rom[files[-1]["end"]:start]
            require(len(gap) <= 6 and all(byte in (0x00, 0xFF) for byte in gap),
                    f"audio file {index} has unexpected inter-file bytes")
        payload = rom[start:start + size]
        files.append({
            "index": index,
            "start": start,
            "end": start + size,
            "size": size,
            "sha256": hashlib.sha256(payload).hexdigest(),
        })
    return files


def parse_pointer_bank(data: bytes, sample_data: bytes, name: str) -> dict:
    require(data[:16] == PTR_MAGIC, f"{name} pointer-bank magic is invalid")
    require(sample_data[:16] == WAVE_MAGIC, f"{name} wave-bank magic is invalid")
    require(be_u32(data, 0x10) == 0, f"{name} pointer bank is already relocated")
    count = be_u32(data, 0x20)
    basenotes = be_u32(data, 0x24)
    detunes = be_u32(data, 0x28)
    wave_list = be_u32(data, 0x2C)
    require(0 < count < 4096, f"{name} has an unreasonable wave count")
    relative_range(basenotes, count, len(data), f"{name} base-note table")
    relative_range(detunes, count * 4, len(data), f"{name} detune table")
    relative_range(wave_list, count * 4, len(data), f"{name} wave list")

    waves = []
    looped = 0
    referenced_sample_bytes = 0
    for index in range(count):
        wave_offset = be_u32(data, wave_list + index * 4)
        relative_range(wave_offset, 20, len(data), f"{name} wave {index}")
        sample_offset = be_u32(data, wave_offset)
        sample_size = be_u32(data, wave_offset + 4)
        wave_type = data[wave_offset + 8]
        flags = data[wave_offset + 9]
        loop_offset = be_u32(data, wave_offset + 12)
        book_offset = be_u32(data, wave_offset + 16)
        require(wave_type in (0, 1), f"{name} wave {index} has unknown type")
        require(flags == 0, f"{name} wave {index} is already relocated")
        relative_range(sample_offset, sample_size, len(sample_data),
                       f"{name} wave {index} sample")
        referenced_sample_bytes += sample_size

        loop = None
        if loop_offset:
            relative_range(loop_offset, 12, len(data), f"{name} wave {index} loop")
            loop = {
                "start": be_u32(data, loop_offset),
                "end": be_u32(data, loop_offset + 4),
                "count": be_u32(data, loop_offset + 8),
            }
            require(loop["end"] >= loop["start"],
                    f"{name} wave {index} loop is reversed")
            looped += int(loop["count"] != 0)

        book = None
        if wave_type == 0:
            require(book_offset != 0, f"{name} ADPCM wave {index} has no book")
            relative_range(book_offset, 8, len(data), f"{name} wave {index} book")
            order = be_u32(data, book_offset)
            predictors = be_u32(data, book_offset + 4)
            require(0 < order <= 8 and 0 < predictors <= 32,
                    f"{name} wave {index} has an invalid ADPCM book")
            coefficients = order * predictors * 8
            relative_range(book_offset + 8, coefficients * 2, len(data),
                           f"{name} wave {index} coefficients")
            book = {"order": order, "predictors": predictors}

        waves.append({
            "index": index,
            "sample_offset": sample_offset,
            "sample_size": sample_size,
            "type": "adpcm" if wave_type == 0 else "raw16",
            "base_note": data[basenotes + index],
            "detune_cents": struct.unpack_from(">b", data, detunes + index * 4)[0],
            "loop": loop,
            "book": book,
        })
    return {
        "wave_count": count,
        "looped_wave_count": looped,
        "referenced_sample_bytes": referenced_sample_bytes,
        "waves": waves,
    }


def parse_effect_bank(data: bytes) -> dict:
    count = be_u32(data, 0)
    entries = 0x18
    require(count == 93, "unexpected effect count")
    require(be_u32(data, 0x10) == 0, "effect bank is already relocated")
    relative_range(be_u32(data, 0x14), 1, len(data), "effect-bank bytecode base")
    relative_range(entries, count * 8, len(data), "effect table")
    effects = []
    for index in range(count):
        offset = be_u32(data, entries + index * 8)
        priority = be_u32(data, entries + index * 8 + 4)
        relative_range(offset, 1, len(data), f"effect {index} bytecode")
        effects.append({"index": index, "offset": offset, "priority": priority})
    return {"effect_count": count, "effects": effects}


def parse_song(data: bytes, index: int, music_wave_count: int) -> dict:
    require(len(data) >= 0x38, f"song {index} is truncated")
    require(be_u32(data, 0) == 0x215, f"song {index} has an unknown version")
    channel_count = be_u32(data, 4)
    wave_count = be_u32(data, 8)
    require(12 <= channel_count <= 24, f"song {index} channel count is invalid")
    require(wave_count <= music_wave_count, f"song {index} wave count is invalid")
    offsets = [be_u32(data, offset) for offset in range(0x0C, 0x28, 4)]
    require(be_u32(data, 0x28) == 0, f"song {index} is already relocated")
    require(data[0x2C:0x38] == bytes(12), f"song {index} reserved bytes are nonzero")
    for number, offset in enumerate(offsets):
        if offset:
            relative_range(offset, 1, len(data), f"song {index} section {number}")

    for table_number in range(3):
        table = offsets[table_number]
        require(table != 0, f"song {index} is missing stream table {table_number}")
        relative_range(table, channel_count * 4, len(data),
                       f"song {index} stream table {table_number}")
        for channel in range(channel_count):
            stream = be_u32(data, table + channel * 4)
            if stream:
                relative_range(stream, 1, len(data),
                               f"song {index} channel {channel} stream {table_number}")

    wave_table = offsets[5]
    require(wave_table != 0, f"song {index} has no wave table")
    relative_range(wave_table, wave_count * 2, len(data), f"song {index} waves")
    wave_ids = [be_u16(data, wave_table + item * 2) for item in range(wave_count)]
    require(all(item == 0xFFFF or item < music_wave_count for item in wave_ids),
            f"song {index} references a wave outside pointer bank B")
    require(offsets[6] != 0, f"song {index} has no master track")
    return {
        "song_index": index,
        "channels": channel_count,
        "wave_slots": wave_count,
        "referenced_waves": sorted(set(item for item in wave_ids if item != 0xFFFF)),
    }


def inventory(rom: bytes) -> dict:
    digest = hashlib.sha1(rom).hexdigest()
    require(digest == ROM_SHA1, f"wrong ROM SHA-1: {digest}")
    files = read_file_table(rom)
    payloads = [rom[item["start"]:item["end"]] for item in files]
    sfx_bank = parse_pointer_bank(payloads[0], payloads[2], "SFX")
    effects = parse_effect_bank(payloads[1])
    music_bank = parse_pointer_bank(payloads[3], payloads[4], "music")
    songs = [
        parse_song(payloads[file_index], file_index - 5, music_bank["wave_count"])
        for file_index in range(5, FILE_COUNT)
    ]
    used_music_waves = sorted({
        wave for song in songs for wave in song["referenced_waves"]
    })
    return {
        "format": "BattleTanx Global Assault libmus inventory v1",
        "rom_sha1": digest,
        "file_table_rom": FILE_TABLE_ROM,
        "file_count": len(files),
        "stored_bytes": sum(item["size"] for item in files),
        "files": files,
        "sfx": {"pointer_bank": sfx_bank, "effect_bank": effects},
        "music": {
            "pointer_bank": music_bank,
            "song_count": len(songs),
            "songs": songs,
            "used_wave_count": len(used_music_waves),
            "unused_wave_count": music_bank["wave_count"] - len(used_music_waves),
            "unused_waves": sorted(set(range(music_bank["wave_count"])) - set(used_music_waves)),
        },
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baseroms/us/baserom.z64"))
    parser.add_argument("--output", type=Path, help="write a metadata-only JSON report")
    args = parser.parse_args()
    report = inventory(args.rom.read_bytes())
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(
        f"Validated {report['file_count']} libmus files, {report['stored_bytes']} bytes: "
        f"{report['sfx']['pointer_bank']['wave_count']} SFX waves, "
        f"{report['sfx']['effect_bank']['effect_count']} effects, "
        f"{report['music']['pointer_bank']['wave_count']} music waves, "
        f"{report['music']['song_count']} songs"
    )


if __name__ == "__main__":
    main()
