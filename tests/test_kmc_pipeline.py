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

    def test_materializes_hilo_hazard_with_one_intervening_instruction(self) -> None:
        source = (
            "\tmfhi\t$4\n"
            "\t#nop\n"
            "\taddu\t$2,$2,$17\n"
            "\tmultu\t$2,$3\n"
        )
        normalized = MODULE.normalize_v3(source)
        self.assertIn(
            "\tmfhi\t$4\n\taddu\t$2,$2,$17\n\tnop\n\tmultu\t$2,$3\n",
            normalized,
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

    def test_region_early_exit_renames_one_branch(self) -> None:
        source = (
            "func_800AA5D0:\n"
            "\tc.le.s\t$f4,$f6\n"
            "\tnop\n"
            "\t.set\tnoreorder\n"
            "\tbc1t\t.L7\n"
            "\tmove\t$2,$0\n"
        )
        normalized = MODULE.normalize_v3(source)
        self.assertIn("\tbc1tl\t.L7\n\tmove\t$2,$0\n", normalized)

    def test_region_early_exit_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_800AA5D0:\n\tnop\n")

    def test_vector_angle_prologue_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_8009DFAC:\n\tnop\n")

    def test_selection_state_dispatch_requires_one_tree(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_8009ACDC:\n\tnop\n")

    def test_snapshot_record_address_rewrites_once(self) -> None:
        source = (
            "func_80079FF0:\n"
            "\tmove\t$4,$3\n"
            "\tsll\t$2,$4,4\n"
            "\tlw\t$4,0($fp)\n"
            "\taddu\t$3,$4,144\n"
            "\taddu\t$2,$2,$3\n"
            "\tsw\t$2,8($fp)\n"
        )
        normalized = MODULE.normalize_v3(source)
        self.assertIn(
            "\tmove\t$2,$3\n\tsll\t$3,$2,4\n"
            "\taddu\t$2,$3,144\n\tlw\t$3,0($fp)\n"
            "\taddu\t$2,$3,$2\n\tsw\t$2,8($fp)\n",
            normalized,
        )

    def test_snapshot_record_address_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_80079FF0:\n\tnop\n")

    def test_large_search_prologue_reorders_once(self) -> None:
        source = (
            "func_800A2B9C:\n"
            "\tsw\t$16,1184($sp)\n"
            "\taddu\t$16,$sp,24\n"
            "\tandi\t$4,$4,0xffff\n"
            "\tli\t$5,0x00400000\t\t# 4194304\n"
            "\tori\t$5,$5,0x1100\n"
        )
        normalized = MODULE.normalize_v3(source)
        self.assertIn(
            "\tandi\t$4,$4,0xffff\n"
            "\tli\t$5,0x00400000\t\t# 4194304\n"
            "\tori\t$5,$5,0x1100\n"
            "\tsw\t$16,1184($sp)\n"
            "\taddu\t$16,$sp,24\n",
            normalized,
        )

    def test_large_search_prologue_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_800A2B9C:\n\tnop\n")

    def test_region_query_schedule_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_800ACFE0:\n\tnop\n")

    def test_object_phase_lookup_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_object_phase_lookup("func_800A8E84:\n\tnop\n")

    def test_display_slot_wait_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_display_slot_wait("func_8007A818:\n\tnop\n")

    def test_display_record_prefix_requires_one_pattern(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.schedule_display_record_prefix("func_8007A8F0:\n\tnop\n")

    def test_free_list_rebucket_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.schedule_free_list_rebucket("func_800A1B44:\n\tnop\n")

    def test_free_list_rebucket_reorders_each_pattern_once(self) -> None:
        source = (
            "func_800A1B44:\n"
            "\tlh\t$3,D_80235EF0\n"
            "\tli\t$2,-1\t\t\t# 0xffffffff\n"
            "\t.set\tnoreorder\n"
            "\tbeq\t$3,$2,.L2\n"
            "\tsubu\t$sp,$sp,8\n"
            "\t.set\tnoreorder\n"
            "\tmove\t$6,$3\n"
            "\tla\t$7,D_80224E68\n"
            "\tsh\t$5,D_80235EF0\n"
            "\tlui\t$at,%hi(D_80224EF4)\n"
            "\taddu\t$at,$at,$3\n"
            "\tlw\t$2,%lo(D_80224EF4)($at)\n"
        )
        normalized = MODULE.schedule_free_list_rebucket(source)
        self.assertIn(
            "\tlh\t$3,D_80235EF0\n"
            "\tsubu\t$sp,$sp,8\n"
            "\tli\t$2,-1\t\t\t# 0xffffffff\n"
            "\t.set\tnoreorder\n"
            "\tbeq\t$3,$2,.L2\n"
            "\tmove\t$6,$3\n",
            normalized,
        )
        self.assertIn(
            "\tlui\t$at,%hi(D_80224EF4)\n"
            "\taddu\t$at,$at,$3\n"
            "\tlw\t$2,%lo(D_80224EF4)($at)\n"
            "\tsh\t$5,D_80235EF0\n",
            normalized,
        )
        self.assertEqual(normalized.count("\tmove\t$6,$3\n"), 1)


if __name__ == "__main__":
    unittest.main()
