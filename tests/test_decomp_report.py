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
        self.assertEqual(measures["total_functions"], 1684)
        self.assertEqual(measures["matched_functions"], 1175)
        self.assertEqual(measures["total_code"], "632176")
        self.assertEqual(measures["matched_code"], "285768")
        self.assertEqual(
            sum(unit["measures"]["total_functions"] for unit in report["units"]),
            measures["total_functions"],
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


if __name__ == "__main__":
    unittest.main()
