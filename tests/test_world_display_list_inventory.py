import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_lzari_assets import inventory_ranges  # noqa: E402
from inventory_world_display_lists import (  # noqa: E402
    find_command_stream,
    inventory_display_lists,
)
from inventory_world_pools import inventory_pools, world_ranges  # noqa: E402


class WorldDisplayListInventoryTests(unittest.TestCase):
    def test_finds_commands_after_leading_payload(self):
        payload = bytes.fromhex(
            "123456789ABCDEF0"
            "0100200800000000"
            "0500020400000000"
            "DF00000000000000"
        )
        start, commands = find_command_stream(payload, "geometry")
        self.assertEqual(start, 8)
        self.assertEqual(len(commands), 3)

    def test_rejects_end_command_with_nonzero_second_word(self):
        payload = bytes.fromhex("0100200800000000DF00000000000001")
        with self.assertRaises(ValueError):
            find_command_stream(payload, "geometry")

    def test_current_rom_display_lists(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        rom = rom_path.read_bytes()
        worlds = inventory_ranges(
            rom, world_ranges(ROOT / "src/code/slot_asset_ranges.c")
        )
        report = inventory_display_lists(rom, inventory_pools(rom, worlds))
        expected = {
            "texture": (736, 42528, 2480, 1889168, 1),
            "state": (409, 33696, 0, 48, 0),
            "geometry": (4638, 231584, 64, 671872, 1),
        }
        for pool in report["pools"]:
            self.assertEqual(
                (
                    pool["chunk_count"],
                    pool["command_bytes"],
                    pool["leading_payload_bytes"],
                    pool["trailing_payload_bytes"],
                    pool["nonzero_command_offsets"],
                ),
                expected[pool["name"]],
            )
        geometry = next(item for item in report["pools"] if item["name"] == "geometry")
        self.assertEqual(geometry["opcode_totals"]["G_ENDDL"], 4638)
        self.assertEqual(geometry["opcode_totals"]["G_VTX"], 4899)
        self.assertEqual(geometry["opcode_totals"]["G_TRI1"], 19411)


if __name__ == "__main__":
    unittest.main()
