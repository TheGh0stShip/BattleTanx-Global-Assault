import importlib.util
import json
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "inventory_libmus_assets", ROOT / "tools" / "inventory_libmus_assets.py"
)
MODULE = importlib.util.module_from_spec(SPEC)
assert SPEC.loader is not None
SPEC.loader.exec_module(MODULE)


class LibmusAssetInventoryTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.rom = (ROOT / "baseroms/us/baserom.z64").read_bytes()
        cls.report = MODULE.inventory(cls.rom)

    def test_complete_retail_inventory(self):
        report = self.report
        self.assertEqual(report["file_count"], 26)
        self.assertEqual(report["stored_bytes"], 2_289_292)
        self.assertEqual(report["sfx"]["pointer_bank"]["wave_count"], 76)
        self.assertEqual(report["sfx"]["effect_bank"]["effect_count"], 93)
        self.assertEqual(report["music"]["pointer_bank"]["wave_count"], 231)
        self.assertEqual(report["music"]["song_count"], 21)
        self.assertEqual(report["music"]["used_wave_count"], 171)
        self.assertEqual(report["music"]["unused_wave_count"], 60)

    def test_report_contains_metadata_not_payloads(self):
        encoded = json.dumps(self.report)
        self.assertNotIn("payload", encoded)
        self.assertNotIn("sample_data", encoded)
        self.assertTrue(all(len(item["sha256"]) == 64 for item in self.report["files"]))

    def test_rejects_wrong_rom(self):
        bad = bytearray(self.rom)
        bad[0x58FAE0] ^= 1
        with self.assertRaisesRegex(MODULE.AudioInventoryError, "wrong ROM SHA-1"):
            MODULE.inventory(bytes(bad))


if __name__ == "__main__":
    unittest.main()
