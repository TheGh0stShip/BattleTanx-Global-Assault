import tempfile
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_world_pools import chunk_filename, gap_filename  # noqa: E402
from pack_world_pools import PoolPackError, rebuild_pool  # noqa: E402


class WorldPoolPackerTests(unittest.TestCase):
    def pool(self):
        return {
            "name": "test",
            "size": 8,
            "chunks": [
                {"index": 0, "pool_offset": 0, "size": 5},
                {"index": 1, "pool_offset": 3, "size": 3},
            ],
            "gaps": [{"pool_offset": 6, "size": 2}],
        }

    def write_inputs(self, root: Path, second: bytes = b"def"):
        pool = self.pool()
        root.joinpath(chunk_filename(pool["chunks"][0])).write_bytes(b"abcde")
        root.joinpath(chunk_filename(pool["chunks"][1])).write_bytes(second)
        gaps = root / "unreferenced"
        gaps.mkdir()
        gaps.joinpath(gap_filename(0, pool["gaps"][0])).write_bytes(b"gh")

    def test_rebuilds_overlapping_split_inputs(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.write_inputs(root)
            self.assertEqual(rebuild_pool(self.pool(), root), b"abcdefgh")

    def test_rejects_conflicting_overlap(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.write_inputs(root, b"XYZ")
            with self.assertRaisesRegex(PoolPackError, "conflicts"):
                rebuild_pool(self.pool(), root)

    def test_rejects_wrong_piece_size(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.write_inputs(root)
            root.joinpath(chunk_filename(self.pool()["chunks"][0])).write_bytes(b"shorter")
            with self.assertRaisesRegex(PoolPackError, "expected 5"):
                rebuild_pool(self.pool(), root)


if __name__ == "__main__":
    unittest.main()
