import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_rom_layout import inventory_layout  # noqa: E402


class RomLayoutInventoryTests(unittest.TestCase):
    def test_current_rom_is_completely_accounted(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        rom = rom_path.read_bytes()
        report = inventory_layout(
            rom,
            ROOT / "src/code/slot_asset_ranges.c",
            ROOT / "src/code/campaign_mission_config.c",
        )
        self.assertEqual(report["rom_size"], 0x800000)
        self.assertEqual(sum(report["bytes"].values()), len(rom))
        self.assertEqual(report["counts"]["world"], 75)
        self.assertEqual(report["counts"]["image"], 195)
        self.assertEqual(report["counts"]["script"], 17)
        self.assertEqual(report["counts"]["audio"], 26)
        self.assertEqual(report["counts"]["image_aux"], 4)
        self.assertEqual(report["counts"]["raw_buffer"], 3)
        self.assertEqual(report["counts"]["alignment_padding"], 233)
        self.assertEqual(report["bytes"]["alignment_padding"], 962)
        self.assertEqual(report["counts"]["stale_song_duplicate"], 6)
        self.assertEqual(report["bytes"]["stale_song_duplicate"], 136294)
        self.assertEqual(report["counts"]["stale_pool_duplicate"], 6)
        self.assertEqual(report["bytes"]["stale_pool_duplicate"], 123912)
        self.assertEqual(report["counts"]["stale_debug_symbols"], 1)
        self.assertEqual(report["bytes"]["stale_debug_symbols"], 31744)
        self.assertEqual(report["counts"]["stale_padding"], 7)
        self.assertEqual(report["bytes"]["stale_padding"], 2402)
        self.assertNotIn("stale_build_material", report["counts"])
        self.assertEqual(
            sorted(
                item["scene_type"]
                for item in report["regions"]
                if item["kind"] == "script"
            ),
            [1, 1, 1, 1, 1, 1, 4, 6, 7, 13, 15, 24, 24, 24, 24, 24, 24],
        )


if __name__ == "__main__":
    unittest.main()
