import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_script_assets import build_script, inventory_scripts, parse_script  # noqa: E402
from pack_script_assets import rebuild_script  # noqa: E402


class ScriptAssetInventoryTests(unittest.TestCase):
    def test_minimal_script(self):
        # Scene 1, setting 2, one stream: type 10, wait 3 ticks, end.
        data = bytes((1, 2, 0, 1, 8, 10, 1, 0, 3, 0))
        parsed = parse_script(data)
        self.assertEqual(parsed["stream_count"], 1)
        self.assertEqual(parsed["streams"][0]["type"], 10)
        self.assertEqual(parsed["streams"][0]["command_count"], 3)

    def test_rebuilds_split_script(self):
        data = bytes((1, 2, 0, 1, 8, 10, 1, 0, 3, 0))
        parsed = parse_script(data)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            root.joinpath("header.bin").write_bytes(data[:4])
            root.joinpath("stream_000.bin").write_bytes(data[4:])
            self.assertEqual(rebuild_script(root, parsed), data)

    def test_json_script_rebuilds_exactly(self):
        data = bytes((1, 2, 0, 1, 8, 10, 1, 0, 3, 0))
        self.assertEqual(build_script(parse_script(data)), data)

    def test_json_text_command_is_editable(self):
        data = bytes((1, 2, 0, 1, 8, 10, 21)) + b"OLD\0\0"
        parsed = parse_script(data)
        parsed["streams"][0]["commands"][1]["text"] = "NEW TEXT"
        rebuilt = build_script(parsed)
        self.assertIn(b"NEW TEXT\0", rebuilt)
        self.assertEqual(parse_script(rebuilt)["streams"][0]["commands"][1]["text"], "NEW TEXT")

    def test_fixed_width_command_fields_are_editable(self):
        data = bytes((1, 2, 0, 1, 8, 10, 20, 0xFF, 0xFF, 0x00, 0, 0, 2, 0))
        parsed = parse_script(data)
        command = parsed["streams"][0]["commands"][1]
        self.assertEqual(command["fields"], {"x": -256, "y": 2})
        command["fields"]["x"] = 0x12345
        rebuilt = build_script(parsed)
        self.assertEqual(
            parse_script(rebuilt)["streams"][0]["commands"][1]["fields"]["x"],
            0x12345,
        )

    def test_variable_width_command_fields_are_editable(self):
        # Opcode 3: x is signed 8-bit, y signed 16-bit, z signed 24-bit,
        # followed by both optional 16-bit parameters.
        data = bytes.fromhex("01020001080A03F9FEFFFDFFFFFC1234567800")
        parsed = parse_script(data)
        command = parsed["streams"][0]["commands"][1]
        self.assertEqual(
            command["fields"],
            {
                "mode": 0xF9,
                "x": -2,
                "y": -3,
                "z": -4,
                "parameter_40": 0x1234,
                "parameter_80": 0x5678,
            },
        )
        command["fields"]["z"] = 0x12345
        rebuilt = build_script(parsed)
        self.assertEqual(
            parse_script(rebuilt)["streams"][0]["commands"][1]["fields"]["z"],
            0x12345,
        )

    def test_current_rom_scripts(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        report = inventory_scripts(
            rom_path.read_bytes(), ROOT / "src/code/campaign_mission_config.c"
        )
        self.assertEqual(report["script_count"], 17)
        self.assertEqual(report["stream_count"], 249)
        self.assertEqual(report["command_count"], 144298)
        self.assertEqual(report["opcode_counts"]["8"], 249)
        self.assertEqual(report["opcode_counts"]["0"], 249)
        self.assertEqual(report["opcode_counts"]["47"], 16)


if __name__ == "__main__":
    unittest.main()
