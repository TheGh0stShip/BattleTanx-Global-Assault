import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_stale_debug import inventory_debug_names  # noqa: E402


class StaleDebugInventoryTests(unittest.TestCase):
    def test_retail_debug_name_inventory(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        report = inventory_debug_names(rom_path.read_bytes())
        self.assertEqual(report["size"], 0x7C00)
        self.assertEqual(report["record_count"], 1305)
        self.assertEqual(report["unique_name_count"], 664)
        self.assertIn("Steps_PruneFork", report["unique_names"])
        self.assertIn("Obstacles_CopyObstacleRef", report["unique_names"])


if __name__ == "__main__":
    unittest.main()
