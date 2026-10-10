import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from verify_asset_corpus import require, verify_corpus  # noqa: E402


class AssetCorpusGateTests(unittest.TestCase):
    def test_require_rejects_changed_inventory_total(self):
        with self.assertRaisesRegex(ValueError, "world count"):
            require(74, 75, "world count")

    def test_wrong_rom_is_rejected_before_asset_parsing(self):
        with self.assertRaisesRegex(ValueError, "base ROM SHA-1"):
            verify_corpus(bytes(0x800000), ROOT)


if __name__ == "__main__":
    unittest.main()
