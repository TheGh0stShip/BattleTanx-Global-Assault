#!/usr/bin/env python3
"""Decode one Sound Tools PTR/WBK wave to an inspection-only PCM16 WAV.

PTR/WBK files do not carry an authoritative playback rate.  The caller must
provide one explicitly; decoding never resamples or applies song/BFX pitch.
"""

from __future__ import annotations

import argparse
import struct
from pathlib import Path

from inventory_libmus_assets import (
    AudioInventoryError,
    be_u32,
    parse_pointer_bank,
    read_file_table,
)


FRAME_BYTES = 9
SAMPLES_PER_FRAME = 16
COEFFICIENTS_PER_PREDICTOR = 16
MAXIMUM_SCALE = 12
MAXIMUM_PREDICTORS = 16


class AudioDecodeError(ValueError):
    """The selected wave cannot be decoded without guessing."""


def _signed16(data: bytes, offset: int) -> int:
    return struct.unpack_from(">h", data, offset)[0]


def read_book(pointer_data: bytes, wave: dict) -> tuple[int, int, list[int]]:
    offset = wave["book_offset"]
    order = be_u32(pointer_data, offset)
    predictors = be_u32(pointer_data, offset + 4)
    if order != 2:
        raise AudioDecodeError(f"ADPCM book order {order} is not 2")
    if not 1 <= predictors <= MAXIMUM_PREDICTORS:
        raise AudioDecodeError(
            f"ADPCM predictor count {predictors} is outside 1..{MAXIMUM_PREDICTORS}"
        )
    coefficient_count = order * predictors * 8
    end = offset + 8 + coefficient_count * 2
    if end > len(pointer_data):
        raise AudioDecodeError("ADPCM book coefficients are truncated")
    coefficients = [
        _signed16(pointer_data, offset + 8 + index * 2)
        for index in range(coefficient_count)
    ]
    return order, predictors, coefficients


def decode_adpcm(encoded: bytes, order: int, predictors: int,
                 coefficients: list[int]) -> list[int]:
    if order != 2:
        raise AudioDecodeError(f"ADPCM book order {order} is not 2")
    if not 1 <= predictors <= MAXIMUM_PREDICTORS:
        raise AudioDecodeError("ADPCM predictor count is invalid")
    if len(coefficients) != predictors * COEFFICIENTS_PER_PREDICTOR:
        raise AudioDecodeError("ADPCM coefficient count is invalid")
    if len(encoded) % FRAME_BYTES:
        raise AudioDecodeError("ADPCM payload is not an exact 9-byte frame multiple")

    for frame in range(len(encoded) // FRAME_BYTES):
        header = encoded[frame * FRAME_BYTES]
        scale = header >> 4
        predictor = header & 0x0F
        if scale > MAXIMUM_SCALE:
            raise AudioDecodeError(
                f"ADPCM frame {frame} scale {scale} exceeds {MAXIMUM_SCALE}"
            )
        if predictor >= predictors:
            raise AudioDecodeError(
                f"ADPCM frame {frame} predictor {predictor} exceeds book"
            )

    samples: list[int] = []
    state = [0] * 8
    for frame_offset in range(0, len(encoded), FRAME_BYTES):
        header = encoded[frame_offset]
        scale = header >> 4
        coefficient_base = (header & 0x0F) * COEFFICIENTS_PER_PREDICTOR
        for half in range(2):
            payload_offset = frame_offset + 1 + half * 4
            residuals = []
            for index in range(8):
                packed = encoded[payload_offset + (index >> 1)]
                nibble = packed >> 4 if index & 1 == 0 else packed & 0x0F
                residuals.append((nibble if nibble < 8 else nibble - 16) << scale)

            older = state[6]
            newer = state[7]
            for index in range(8):
                accumulator = (residuals[index] << 11) & 0xFFFFFFFF
                accumulator = (
                    accumulator + coefficients[coefficient_base + index] * older
                ) & 0xFFFFFFFF
                accumulator = (
                    accumulator + coefficients[coefficient_base + 8 + index] * newer
                ) & 0xFFFFFFFF
                for previous in range(index):
                    accumulator = (
                        accumulator
                        + coefficients[coefficient_base + 8 + previous]
                        * residuals[index - 1 - previous]
                    ) & 0xFFFFFFFF
                signed = accumulator if accumulator < 0x80000000 else accumulator - 0x100000000
                decoded = signed >> 11
                clamped = max(-32768, min(32767, decoded))
                state[index] = clamped
                samples.append(clamped)
    return samples


def wav_bytes(samples: list[int], sample_rate: int,
              loop: dict | None = None) -> bytes:
    if not 1000 <= sample_rate <= 384000:
        raise AudioDecodeError("sample rate must be within 1000..384000 Hz")
    pcm = struct.pack(f"<{len(samples)}h", *samples)
    chunks = [
        b"fmt " + struct.pack("<IHHIIHH", 16, 1, 1, sample_rate,
                              sample_rate * 2, 2, 16),
        b"data" + struct.pack("<I", len(pcm)) + pcm,
    ]
    if loop is not None:
        if loop["count"] != 0xFFFFFFFF:
            raise AudioDecodeError("finite ADPCM loop conversion is not proven")
        start = loop["start"]
        end = loop["end"]
        if not 0 <= start < end <= len(samples):
            raise AudioDecodeError("ADPCM loop lies outside decoded samples")
        sample_period = (1_000_000_000 + sample_rate // 2) // sample_rate
        body = struct.pack(
            "<15I", 0, 0, sample_period, 60, 0, 0, 0, 1, 0,
            0, 0, start, end - 1, 0, 0,
        )
        chunks.append(b"smpl" + struct.pack("<I", len(body)) + body)
    payload = b"WAVE" + b"".join(chunks)
    return b"RIFF" + struct.pack("<I", len(payload)) + payload


def decode_wave(pointer_data: bytes, wave_data: bytes, index: int,
                sample_rate: int) -> tuple[bytes, dict]:
    bank = parse_pointer_bank(pointer_data, wave_data, "selected")
    if not 0 <= index < bank["wave_count"]:
        raise AudioDecodeError(f"wave index {index} is outside the bank")
    wave = bank["waves"][index]
    if wave["type"] != "adpcm":
        raise AudioDecodeError("selected wave is not ADPCM")
    order, predictors, coefficients = read_book(pointer_data, wave)
    encoded = wave_data[
        wave["sample_offset"]:wave["sample_offset"] + wave["sample_size"]
    ]
    samples = decode_adpcm(encoded, order, predictors, coefficients)
    return wav_bytes(samples, sample_rate, wave["loop"]), {
        "wave_index": index,
        "encoded_bytes": len(encoded),
        "decoded_samples": len(samples),
        "sample_rate": sample_rate,
        "loop": wave["loop"],
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--rom", type=Path)
    source.add_argument("--pointer", type=Path)
    parser.add_argument("--wave-bank", type=Path,
                        help="WBK paired with --pointer")
    parser.add_argument("--bank", choices=("sfx", "music"),
                        help="bank selected from --rom")
    parser.add_argument("--index", required=True, type=int)
    parser.add_argument("--sample-rate", required=True, type=int,
                        help="explicit WAV metadata rate; the bank stores none")
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()

    if args.rom:
        if args.bank is None or args.wave_bank is not None:
            parser.error("--rom requires --bank and does not accept --wave-bank")
        rom = args.rom.read_bytes()
        files = read_file_table(rom)
        pointer_index, wave_index = (0, 2) if args.bank == "sfx" else (3, 4)
        pointer_data = rom[files[pointer_index]["start"]:files[pointer_index]["end"]]
        wave_data = rom[files[wave_index]["start"]:files[wave_index]["end"]]
    else:
        if args.wave_bank is None or args.bank is not None:
            parser.error("--pointer requires --wave-bank and does not accept --bank")
        pointer_data = args.pointer.read_bytes()
        wave_data = args.wave_bank.read_bytes()

    try:
        wav, report = decode_wave(
            pointer_data, wave_data, args.index, args.sample_rate
        )
    except (AudioInventoryError, AudioDecodeError) as exc:
        parser.error(str(exc))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(wav)
    loop = " with stored infinite loop" if report["loop"] else ""
    print(
        f"Decoded wave {args.index}: {report['encoded_bytes']} ADPCM bytes -> "
        f"{report['decoded_samples']} mono PCM16 samples at explicit "
        f"{args.sample_rate} Hz{loop}"
    )


if __name__ == "__main__":
    main()
