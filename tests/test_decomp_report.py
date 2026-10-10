import importlib.util
import json
import re
import tempfile
import tomllib
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "generate_decomp_report", ROOT / "tools" / "generate_decomp_report.py"
)
REPORT = importlib.util.module_from_spec(SPEC)
assert SPEC.loader is not None
SPEC.loader.exec_module(REPORT)


class DecompReportTests(unittest.TestCase):
    def test_readme_progress_matches_report(self):
        report = REPORT.build_report(
            ROOT / "config/us/recomp_function_boundaries.toml",
            ROOT / "config/us/splat.yaml",
        )
        measures = report["measures"]
        readme = (ROOT / "README.md").read_text()
        function_status = re.search(
            r"([\d,]+) of ([\d,]+) catalogue functions .*?"
            r"([\d,]+) of ([\d,]+) bytes",
            readme,
        )
        data_status = re.search(
            r"data accounts for ([\d,]+) of ([\d,]+) bytes", readme
        )
        self.assertIsNotNone(function_status)
        self.assertIsNotNone(data_status)
        self.assertEqual(
            tuple(int(value.replace(",", "")) for value in function_status.groups()),
            (
                measures["matched_functions"],
                measures["total_functions"],
                int(measures["matched_code"]),
                int(measures["total_code"]),
            ),
        )
        self.assertEqual(
            tuple(int(value.replace(",", "")) for value in data_status.groups()),
            (int(measures["matched_data"]), int(measures["total_data"])),
        )

    def test_current_production_layout(self):
        report = REPORT.build_report(
            ROOT / "config/us/recomp_function_boundaries.toml",
            ROOT / "config/us/splat.yaml",
        )
        measures = report["measures"]
        self.assertEqual(report["version"], 2)
        self.assertEqual(measures["total_functions"], 1879)
        self.assertEqual(measures["matched_functions"], 1684)
        self.assertEqual(measures["total_code"], "632132")
        self.assertEqual(measures["matched_code"], "493332")
        self.assertEqual(measures["total_data"], "115837")
        self.assertEqual(measures["matched_data"], "111769")
        self.assertEqual([item["name"] for item in report["categories"]], ["Code", "Data"])
        data = report["categories"][1]["measures"]
        self.assertAlmostEqual(data["matched_data_percent"], 111769 * 100 / 115837)
        unmatched = [
            unit for unit in report["units"]
            if unit["name"].startswith("data/unmatched_")
        ]
        self.assertEqual(len(unmatched), 81)
        self.assertEqual(
            sum(int(unit["sections"][0]["size"]) for unit in unmatched), 4068
        )
        self.assertTrue(
            all("virtual_address" in unit["sections"][0]["metadata"] for unit in unmatched)
        )
        self.assertEqual(
            sum(
                unit["measures"]["total_functions"]
                for unit in report["units"]
                if "code" in unit["metadata"]["progress_categories"]
            ),
            measures["total_functions"],
        )

    def test_owned_data_ranges_do_not_overlap(self):
        ranges = REPORT.load_owned_data(
            [ROOT / "config/us/unit_rodata.tsv", ROOT / "config/us/unit_data.tsv"]
        )
        self.assertEqual(sum(item["size"] for item in ranges), 111769)
        self.assertTrue(
            all(
                left["end"] <= right["address"]
                for left, right in zip(ranges, ranges[1:])
            )
        )

    def test_alignment_fill_is_excluded_from_data_progress(self):
        exclusions = REPORT.load_data_exclusions(ROOT / "config/us/data_exclusions.tsv")
        self.assertEqual(len(exclusions), 151)
        self.assertEqual(sum(item["size"] for item in exclusions), 1135)

    def test_report_is_json_serializable_with_string_u64_fields(self):
        report = REPORT.build_report(
            ROOT / "config/us/recomp_function_boundaries.toml",
            ROOT / "config/us/splat.yaml",
        )
        encoded = json.dumps(report)
        decoded = json.loads(encoded)
        self.assertIsInstance(decoded["measures"]["total_code"], str)
        self.assertIsInstance(decoded["units"][0]["functions"][0]["address"], str)

    def test_data_scope_is_the_loaded_main_image(self):
        self.assertEqual(REPORT.MAIN_IMAGE_START, 0x1000)
        self.assertEqual(REPORT.MAIN_IMAGE_END, 0xB7E30)
        self.assertEqual(
            REPORT.MAIN_IMAGE_END
            - REPORT.MAIN_IMAGE_START
            - 632132
            - 1135,
            115837,
        )

    def test_late_gameplay_functions_are_not_reported_as_data(self):
        report = REPORT.build_report(
            ROOT / "config/us/recomp_function_boundaries.toml",
            ROOT / "config/us/splat.yaml",
        )
        expected = {
            0x800DA984,
            0x800DE4DC,
            0x800E18D8,
            0x800E1980,
            0x800E3404,
            0x800E44C8,
            0x800E6040,
            0x800E7968,
        }
        functions = REPORT.load_functions(
            ROOT / "config/us/recomp_function_boundaries.toml"
        )
        self.assertTrue(expected.issubset({item["vram"] for item in functions}))

        for unit in report["units"]:
            if not unit["name"].startswith("data/unmatched_"):
                continue
            section = unit["sections"][0]
            start = int(section["metadata"]["virtual_address"], 0)
            end = start + int(section["size"])
            self.assertFalse(any(start <= address < end for address in expected))

    def test_verified_manual_function_boundaries_are_catalogued(self):
        functions = REPORT.load_functions(
            ROOT / "config/us/recomp_function_boundaries.toml"
        )
        catalogue_addresses = {item["vram"] for item in functions}
        with (ROOT / "config/us/manual_symbols.toml").open("rb") as stream:
            manual_symbols = tomllib.load(stream)["symbol"]

        verified_addresses = {
            item["vram"]
            for item in manual_symbols
            if item.get("type") == "func"
            and (
                "byte-exact C build" in item.get("reason", "")
                or "Matched C TU" in item.get("reason", "")
            )
        }
        self.assertTrue(verified_addresses.issubset(catalogue_addresses))

        # These three functions predate the standardized verification reason,
        # but their current production units are also byte-exact.
        self.assertTrue(
            {0x800D09E0, 0x800DD6F4, 0x800F6100}.issubset(catalogue_addresses)
        )

    def test_function_catalogue_has_no_overlaps(self):
        functions = REPORT.load_functions(
            ROOT / "config/us/recomp_function_boundaries.toml"
        )
        self.assertTrue(
            all(
                left["vram"] + left["size"] <= right["vram"]
                for left, right in zip(functions, functions[1:])
            )
        )


if __name__ == "__main__":
    unittest.main()
