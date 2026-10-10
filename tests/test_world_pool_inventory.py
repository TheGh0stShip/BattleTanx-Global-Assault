import hashlib
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_lzari_assets import inventory_ranges  # noqa: E402
from inventory_world_pools import (  # noqa: E402
    coverage_ranges,
    inventory_pools,
    world_ranges,
)


class WorldPoolInventoryTests(unittest.TestCase):
    def test_coverage_merges_overlaps_and_reports_gaps(self):
        chunks = [
            {"pool_offset": 2, "size": 4},
            {"pool_offset": 4, "size": 4},
            {"pool_offset": 10, "size": 2},
        ]
        coverage, gaps = coverage_ranges(chunks, 16)
        self.assertEqual(
            coverage,
            [
                {"pool_offset": 2, "size": 6},
                {"pool_offset": 10, "size": 2},
            ],
        )
        self.assertEqual(
            gaps,
            [
                {"pool_offset": 0, "size": 2},
                {"pool_offset": 8, "size": 2},
                {"pool_offset": 12, "size": 4},
            ],
        )

    def test_current_rom_pool_inventory(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        rom = rom_path.read_bytes()
        worlds = inventory_ranges(
            rom, world_ranges(ROOT / "src/code/slot_asset_ranges.c")
        )
        report = inventory_pools(rom, worlds)
        self.assertEqual(report["world_count"], 75)
        self.assertEqual(report["pool_reference_count"], 39103)
        expected = {
            "texture": (736, 1934176, 1930032, 123088, 25),
            "state": (409, 33744, 33664, 4096, 28),
            "geometry": (4638, 903520, 903424, 102904, 92),
        }
        for pool in report["pools"]:
            chunks, with_overlap, referenced, unreferenced, segments = expected[
                pool["name"]
            ]
            self.assertEqual(pool["chunk_count"], chunks)
            self.assertEqual(pool["referenced_bytes_with_overlap"], with_overlap)
            self.assertEqual(pool["referenced_bytes"], referenced)
            self.assertEqual(pool["unreferenced_bytes"], unreferenced)
            self.assertEqual(len(pool["coverage"]), segments)
            self.assertEqual(referenced + unreferenced, pool["size"])
            payload = rom[pool["rom_start"] : pool["rom_end"]]
            self.assertEqual(hashlib.sha256(payload).hexdigest(), pool["sha256"])


if __name__ == "__main__":
    unittest.main()
