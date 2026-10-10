import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_lzari_assets import inventory_ranges  # noqa: E402
from inventory_world_pools import world_ranges  # noqa: E402
from inventory_world_textures import inventory_textures  # noqa: E402


class WorldTextureInventoryTests(unittest.TestCase):
    def test_current_rom_texture_inventory(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        rom = rom_path.read_bytes()
        worlds = inventory_ranges(
            rom, world_ranges(ROOT / "src/code/slot_asset_ranges.c")
        )
        report = inventory_textures(rom, worlds)
        self.assertEqual(report["usage_count"], 1351)
        self.assertEqual(report["reference_count"], 38910)
        self.assertEqual(report["preview_count"], 1556)
        self.assertEqual(
            report["formats"],
            {"CI4": 108, "I4": 14, "I8": 7, "RGBA16": 1219, "RGBA32": 2},
        )
        self.assertEqual(
            report["statuses"],
            {"exact": 1347, "missing texture-state commands": 1, "short payload": 3},
        )


if __name__ == "__main__":
    unittest.main()
