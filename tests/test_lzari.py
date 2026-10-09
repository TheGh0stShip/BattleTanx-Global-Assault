import hashlib
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from lzari import LzariError, compress, decompress  # noqa: E402


class LzariTests(unittest.TestCase):
    def test_known_rom_asset(self):
        rom = ROOT / "baseroms/us/baserom.z64"
        if not rom.exists():
            self.skipTest("base ROM is unavailable")
        data = rom.read_bytes()[0x3F9B60:0x3FCCB0]
        decoded = decompress(data)
        self.assertEqual(len(decoded), 0xB6C4)
        self.assertEqual(decoded[:0x20], bytes.fromhex(
            "000000200000002400000044000030c8"
            "0000339000003c0000003e1c0000b6c4"
        ))
        self.assertEqual(
            hashlib.sha256(decoded).hexdigest(),
            "53adcb8fa1ee6307a4e945be6f5a849e711b5c4ef647247e3d16da7fed1d2b70",
        )
        self.assertEqual(compress(decoded), data)

    def test_empty_round_trip(self):
        encoded = compress(b"")
        self.assertEqual(encoded, bytes(4))
        self.assertEqual(decompress(encoded), b"")

    def test_rejects_oversized_header(self):
        with self.assertRaisesRegex(LzariError, "exceeds limit"):
            decompress(bytes.fromhex("01000000ffffff"), max_output=0x1000)

    def test_rejects_truncated_stream(self):
        with self.assertRaises(LzariError):
            decompress(bytes.fromhex("00000010000000"))


if __name__ == "__main__":
    unittest.main()
