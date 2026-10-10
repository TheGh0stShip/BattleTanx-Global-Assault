import importlib.util
from pathlib import Path
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location("run_decomp_campaign", ROOT / "tools/run_decomp_campaign.py")
CAMPAIGN = importlib.util.module_from_spec(SPEC)
assert SPEC.loader is not None
SPEC.loader.exec_module(CAMPAIGN)


class DecompCampaignTests(unittest.TestCase):
    def test_diff_is_not_misclassified_as_process_error(self):
        self.assertEqual(CAMPAIGN.parse_result("RESULT x.c: DIFF(7)\n", 1), ("DIFF", 7))

    def test_result_diff_uses_preceding_function_count(self):
        output = "FUNC 80090000 func_80090000 0x40 DIFF(3)\nRESULT x.c: DIFF\n"
        self.assertEqual(CAMPAIGN.parse_result(output, 1), ("DIFF", 3))

    def test_whole_unit_score_wins_over_isolated_function_score(self):
        output = (
            ".text 800B06E0..800B22A4: 1355 mismatched words\n"
            "FUNC 800B129C func_800B129C 0x20C DIFF(25)\n"
            "RESULT x.c: DIFF\n"
        )
        self.assertEqual(CAMPAIGN.parse_result(output, 1), ("DIFF", 1355))

    def test_unit_match_with_nonzero_status_is_still_a_match(self):
        self.assertEqual(CAMPAIGN.parse_result("RESULT x.c: UNIT MATCH\n", 1), ("MATCH", 0))

    def test_zero_byte_unit_match_is_invalid(self):
        output = (
            ".text 80080CC8..80080CC8: 0 mismatched words\n"
            "RESULT func_80080CC8.c: UNIT MATCH\n"
        )
        self.assertEqual(CAMPAIGN.parse_result(output, 0), ("INVALID", None))

    def test_production_owner_uses_current_segment(self):
        segments = [(0x20000, "asm", "before"), (0x20C4C, "c", "code/tank_contact_scan"), (0x20DCC, "asm", "after")]
        self.assertEqual(
            CAMPAIGN.production_owner("80090C4C", segments),
            ("c", "code/tank_contact_scan"),
        )

    def test_production_state_finds_inline_assembly(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / "src/code/example.c"
            source.parent.mkdir(parents=True)
            source.write_text('void f(void) { __asm__("nop"); }\n')
            self.assertEqual(
                CAMPAIGN.production_state(root, "c", "code/example"),
                "c_inline_asm",
            )

    def test_production_state_distinguishes_clean_c_and_asm_segments(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / "src/code/example.c"
            source.parent.mkdir(parents=True)
            source.write_text("void f(void) {}\n")
            self.assertEqual(CAMPAIGN.production_state(root, "c", "code/example"), "c_clean")
            self.assertEqual(CAMPAIGN.production_state(root, "asm", "example"), "asm")

    def test_candidate_source_state_rejects_matching_tricks(self):
        self.assertEqual(
            CAMPAIGN.c_source_state('register void *q asm("$4");\n'),
            "c_register_asm",
        )
        self.assertEqual(
            CAMPAIGN.c_source_state('void f(void) { asm("nop"); }\n'),
            "c_inline_asm",
        )
        self.assertEqual(
            CAMPAIGN.c_source_state("volatile int x;\n"),
            "c_volatile",
        )
        self.assertEqual(CAMPAIGN.c_source_state("int x;\n"), "c_clean")

    def test_search_candidate_ignores_function_prototypes(self):
        source = "void func_80080000(void);\nvoid func_80090000(void) { func_80080000(); }\n"
        match = CAMPAIGN.first_function_definition(source)
        self.assertIsNotNone(match)
        self.assertEqual(match.group(1), "func_80090000")

    def test_discovery_ignores_missing_candidates(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            lane = root / "lane"
            (lane / "tools").mkdir(parents=True)
            (lane / "tools/kmc_cmp.py").write_text("# comparator\n")
            (lane / "candidate.c").write_text("void func_80090000(void) {}\n")
            (lane / "NEAR_MISSES.tsv").write_text(
                "function\tbest_candidate\tmismatch_words\n"
                "func_80090000\tcandidate.c\t3\n"
                "func_80090004\tmissing.c\t2\n"
            )
            jobs = CAMPAIGN.discover(root)
            self.assertEqual(len(jobs), 1)
            self.assertEqual(jobs[0]["target"], "80090000")


if __name__ == "__main__":
    unittest.main()
