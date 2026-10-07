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


if __name__ == "__main__":
    unittest.main()
