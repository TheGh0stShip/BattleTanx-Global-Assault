import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_stale_object import inventory_stale_object  # noqa: E402


class StaleObjectInventoryTests(unittest.TestCase):
    def test_retail_stale_steps_object(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        report = inventory_stale_object(rom_path.read_bytes())
        self.assertEqual(report["stale_text_size"], 0x7F6)
        self.assertEqual(report["leading_exact_bytes"], 2)
        self.assertEqual(report["instruction_words"], 509)
        self.assertEqual(report["exact_instruction_words"], 438)
        self.assertEqual(len(report["relocation_words"]), 71)
        self.assertEqual(report["debug_lines_size"], 0x69C)
        self.assertEqual(report["debug_symbols_size"], 0x6D6E)
        self.assertEqual(
            {item["kind"] for item in report["relocation_words"]},
            {"jump-target", "immediate"},
        )


if __name__ == "__main__":
    unittest.main()
