#!/usr/bin/env python3
"""Decode BattleTanx: Global Assault's big-endian LZARI streams."""

from __future__ import annotations

import argparse
import struct
from pathlib import Path


N = 4096
F = 60
THRESHOLD = 2
N_CHAR = 256 - THRESHOLD + F
Q1 = 1 << 15
Q2 = 2 * Q1
Q3 = 3 * Q1
Q4 = 4 * Q1
MAX_CUM = Q1 - 1


class LzariError(ValueError):
    pass


class Decoder:
    def __init__(self, source: bytes):
        self.source = source
        self.offset = 0
        self.mask = 0
        self.byte = 0
        self.padding_bits = 0
        self.low = 0
        self.high = Q4
        self.value = 0

        self.sym_to_char = [0] * (N_CHAR + 1)
        self.sym_freq = [0] * (N_CHAR + 1)
        self.sym_cum = [0] * (N_CHAR + 1)
        self.position_cum = [0] * (N + 1)
        self._start_model()

    def _bit(self) -> int:
        self.mask >>= 1
        if self.mask == 0:
            if self.offset >= len(self.source):
                # Okumura's GetBit uses getc(), whose EOF value behaves as an
                # all-one byte. Retail streams rely on this arithmetic-coder
                # tail padding, but cap it so a truncated stream cannot run on.
                self.padding_bits += 8
                if self.padding_bits > 32:
                    raise LzariError("compressed stream ended before output was complete")
                self.byte = 0xFF
            else:
                self.byte = self.source[self.offset]
                self.offset += 1
            self.mask = 0x80
        return int(bool(self.byte & self.mask))

    def _start_model(self) -> None:
        for sym in range(N_CHAR, 0, -1):
            self.sym_to_char[sym] = sym - 1
            self.sym_freq[sym] = 1
            self.sym_cum[sym - 1] = self.sym_cum[sym] + 1
        for index in range(N, 0, -1):
            self.position_cum[index - 1] = (
                self.position_cum[index] + 10000 // (index + 200)
            )

    def _update_model(self, sym: int) -> None:
        if self.sym_cum[0] >= MAX_CUM:
            total = 0
            for index in range(N_CHAR, 0, -1):
                self.sym_cum[index] = total
                self.sym_freq[index] = (self.sym_freq[index] + 1) >> 1
                total += self.sym_freq[index]
            self.sym_cum[0] = total

        index = sym
        while self.sym_freq[index] == self.sym_freq[index - 1]:
            index -= 1
        if index < sym:
            self.sym_to_char[index], self.sym_to_char[sym] = (
                self.sym_to_char[sym],
                self.sym_to_char[index],
            )
        self.sym_freq[index] += 1
        index -= 1
        while index >= 0:
            self.sym_cum[index] += 1
            index -= 1

    def _renormalize(self) -> None:
        while True:
            if self.low >= Q2:
                self.value -= Q2
                self.low -= Q2
                self.high -= Q2
            elif self.low >= Q1 and self.high <= Q3:
                self.value -= Q1
                self.low -= Q1
                self.high -= Q1
            elif self.high > Q2:
                return
            self.low *= 2
            self.high *= 2
            self.value = self.value * 2 + self._bit()

    @staticmethod
    def _search(cumulative: list[int], value: int, high: int) -> int:
        low = 1
        while low < high:
            middle = (low + high) // 2
            if cumulative[middle] > value:
                low = middle + 1
            else:
                high = middle
        return low

    def start(self) -> None:
        for _ in range(17):
            self.value = self.value * 2 + self._bit()

    def character(self) -> int:
        span = self.high - self.low
        scaled = ((self.value - self.low + 1) * self.sym_cum[0] - 1) // span
        sym = self._search(self.sym_cum, scaled, N_CHAR)
        self.high = self.low + span * self.sym_cum[sym - 1] // self.sym_cum[0]
        self.low += span * self.sym_cum[sym] // self.sym_cum[0]
        self._renormalize()
        character = self.sym_to_char[sym]
        self._update_model(sym)
        return character

    def position(self) -> int:
        span = self.high - self.low
        scaled = ((self.value - self.low + 1) * self.position_cum[0] - 1) // span
        position = self._search(self.position_cum, scaled, N) - 1
        self.high = self.low + span * self.position_cum[position] // self.position_cum[0]
        self.low += span * self.position_cum[position + 1] // self.position_cum[0]
        self._renormalize()
        return position


def decompress(stream: bytes, *, max_output: int = 64 * 1024 * 1024) -> bytes:
    if len(stream) < 7:
        raise LzariError("stream is too short")
    output_size = struct.unpack_from(">I", stream)[0]
    if output_size == 0:
        return b""
    if output_size > max_output:
        raise LzariError(
            f"declared output size 0x{output_size:X} exceeds limit 0x{max_output:X}"
        )

    decoder = Decoder(stream[4:])
    decoder.start()
    ring = bytearray(N)
    ring[:N - F] = b" " * (N - F)
    ring_offset = N - F
    output = bytearray()

    while len(output) < output_size:
        character = decoder.character()
        if character < 256:
            output.append(character)
            ring[ring_offset] = character
            ring_offset = (ring_offset + 1) & (N - 1)
            continue

        source_offset = (ring_offset - decoder.position() - 1) & (N - 1)
        length = character - 255 + THRESHOLD
        if len(output) + length > output_size:
            raise LzariError("match extends beyond the declared output size")
        for index in range(length):
            character = ring[(source_offset + index) & (N - 1)]
            output.append(character)
            ring[ring_offset] = character
            ring_offset = (ring_offset + 1) & (N - 1)

    return bytes(output)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--max-output", type=lambda value: int(value, 0), default=64 << 20)
    args = parser.parse_args()
    args.output.write_bytes(decompress(args.input.read_bytes(), max_output=args.max_output))


if __name__ == "__main__":
    main()
