from __future__ import annotations

import tomllib
import unittest
from pathlib import Path


BOUNDARIES = Path("config/us/recomp_function_boundaries.toml")
LAST_VERIFIED_CODE_END = 0x80114498


class FunctionBoundaryTests(unittest.TestCase):
    def test_imported_functions_do_not_extend_into_high_address_data(self) -> None:
        document = tomllib.loads(BOUNDARIES.read_text())
        functions = [
            function
            for section in document["section"]
            for function in section.get("functions", ())
        ]
        high_functions = [
            function for function in functions if function["vram"] >= LAST_VERIFIED_CODE_END
        ]
        self.assertEqual([], high_functions)


if __name__ == "__main__":
    unittest.main()
