from __future__ import annotations

import tomllib
import unittest
from pathlib import Path


BOUNDARIES = Path("config/us/recomp_function_boundaries.toml")
LAST_VERIFIED_CODE_END = 0x80114498


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


if __name__ == "__main__":
    unittest.main()
