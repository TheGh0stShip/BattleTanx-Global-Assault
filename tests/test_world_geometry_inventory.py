import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_lzari_assets import inventory_ranges  # noqa: E402
from inventory_world_geometry import inventory_geometry  # noqa: E402
from inventory_world_pools import inventory_pools, world_ranges  # noqa: E402


class WorldGeometryInventoryTests(unittest.TestCase):
    def test_current_rom_geometry_inventory(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        rom = rom_path.read_bytes()
        worlds = inventory_ranges(
            rom, world_ranges(ROOT / "src/code/slot_asset_ranges.c")
        )
        report = inventory_geometry(rom, inventory_pools(rom, worlds))
        self.assertEqual(report["chunk_count"], 4638)
        self.assertEqual(report["vertex_load_count"], 4899)
        self.assertEqual(report["vertex_count"], 41996)
        self.assertEqual(report["triangle_count"], 19411)
        self.assertEqual(
            report["statuses"], {"exact": 4637, "vertex-command overlap": 1}
        )
        self.assertEqual(
            [item["pool_offset"] for item in report["chunks"] if item["status"] != "exact"],
            [0x5D40],
        )


if __name__ == "__main__":
    unittest.main()
