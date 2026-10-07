from __future__ import annotations

import hashlib
import unittest
from pathlib import Path


ROM = Path("BattleTanx - Global Assault (USA).z64")
VRAM_TO_ROM = 0x80070000
F3DEX_OFFSET = 0x000B5FF8
F3DEX_SIGNATURE = (
    b"RSP Gfx ucode F3DEX       fifo 2.07  "
    b"Yoshitaka Yasumoto 1998 Nintendo.\n\0"
)
CODE_FINGERPRINTS = {
    (0x80105B10, 0x290): "c0b4f26132ee60ee5af1a6113590ae09c98e9178dab0b767f4e054acb5a13198",
    (0x80110E40, 0xA0): "eedcf8947e753222eb1c9ae9399b41ec3c88cb814fefe1e041efa653255e7ec3",
    (0x8010FA80, 0xE0): "f16b7de0b557e332cf4fcc108b5a4fd10a635e5f589900046165e991223c3eba",
}


class LibraryFingerprintTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        if not ROM.exists():
            raise unittest.SkipTest("local legally obtained ROM is unavailable")
        cls.rom = ROM.read_bytes()

    def test_f3dex_fifo_207_signature(self) -> None:
        actual = self.rom[F3DEX_OFFSET : F3DEX_OFFSET + len(F3DEX_SIGNATURE)]
        self.assertEqual(F3DEX_SIGNATURE, actual)

    def test_stable_libultra_20i_code_fingerprints(self) -> None:
        for (vram, size), expected in CODE_FINGERPRINTS.items():
            with self.subTest(vram=f"0x{vram:08X}"):
                offset = vram - VRAM_TO_ROM
                actual = hashlib.sha256(self.rom[offset : offset + size]).hexdigest()
                self.assertEqual(expected, actual)


if __name__ == "__main__":
    unittest.main()
