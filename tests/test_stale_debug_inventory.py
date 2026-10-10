import sys
import unittest
import csv
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from inventory_stale_debug import (  # noqa: E402
    FUNCTION_RECORDS_START,
    FUNCTION_TEXT_BASE,
    inventory_debug_names,
    inventory_function_records,
    inventory_type_only_records,
)


class StaleDebugInventoryTests(unittest.TestCase):
    def test_retail_debug_name_inventory(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        report = inventory_debug_names(rom_path.read_bytes())
        self.assertEqual(report["size"], 0x7C00)
        self.assertEqual(report["record_count"], 1305)
        self.assertEqual(report["unique_name_count"], 664)
        self.assertIn("Steps_PruneFork", report["unique_names"])
        self.assertIn("Obstacles_CopyObstacleRef", report["unique_names"])

    def test_retail_function_record_chain(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        records = inventory_function_records(rom_path.read_bytes())
        expected = [
            ("Steps_PruneFork", 12, 2, 64),
            ("Steps_InitStep_Leg", 15, 2, 356),
            ("Steps_NewStep", 16, 2, 632),
            ("Steps_InitStepPool", 21, 2, 1216),
            ("Steps_FreeBranch", 26, 2, 2552),
            ("Steps_CopyObstacleRef", 18, 2, 876),
            ("Steps_GetNextStepId", 23, 2, 1820),
            ("Steps_PruneForksBelow", 19, 2, 940),
            ("Steps_GetNextLegPtr", 10, 2, 0),
            ("Steps_InitStep_Free", 17, 2, 828),
            ("Steps_StepPtrFromId", 25, 2, 2308),
            ("Steps_SpliceIn", 20, 2, 1124),
            ("Steps_FreeStep", 11, 2, 24),
            ("Steps_InitStep_Fork", 14, 2, 164),
            ("Steps_InitStep", 24, 2, 2176),
            ("Steps_CropLinearBranch", 13, 2, 140),
        ]
        self.assertEqual(records[0]["record_offset"], FUNCTION_RECORDS_START)
        self.assertEqual(
            [
                (r["name"], r["type_index"], r["section"], r["value"])
                for r in records
            ],
            expected,
        )
        self.assertTrue(all(r["record_class"] == 0x0C for r in records))
        self.assertEqual(records[0]["retail_address"], FUNCTION_TEXT_BASE + 0x40)
        self.assertEqual(records[0]["legacy_symbol"], "func_8007D760")
        self.assertEqual(
            {r["retail_address"] for r in records},
            {
                0x8007D720,
                0x8007D738,
                0x8007D760,
                0x8007D7AC,
                0x8007D7C4,
                0x8007D884,
                0x8007D998,
                0x8007DA5C,
                0x8007DA8C,
                0x8007DACC,
                0x8007DB84,
                0x8007DBE0,
                0x8007DE3C,
                0x8007DFA0,
                0x8007E024,
                0x8007E118,
            },
        )

    def test_retail_type_only_record_chain(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        rom = rom_path.read_bytes()
        functions = inventory_function_records(rom)
        final = functions[-1]
        offset = final["record_offset"] + 1 + len(final["name"]) + 9
        records = inventory_type_only_records(rom, offset)
        self.assertEqual(
            [(r["name"], r["type_index"]) for r in records],
            [
                ("Steps_InitStep_Arrival", 27),
                ("Obstacles_InitObstacleRef", 29),
                ("Obstacles_CopyObstacleRef", 28),
            ],
        )
        self.assertTrue(all(r["record_class"] == 0x0E for r in records))

    def test_recovered_symbol_map_matches_retail_records(self):
        rom_path = ROOT / "baseroms/us/baserom.z64"
        if not rom_path.exists():
            self.skipTest("base ROM is unavailable")
        records = inventory_function_records(rom_path.read_bytes())
        with (ROOT / "config/us/recovered_debug_symbols.tsv").open(newline="") as stream:
            rows = list(csv.DictReader(stream, delimiter="\t"))
        self.assertEqual(len(rows), 16)
        by_address = {int(row["address"], 0): row for row in rows}
        self.assertEqual(set(by_address), {r["retail_address"] for r in records})
        for record in records:
            row = by_address[record["retail_address"]]
            self.assertEqual(row["legacy_symbol"], record["legacy_symbol"])
            self.assertEqual(row["recovered_symbol"], record["name"])
            self.assertEqual(int(row["section_offset"], 0), record["value"])
            self.assertEqual(int(row["type_index"], 0), record["type_index"])


if __name__ == "__main__":
    unittest.main()
