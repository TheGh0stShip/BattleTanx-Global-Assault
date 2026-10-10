import hashlib
import importlib.util
import struct
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
SPEC = importlib.util.spec_from_file_location(
    "decode_libmus_wave", ROOT / "tools" / "decode_libmus_wave.py"
)
MODULE = importlib.util.module_from_spec(SPEC)
assert SPEC.loader is not None
SPEC.loader.exec_module(MODULE)


class LibmusWaveDecoderTests(unittest.TestCase):
    @staticmethod
    def oracle_book():
        coefficients = [0] * 64
        coefficients[16] = 1
        coefficients[17] = 2048
        coefficients[25] = -2048
        coefficients[40] = 2048
        coefficients[41] = 1024
        coefficients[48] = 32767
        coefficients[49] = -32768
        coefficients[50] = 1
        coefficients[51] = -1
        return coefficients

    def assert_vector(self, encoded, expected):
        actual = MODULE.decode_adpcm(
            bytes.fromhex(encoded), 2, 4, self.oracle_book()
        )
        self.assertEqual(actual, expected)

    def test_oracle_vectors_pin_runtime_semantics(self):
        self.assert_vector(
            "00 78 F1 2E 3D 4C 5B 6A 90",
            [7, -8, -1, 1, 2, -2, 3, -3, 4, -4, 5, -5, 6, -6, -7, 0],
        )
        self.assert_vector(
            "02 00 00 00 23 1F 00 00 00",
            [0, 0, 0, 0, 0, 0, 2, 5, 6, 2, -1, -1, 0, 0, 0, 0],
        )
        self.assert_vector(
            "C3 00 00 00 78 00 00 00 00",
            [0, 0, 0, 0, 0, 0, 28672, -32768,
             32767, -32768, 14, -14, 0, 0, 0, 0],
        )

    def test_signed_32_bit_accumulator_wrap(self):
        coefficients = [0] * 64
        coefficients[8:16] = [32767] * 8
        actual = MODULE.decode_adpcm(
            bytes.fromhex("C0 77 77 77 77 77 77 77 77"), 2, 4, coefficients
        )
        self.assertEqual(
            actual,
            [28672, 32767, 32767, -32768, -32768, 32767, 32767, -32768,
             -32768, -32768, 32767, 32767, -32768, -32768, 32767, 32767],
        )

    def test_every_retail_wave_decodes_and_wav_is_well_formed(self):
        rom = (ROOT / "baseroms/us/baserom.z64").read_bytes()
        files = MODULE.read_file_table(rom)
        aggregate = hashlib.sha256()
        wave_count = 0
        for pointer_index, wave_index in ((0, 2), (3, 4)):
            pointer = rom[files[pointer_index]["start"]:files[pointer_index]["end"]]
            waves = rom[files[wave_index]["start"]:files[wave_index]["end"]]
            bank = MODULE.parse_pointer_bank(pointer, waves, "test")
            for index in range(bank["wave_count"]):
                wav, report = MODULE.decode_wave(pointer, waves, index, 22050)
                self.assertEqual(wav[:4], b"RIFF")
                self.assertEqual(wav[8:12], b"WAVE")
                self.assertEqual(struct.unpack_from("<I", wav, 4)[0] + 8, len(wav))
                self.assertEqual(report["decoded_samples"] % 16, 0)
                aggregate.update(wav)
                wave_count += 1
        self.assertEqual(wave_count, 307)
        self.assertEqual(
            aggregate.hexdigest(),
            "4f7c4695cf72805d1c0bd62653f005cabc1dea0e9885f29c69a8baa239c54b64",
        )

    def test_rejects_implicit_rate_and_malformed_frames(self):
        with self.assertRaisesRegex(MODULE.AudioDecodeError, "9-byte"):
            MODULE.decode_adpcm(bytes(8), 2, 4, self.oracle_book())
        with self.assertRaisesRegex(MODULE.AudioDecodeError, "scale"):
            MODULE.decode_adpcm(bytes([0xD0]) + bytes(8), 2, 4, self.oracle_book())
        with self.assertRaisesRegex(MODULE.AudioDecodeError, "sample rate"):
            MODULE.wav_bytes([], 0)


if __name__ == "__main__":
    unittest.main()
