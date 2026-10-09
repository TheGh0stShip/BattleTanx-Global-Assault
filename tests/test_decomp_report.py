import importlib.util
import json
import tempfile
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
    def test_current_production_layout(self):
        report = REPORT.build_report(
            ROOT / "config/us/recomp_function_boundaries.toml",
            ROOT / "config/us/splat.yaml",
        )
        measures = report["measures"]
        self.assertEqual(report["version"], 2)
        self.assertEqual(measures["total_functions"], 1733)
        self.assertEqual(measures["matched_functions"], 1398)
        self.assertEqual(measures["total_code"], "630540")
        self.assertEqual(measures["matched_code"], "370404")
        self.assertEqual(measures["total_data"], "118564")
        self.assertEqual(measures["matched_data"], "46032")
        self.assertEqual([item["name"] for item in report["categories"]], ["Code", "Data"])
        data = report["categories"][1]["measures"]
        self.assertAlmostEqual(data["matched_data_percent"], 46032 * 100 / 118564)
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
        self.assertEqual(sum(item["size"] for item in ranges), 46032)
        self.assertTrue(
            all(
                left["end"] <= right["address"]
                for left, right in zip(ranges, ranges[1:])
            )
        )

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
            - 630540,
            118564,
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
