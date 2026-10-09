import importlib.util
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[1]
MODULE_PATH = ROOT / "tools" / "normalize_kmc_gcc_asm.py"
SPEC = importlib.util.spec_from_file_location("normalize_kmc_gcc_asm", MODULE_PATH)
assert SPEC is not None and SPEC.loader is not None
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class KmcPipelineTests(unittest.TestCase):
    def test_preserves_compiler_filled_delay_slot(self) -> None:
        source = (
            "\t.set\tnoreorder\n"
            "\tjal\t_bzero\n"
            "\tli\t$5,0x300\n"
            "\t.set\treorder\n"
        )
        self.assertEqual(
            (
                "\t.set\tnoreorder\n"
                "\t.set\tnoreorder\n"
                "\tjal\t_bzero\n"
                "\tli\t$5,0x300\n"
                "\t.set\tnoreorder\n"
            ),
            MODULE.normalize(source),
        )

    def test_adds_nop_to_unfilled_reorder_transfer(self) -> None:
        source = "\tsb\t$4,D_80114512\n\tjal\tfunc_800A9F10\n\tlw\t$31,20($sp)\n"
        self.assertEqual(
            "\t.set\tnoreorder\n\tsb\t$4,D_80114512\n"
            "\tjal\tfunc_800A9F10\n\tnop\n\tlw\t$31,20($sp)\n",
            MODULE.normalize(source),
        )

    def test_global_noreorder_prevents_store_scheduling(self) -> None:
        source = "\tsw\t$4,D_80114C80\n\tj\t$31\n"
        self.assertEqual(
            "\t.set\tnoreorder\n\tsw\t$4,D_80114C80\n\tj\t$31\n\tnop\n",
            MODULE.normalize(source),
        )

    def test_uses_retail_encoding_for_long_shift_helper_branch(self) -> None:
        source = "\t.set\tnoreorder\n\tb\t3f\n\tmove\t$12,$0\n"
        self.assertEqual(
            "\t.set\tnoreorder\n\t.set\tnoreorder\n"
            "\tbgez\t$zero,3f\n\tmove\t$12,$0\n",
            MODULE.normalize(source),
        )

    def test_materializes_fp_compare_hazard_but_not_load_placeholder(self) -> None:
        source = (
            "\tl.s\t$f0,0($4)\n"
            "\t#nop\n"
            "\tc.le.s\t$f4,$f0\n"
            "\t#nop\n"
            "\tbc1t\t.L1\n"
        )
        self.assertEqual(
            "\t.set\tnoreorder\n"
            "\tl.s\t$f0,0($4)\n"
            "\t#nop\n"
            "\tc.le.s\t$f4,$f0\n"
            "\tnop\n"
            "\tbc1t\t.L1\n"
            "\tnop\n",
            MODULE.normalize(source),
        )

    def test_uses_at_for_indexed_symbol_load(self) -> None:
        source = "\tlbu\t$2,D_801146D4+8($5)\n"
        self.assertEqual(
            "\t.set\tnoreorder\n"
            "\tlui\t$at,%hi(D_801146D4+8)\n"
            "\taddu\t$at,$at,$5\n"
            "\tlbu\t$2,%lo(D_801146D4+8)($at)\n",
            MODULE.normalize(source),
        )

    def test_resource_copy_prologue_reorders_once(self) -> None:
        source = (
            "func_8007E118:\n"
            "\tsubu\t$sp,$sp,24\n"
            "\tsw\t$31,20($sp)\n"
            "\tsw\t$16,16($sp)\n"
            "\tlbu\t$3,0($4)\n"
            "\tli\t$2,0x00000002\t\t# 2\n"
            "\t.set\tnoreorder\n"
            "\tbne\t$3,$2,.L2\n"
            "\tmove\t$16,$5\n"
        )
        normalized = MODULE.normalize_v3(source)
        expected = (
            "\tlbu\t$3,0($4)\n"
            "\tsubu\t$sp,$sp,24\n"
            "\tsw\t$16,16($sp)\n"
            "\tmove\t$16,$5\n"
            "\tli\t$2,0x00000002\t\t# 2\n"
            "\t.set\tnoreorder\n"
            "\tbne\t$3,$2,.L2\n"
            "\tsw\t$31,20($sp)\n"
        )
        self.assertIn(expected, normalized)

    def test_resource_copy_prologue_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_8007E118:\n\tnop\n")

    def test_owner_search_prologue_reorders_once(self) -> None:
        source = (
            "func_80083DF0:\n"
            "\tsw\t$31,24($sp)\n"
            "\tsw\t$17,20($sp)\n"
        )
        normalized = MODULE.normalize_v3(source)
        self.assertIn(
            "\tsw\t$17,20($sp)\n\tsw\t$31,24($sp)\n",
            normalized,
        )

    def test_owner_search_prologue_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_80083DF0:\n\tnop\n")

    def test_object_phase_lookup_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_object_phase_lookup("func_800A8E84:\n\tnop\n")

    def test_display_slot_wait_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_display_slot_wait("func_8007A818:\n\tnop\n")

    def test_display_record_prefix_requires_one_pattern(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.schedule_display_record_prefix("func_8007A8F0:\n\tnop\n")


if __name__ == "__main__":
    unittest.main()
