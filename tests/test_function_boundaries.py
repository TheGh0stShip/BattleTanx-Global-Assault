from __future__ import annotations

import tomllib
import unittest
from pathlib import Path


BOUNDARIES = Path("config/us/recomp_function_boundaries.toml")
LAST_VERIFIED_CODE_END = 0x8011449C


class FunctionBoundaryTests(unittest.TestCase):
    @staticmethod
    def load_functions() -> list[dict[str, object]]:
        document = tomllib.loads(BOUNDARIES.read_text())
        return [
            function
            for section in document["section"]
            for function in section.get("functions", ())
        ]

    def test_imported_functions_do_not_extend_into_high_address_data(self) -> None:
        functions = self.load_functions()
        high_functions = [
            function for function in functions if function["vram"] >= LAST_VERIFIED_CODE_END
        ]
        self.assertEqual([], high_functions)

    def test_contquery_interior_signature_is_not_a_function(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        self.assertNotIn(0x801030D4, by_address)
        self.assertEqual(
            {"name": "osContStartQuery", "vram": 0x801030B0, "size": 0x84},
            by_address[0x801030B0],
        )

    def test_pi_cmd_queue_interior_signature_is_not_a_function(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        self.assertNotIn(0x8010CF88, by_address)
        self.assertEqual(
            {"name": "osPiGetCmdQueue", "vram": 0x8010CF70, "size": 0x28},
            by_address[0x8010CF70],
        )

    def test_pi_access_queue_object_uses_pi_symbols(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        expected = {
            0x8010D680: ("__osPiCreateAccessQueue", 0x50),
            0x8010D6D0: ("__osPiGetAccess", 0x44),
            0x8010D714: ("__osPiRelAccess", 0x2C),
        }
        for address, (name, size) in expected.items():
            self.assertEqual(
                {"name": name, "vram": address, "size": size},
                by_address[address],
            )

    def test_scheduler_noop_stubs_are_independent_functions(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        expected = {
            0x8010F014: "func_8010F014",
            0x8010F01C: "func_8010F01C",
            0x8010F024: "func_8010F024",
            0x8010F02C: "func_8010F02C",
        }
        for address, name in expected.items():
            self.assertEqual(
                {"name": name, "vram": address, "size": 0x8},
                by_address[address],
            )
        self.assertNotIn("ptstart", {function["name"] for function in functions})

    def test_synthesizer_object_stubs_are_independent_functions(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        expected = {
            0x80110750: ("alSynDelete_80110750", 0x10),
            0x80110760: ("func_80110760", 0x8),
            0x80110870: ("__allocParam_80110870", 0x30),
            0x801108A0: ("func_801108A0", 0x8),
            0x801108A8: ("alAudioFrame", 0x298),
        }
        for address, (name, size) in expected.items():
            self.assertEqual(
                {"name": name, "vram": address, "size": size},
                by_address[address],
            )

    def test_late_sdk_static_symbols_use_source_backed_names(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        expected = {
            0x80110020: ("proutSprintf", 0x24),
            0x801100E0: ("_VirtualToPhysicalTask", 0x11C),
            0x801116E8: ("viMgrMain", 0x1D8),
            0x801118C0: ("__osViGetCurrentContext", 0x10),
        }
        for address, (name, size) in expected.items():
            self.assertEqual(
                {"name": name, "vram": address, "size": size},
                by_address[address],
            )

    def test_formatted_io_static_boundaries(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        expected = {
            0x80112110: ("_Putfld", 0x670),
            0x80112DD0: ("_Genld", 0x568),
            0x80113338: ("func_80113338", 0x8),
            0x80113340: ("_Ldtob", 0x550),
        }
        for address, (name, size) in expected.items():
            self.assertEqual(
                {"name": name, "vram": address, "size": size},
                by_address[address],
            )

    def test_final_libgcc_helpers_exclude_embedded_data(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        expected = {
            0x80113D10: ("__cmpdi2", 0x40),
            0x80113D50: ("__floatdisf", 0xBC),
            0x80113E10: ("__udivdi3", 0x20),
            0x80113E30: ("__udivmoddi4", 0x63C),
            0x80114470: ("__umoddi3", 0x2C),
        }
        for address, (name, size) in expected.items():
            self.assertEqual(
                {"name": name, "vram": address, "size": size},
                by_address[address],
            )

        covered_words = {
            address
            for function in functions
            for address in range(
                int(function["vram"]),
                int(function["vram"]) + int(function["size"]),
                4,
            )
        }
        self.assertNotIn(0x80113E0C, covered_words)
        self.assertNotIn(0x8011446C, covered_words)
        self.assertIn(0x80114498, covered_words)

    def test_game_code_merged_boundaries_are_split(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}

        expected = {
            0x8007A720: 0x3C, 0x8007A75C: 0x58, 0x8007A7B4: 0x64,
            0x8007A8F0: 0xFC, 0x8007A9EC: 0xBC, 0x8007AAA8: 0xBC,
            0x8007AC34: 0x38, 0x8007AC6C: 0x38, 0x8007ACA4: 0x54,
            0x8007ADC0: 0x20, 0x8007ADE0: 0x10,
            0x8007C364: 0x3B4, 0x8007C718: 0x2A0,
            0x8007D4A0: 0x1C, 0x8007D4BC: 0x44,
            0x8007D69C: 0x68, 0x8007D710: 0x8, 0x8007D720: 0x18,
            0x8007D738: 0x28, 0x8007D760: 0x4C, 0x8007D7AC: 0x18,
            0x8007D7C4: 0xC0,
            0x8007DBE0: 0x174, 0x8007DD54: 0xE8,
            0x8007E210: 0x58, 0x8007E268: 0x6C,
            0x8007E118: 0x4C,
            0x8007E64C: 0x12C, 0x8007E778: 0xC,
            0x80080920: 0x1D4, 0x80080AF4: 0x64,
            0x80080B58: 0x64,
            0x80081FD8: 0x2C, 0x80082004: 0x6C,
            0x80082560: 0x44, 0x800825A4: 0xE8,
            0x80082A90: 0xAC, 0x80082B40: 0x20,
            0x80082B60: 0x8, 0x80082B68: 0x6C,
            0x80082BD4: 0x48, 0x80082C1C: 0xEC,
            0x80082D60: 0x8, 0x80082D68: 0x30,
            0x80083DF0: 0xCC, 0x80083EBC: 0xB8,
            0x80083F74: 0x58, 0x80083FF8: 0x1DC,
            0x800841D4: 0x50,
            0x8008474C: 0x120, 0x8008486C: 0x7C,
            0x800848E8: 0x368, 0x80084C50: 0x28,
            0x80084C78: 0x50, 0x80084CC8: 0x480,
            0x80085148: 0x44, 0x8008518C: 0xC4,
            0x80085250: 0xA8,
            0x80086700: 0xC4, 0x800867C4: 0x7C,
            0x80086A20: 0x190, 0x80086BB0: 0x8,
            0x80086BB8: 0x34, 0x80086BEC: 0x90,
            0x80086C7C: 0x70, 0x8008723C: 0x18C,
            0x80087570: 0x228, 0x80087798: 0x8C,
            0x80087824: 0x1D4, 0x800879F8: 0x88,
            0x80088720: 0x3C, 0x8008875C: 0xF8,
            0x80088B6C: 0xF8,
            0x80089154: 0x18, 0x8008916C: 0x94,
            0x80089E20: 0x8, 0x80089E28: 0x20,
            0x80089E48: 0x3C,
            0x8008A1A4: 0x1A0, 0x8008A344: 0xC,
            0x8008A350: 0x50, 0x8008A3A0: 0x58,
            0x8008A3F8: 0x1EC, 0x8008A5E4: 0x48,
            0x8008A62C: 0x138, 0x8008A764: 0x12C,
            0x8008A890: 0x34, 0x8008A8C4: 0x15C,
            0x8008AA20: 0x128,
            0x8008B6D0: 0xB8, 0x8008B788: 0xA4,
            0x8008BD5C: 0x118, 0x8008BE74: 0x50,
            0x8008BEC4: 0x98,
            0x8008C3B4: 0x208, 0x8008C5BC: 0x1C,
        }
        for address, size in expected.items():
            self.assertEqual(size, by_address[address]["size"])
            self.assertEqual(f"func_{address:08X}", by_address[address]["name"])

    def test_game_code_trailing_data_is_not_function_coverage(self) -> None:
        functions = self.load_functions()
        by_address = {function["vram"]: function for function in functions}
        self.assertEqual(0x98, by_address[0x8007AF84]["size"])
        self.assertEqual(0xCC, by_address[0x8007D39C]["size"])
        self.assertEqual(0x14, by_address[0x8007D484]["size"])

        covered_words = {
            address
            for function in functions
            for address in range(
                int(function["vram"]),
                int(function["vram"]) + int(function["size"]),
                4,
            )
        }
        for address in (0x8007B01C, 0x8007D468, 0x8007D46C,
                        0x8007D498, 0x8007D49C, 0x8007D704,
                        0x8007D708, 0x8007D70C, 0x8007D718,
                        0x8007D71C, 0x8007E164, 0x8007E168,
                        0x8007E16C, 0x80080BBC, 0x80082B3C,
                        0x80082D08, 0x80082D0C):
            self.assertNotIn(address, covered_words)


if __name__ == "__main__":
    unittest.main()
