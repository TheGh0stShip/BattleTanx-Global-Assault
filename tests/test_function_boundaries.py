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


if __name__ == "__main__":
    unittest.main()
