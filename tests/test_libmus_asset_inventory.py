import importlib.util
import json
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "inventory_libmus_assets", ROOT / "tools" / "inventory_libmus_assets.py"
)
MODULE = importlib.util.module_from_spec(SPEC)
assert SPEC.loader is not None
SPEC.loader.exec_module(MODULE)
sys.path.insert(0, str(ROOT / "tools"))
PACK_SPEC = importlib.util.spec_from_file_location(
    "pack_libmus_assets", ROOT / "tools" / "pack_libmus_assets.py"
)
PACK = importlib.util.module_from_spec(PACK_SPEC)
assert PACK_SPEC.loader is not None
PACK_SPEC.loader.exec_module(PACK)


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
        self.assertEqual(report["store_start"], 0x58FAE0)
        self.assertEqual(report["store_end"], 0x7BE9AE)
        self.assertEqual(report["store_span_bytes"], 2_289_358)
        self.assertEqual(report["alignment_bytes"], 66)
        self.assertEqual(len(report["gaps"]), 18)

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

    def test_split_store_rebuilds_retail_bytes(self):
        with tempfile.TemporaryDirectory() as temporary:
            source = Path(temporary)
            MODULE.extract(self.rom, self.report, source)
            rebuilt = PACK.rebuild(self.report, source, require_original=True)
            self.assertEqual(
                rebuilt,
                self.rom[self.report["store_start"]:self.report["store_end"]],
            )

    def test_rebuild_rejects_changed_retail_component(self):
        with tempfile.TemporaryDirectory() as temporary:
            source = Path(temporary)
            MODULE.extract(self.rom, self.report, source)
            path = source / "file_02.bin"
            changed = bytearray(path.read_bytes())
            changed[-1] ^= 1
            path.write_bytes(changed)
            with self.assertRaisesRegex(PACK.AudioPackError, "retail hash"):
                PACK.rebuild(self.report, source, require_original=True)

    def test_rebuild_rejects_uncovered_manifest_range(self):
        with tempfile.TemporaryDirectory() as temporary:
            source = Path(temporary)
            MODULE.extract(self.rom, self.report, source)
            damaged = json.loads(json.dumps(self.report))
            damaged["gaps"][0]["start"] += 1
            with self.assertRaisesRegex(PACK.AudioPackError, "uncovered range"):
                PACK.rebuild(damaged, source)


if __name__ == "__main__":
    unittest.main()
