import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_lzari_assets import (  # noqa: E402
    bundle_layout,
    bundle_offsets,
    format_hint,
    inventory,
    load_boundaries,
)
from pack_lzari_bundle import pack_bundle  # noqa: E402


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

    def test_minimal_btga_bundle(self):
        data = bytes.fromhex(
            "0000002000000024000000240000002400000024000000240000002400000024"
            "00000000"
        )
        layout = bundle_layout(data)
        self.assertIsNotNone(layout)
        self.assertEqual(layout["component_counts"], [1, 0, 0, 0, 0, None, 0])
        self.assertEqual(format_hint(data), "btga_7_component_bundle")

    def test_current_rom_inventory(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        rom = rom_path.read_bytes()
        streams = inventory(rom, ROOT / "src/code/slot_asset_ranges.c")
        self.assertEqual(len(streams), 74)
        self.assertEqual(sum(item["packed_size"] for item in streams), 481785)
        self.assertEqual(sum(item["decoded_size"] for item in streams), 1570628)
        self.assertTrue(all(item["padding_bits"] in (8, 16) for item in streams))
        self.assertTrue(
            all(item["format_hint"] == "btga_7_component_bundle" for item in streams)
        )
        self.assertTrue(all(len(item["component_sizes"]) == 7 for item in streams))
        self.assertTrue(
            all(
                item["component_sizes"][1] == item["component_counts"][1] * 16
                for item in streams
            )
        )
        self.assertTrue(all(item["reencode_exact"] for item in streams))
        first = streams[0]
        offsets = first["offsets"]
        components = [
            first["data"][left:right]
            for left, right in zip(offsets, offsets[1:])
        ]
        self.assertEqual(pack_bundle(components), first["data"])


if __name__ == "__main__":
    unittest.main()
