from __future__ import annotations

import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.verify_rom import EXPECTED_SIZE, Z64_MAGIC, verify


class VerifyRomTests(unittest.TestCase):
    def test_rejects_wrong_hash(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            rom = Path(directory) / "bad.z64"
            rom.write_bytes(Z64_MAGIC + bytes(EXPECTED_SIZE - len(Z64_MAGIC)))
            with self.assertRaises(SystemExit) as raised:
                verify(rom)
            self.assertIn("SHA-1", str(raised.exception))

    def test_accepts_expected_dump(self) -> None:
        rom = Path("BattleTanx - Global Assault (USA).z64")
        if not rom.exists():
            self.skipTest("local legally obtained ROM is unavailable")
        verify(rom)


if __name__ == "__main__":
    unittest.main()
