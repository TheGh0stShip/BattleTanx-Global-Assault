import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_lzari_assets import (  # noqa: E402
    bundle_layout,
    bundle_offsets,
    format_hint,
    image_ranges,
    inventory,
    inventory_known,
    load_boundaries,
)
from pack_lzari_bundle import pack_bundle  # noqa: E402
from n64_texture import decode_texture  # noqa: E402


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
        self.assertEqual(layout["group_count"], 0)
        self.assertEqual(layout["placement_count"], 0)
        self.assertEqual(layout["model_count"], 0)
        self.assertEqual(layout["part_count"], 0)
        self.assertEqual(layout["pool_ref_count"], 0)
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
                item["component_sizes"][1] == item["group_count"] * 16
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

    def test_image_table_and_complete_known_inventory(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        rom = rom_path.read_bytes()
        images = image_ranges(rom)
        self.assertEqual(len(images), 195)
        self.assertEqual(images[0]["start"], 0x46F660)
        self.assertEqual(images[-1]["end"], 0x514714)
        streams = inventory_known(rom, ROOT / "src/code/slot_asset_ranges.c")
        self.assertEqual(len(streams), 271)
        self.assertEqual(sum(item["kind"] == "world" for item in streams), 75)
        self.assertEqual(sum(item["kind"] == "image" for item in streams), 195)
        self.assertEqual(sum(item["packed_size"] for item in streams), 1174975)
        self.assertEqual(sum(item["decoded_size"] for item in streams), 2886765)
        self.assertTrue(all(item["reencode_exact"] for item in streams))
        self.assertTrue(all(item["storage_padding_bytes"] in (0, 1) for item in streams))
        image_streams = [item for item in streams if item["kind"] == "image"]
        self.assertEqual(
            {(item["format"], item["flags"]) for item in image_streams},
            {(0, 2), (0, 3), (2, 0), (2, 1), (3, 1), (3, 2)},
        )
        for item in image_streams:
            pixels = decode_texture(
                item["data"], item["format"], item["flags"], item["width"], item["height"]
            )
            self.assertEqual(len(pixels), item["width"] * item["height"] * 4)


if __name__ == "__main__":
    unittest.main()
