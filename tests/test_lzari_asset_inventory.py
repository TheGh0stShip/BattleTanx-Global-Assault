import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_lzari_assets import (  # noqa: E402
    bundle_offsets,
    format_hint,
    inventory,
    load_boundaries,
)


class LzariAssetInventoryTests(unittest.TestCase):
    def test_boundaries_are_unique_and_sorted(self):
        values = load_boundaries(ROOT / "src/code/slot_asset_ranges.c")
        self.assertEqual(values, sorted(set(values)))
        self.assertEqual(values[0], 0x3F9B60)
        self.assertEqual(values[-1], 0x46F652)

    def test_known_offset_table(self):
        data = bytes.fromhex("0000000800000008")
        self.assertEqual(bundle_offsets(data), (8, 8))
        self.assertEqual(format_hint(data), "be_2_offset_bundle")
        self.assertEqual(format_hint(b"plain data"), "unknown")

    def test_current_rom_inventory(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        streams = inventory(rom_path.read_bytes(), ROOT / "src/code/slot_asset_ranges.c")
        self.assertEqual(len(streams), 74)
        self.assertEqual(sum(item["packed_size"] for item in streams), 481785)
        self.assertEqual(sum(item["decoded_size"] for item in streams), 1570628)
        self.assertTrue(all(item["padding_bits"] in (8, 16) for item in streams))
        self.assertTrue(all(item["format_hint"] == "be_8_offset_bundle" for item in streams))
        self.assertTrue(all(len(item["component_sizes"]) == 7 for item in streams))


if __name__ == "__main__":
    unittest.main()
