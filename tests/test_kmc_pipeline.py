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
    def test_angle_step_update_requires_both_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.shape_angle_step_update("func_8009D75C:\n\tnop\n")

    def test_angle_step_update_uses_retail_shared_store(self) -> None:
        source = (
            "func_8009D75C:\n\tor\t$2,$4,$22\n"
            "\tmove\t$4,$2\n\tandi\t$3,$18,0xffff\n"
            "\tandi\t$2,$4,0xffff\n\tsltu\t$2,$2,$3\n"
            "\t.set\tnoreorder\n\tbeq\t$2,$0,.L2\n\tsrl\t$2,$20,15\n"
            "\t.set\tnoreorder\n\t.set\tnoreorder\n\tj\t.L3\n"
            "\tsh\t$19,0($17)\n\t.set\tnoreorder\n.L2:\n"
            "\tandi\t$2,$2,0x0001\n\t.set\tnoreorder\n"
            "\tbeq\t$2,$0,.L4\n\tor\t$2,$16,$22\n"
            "\t.set\tnoreorder\n\tsubu\t$2,$2,$3\n"
            "\t.set\tnoreorder\n\tj\t.L3\n\tsh\t$2,0($17)\n"
            "\t.set\tnoreorder\n.L4:\n\taddu\t$18,$21,$18\n"
            "\tsh\t$18,0($17)\n.L3:\n\tandi\t$2,$4,0xffff\n"
        )
        normalized = MODULE.shape_angle_step_update(source)
        self.assertIn("\taddu\t$2,$4,$22\n", normalized)
        self.assertIn(
            "\tmove\t$3,$18\n\tmove\t$4,$2\n"
            "\tandi\t$2,$4,0xffff\n\tandi\t$3,$3,0xffff\n",
            normalized,
        )
        self.assertIn(".L5:\n\tsh\t$2,0($17)\n", normalized)

    def test_search_record_address_uses_retained_index_copy(self) -> None:
        source = "func_8008875C:\n.L5:\n\tsll\t$2,$2,3\n"
        normalized = MODULE.normalize_v3(source)
        self.assertIn(".L5:\n\tsll\t$2,$3,3\n", normalized)

    def test_search_record_address_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize_v3("func_8008875C:\n\tnop\n")

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
            "Steps_FreeBranch:\n"
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
            MODULE.normalize_v3("Steps_FreeBranch:\n\tnop\n")

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

    def test_turret_sweep_hazards_require_both_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.reproduce_turret_sweep_assembler_hazards(
                "func_800E1BB0:\n\tnop\n"
            )

    def test_entity_model_draw_label_hazard_is_narrowly_rewritten(self) -> None:
        source = (
            "func_800E3FDC:\n"
            "\tj\t.L20\n"
            "\taddu\t$2,$4,$2\n"
            "\t.set\tnoreorder\n"
            ".L21:\n"
            "\tmult\t$6,$5\n"
        )
        normalized = MODULE.reproduce_entity_model_draw_label_hazard(source)
        self.assertIn(
            "\tj\t.L20\n.L21 = . + 4\n\taddu\t$2,$4,$2\n",
            normalized,
        )
        self.assertNotIn(".L21:\n\tmult\t$6,$5\n", normalized)

    def test_entity_model_draw_label_hazard_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.reproduce_entity_model_draw_label_hazard(
                "func_800E3FDC:\n\tnop\n"
            )

    def test_crate_list_head_store_is_scheduled_first(self) -> None:
        source = (
            "func_800E66A8:\n"
            "\tlbu\t$3,29($17)\n"
            "\tli\t$2,0x0000007f\t\t# 127\n"
            "\tsw\t$16,12($17)\n"
        )
        normalized = MODULE.schedule_crate_list_head_store(source)
        self.assertIn(
            "\tsw\t$16,12($17)\n"
            "\tlbu\t$3,29($17)\n"
            "\tli\t$2,0x0000007f\t\t# 127\n",
            normalized,
        )

    def test_crate_list_head_store_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.schedule_crate_list_head_store("func_800E66A8:\n\tnop\n")

    def test_script_pair_registers_are_narrowly_swapped(self) -> None:
        source = (
            "func_800D12B0:\n"
            "\tlbu\t$2,0($6)\n"
            "\taddu\t$6,$6,1\n"
            "\tlbu\t$4,0($6)\n"
            "\taddu\t$6,$6,1\n"
            "\tlbu\t$3,D_803A6A04\n"
            "\tsll\t$2,$2,8\n"
            "\tor\t$4,$4,$2\n"
            "\tsh\t$4,D_803A666E\n"
            "\tlbu\t$2,0($6)\n"
            "\taddu\t$6,$6,1\n"
            "\tlbu\t$4,0($6)\n"
            "\tsll\t$2,$2,8\n"
            "\tor\t$4,$4,$2\n"
            "\tli\t$2,0x00000017\t\t# 23\n"
            "\tsh\t$4,D_803A6670\n"
            "\t.set\tnoreorder\n"
            "\tbne\t$3,$2,.L2\n"
            "\taddu\t$6,$6,1\n"
        )
        normalized = MODULE.swap_script_pair_registers(source)
        self.assertIn("\tlbu\t$3,0($6)\n", normalized)
        self.assertIn("\tlbu\t$4,D_803A6A04\n", normalized)
        self.assertIn("\tsh\t$3,D_803A666E\n", normalized)
        self.assertIn("\tbne\t$4,$2,.L2\n", normalized)
        self.assertNotIn("\tsh\t$4,D_803A666E\n", normalized)

    def test_script_pair_register_swap_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.swap_script_pair_registers("func_800D12B0:\n\tnop\n")

    def test_projectile_segment_label_hazards_are_suppressed(self) -> None:
        source = (
            "func_800DB1B0:\n"
            "\tl.s\t$f0,$LF_lis4\n"
            ".L21:\n"
            "\tmul.s\t$f2,$f2,$f0\n"
            "\tdiv.s\t$f2,$f2,$f0\n"
            "\tadd.s\t$f4,$f4,$f2\n"
            ".L17:\n"
            "\tmul.s\t$f2,$f20,$f10\n"
        )
        normalized = MODULE.reproduce_projectile_segment_label_hazards(source)
        self.assertIn(".L21 = . + 4\n\tlwc1\t$f0,%lo($LF_lis4)($1)\n", normalized)
        self.assertIn(".L17 = . + 4\n\tadd.s\t$f4,$f4,$f2\n", normalized)
        self.assertNotIn(".L21:\n", normalized)
        self.assertNotIn(".L17:\n", normalized)

    def test_projectile_segment_label_hazards_require_both_fires(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "labeled FP load fired 0 times"):
            MODULE.reproduce_projectile_segment_label_hazards(
                "func_800DB1B0:\n\tnop\n"
            )
        with self.assertRaisesRegex(RuntimeError, "labeled FP add fired 0 times"):
            MODULE.reproduce_projectile_segment_label_hazards(
                "func_800DB1B0:\n\tl.s\t$f0,$LF_lis4\n.L21:\n"
            )

    def test_flag_dispatch_output_is_allocated_to_a3(self) -> None:
        source = (
            "func_800E4DA0:\n"
            "\tsubu\t$sp,$sp,32\n\tlw\t$8,48($sp)\n"
            "\tsw\t$16,16($sp)\n\tmove\t$16,$4\n"
            "\tsw\t$31,28($sp)\n\tsw\t$18,24($sp)\n"
            "\tsw\t$17,20($sp)\n\tlbu\t$2,10($16)\n"
            "\tmove\t$18,$7\n\tandi\t$2,$2,0x0002\n"
            "\t.set\tnoreorder\n\tbne\t$2,$0,.L1\n\tmove\t$17,$5\n"
            "\tsw\t$2,0($8)\n\tsw\t$2,0($8)\n"
            "\tsb\t$2,0($8)\n\ts.s\t$f0,4($8)\n\ts.s\t$f0,8($8)\n"
        )
        normalized = MODULE.allocate_flag_dispatch_output_to_a3(source)
        self.assertIn("\tmove\t$18,$7\n\tlw\t$7,48($sp)\n", normalized)
        self.assertEqual(normalized.count("\tsw\t$2,0($7)\n"), 2)
        self.assertIn("\tsb\t$2,0($7)\n", normalized)
        self.assertIn("\ts.s\t$f0,4($7)\n", normalized)
        self.assertIn("\ts.s\t$f0,8($7)\n", normalized)

    def test_flag_dispatch_output_requires_exact_fire_counts(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "output prologue fired 0 times"):
            MODULE.allocate_flag_dispatch_output_to_a3("func_800E4DA0:\n\tnop\n")

    def test_value_decay_clamp_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.shape_value_decay_clamp("func_800B6934:\n\tnop\n")

    def test_value_decay_clamp_rewrites_branch_and_stores(self) -> None:
        source = (
            "func_800B6934:\n"
            ".L10:\n"
            "\t.set\tnoreorder\n"
            "\tnop\n"
            "\tbc1tl\t.L8\n"
            "\tmov.s\t$f0,$f6\n"
            "\t.set\tnoreorder\n"
            ".L8:\n"
            "\ts.s\t$f0,28($4)\n"
            ".L4:\n"
        )
        normalized = MODULE.shape_value_decay_clamp(source)
        self.assertIn(
            "\tbc1f\t.L4\n"
            "\ts.s\t$f0,28($4)\n"
            "\t.set\tnoreorder\n"
            "\ts.s\t$f6,28($4)\n",
            normalized,
        )

    def test_mask_value_magnitude_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.preserve_mask_value_magnitude_copy("func_80098454:\n\tnop\n")

    def test_mask_value_magnitude_uses_retained_copy(self) -> None:
        source = (
            "func_80098454:\n"
            "\tbgez\t$4,.L10\n"
            "\tmove\t$2,$4\n"
            "\t.set\tnoreorder\n"
            "\tsubu\t$2,$0,$4\n"
            ".L10:\n"
        )
        normalized = MODULE.preserve_mask_value_magnitude_copy(source)
        self.assertIn("\tsubu\t$2,$0,$2\n", normalized)

    def test_pool_type4_removal_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.schedule_pool_type4_removal("func_8007E7A8:\n\tnop\n")

    def test_pool_type4_removal_schedules_state_before_call(self) -> None:
        source = (
            "func_8007E7A8:\n"
            ".L7:\n"
            "\t.set\tnoreorder\n"
            "\tjal\tSteps_SpliceIn\n"
            "\tsw\t$0,240($16)\n"
            "\t.set\tnoreorder\n"
            "\tlhu\t$2,246($16)\n"
            "\tsh\t$0,244($16)\n"
        )
        normalized = MODULE.schedule_pool_type4_removal(source)
        self.assertIn(
            "\tsw\t$0,240($16)\n"
            "\t.set\tnoreorder\n"
            "\tjal\tSteps_SpliceIn\n"
            "\tsh\t$0,244($16)\n"
            "\t.set\tnoreorder\n"
            "\tlhu\t$2,246($16)\n",
            normalized,
        )

    def test_object_distance_probe_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "prologue rewrite fired 0 times"):
            MODULE.schedule_object_distance_probe("func_80080C04:\n\tnop\n")

    def test_object_distance_probe_rewrites_each_pattern_once(self) -> None:
        source = (
            "func_80080C04:\n"
            "\tsw\t$16,16($sp)\n"
            "\taddu\t$16,$4,296\n"
            "\tsw\t$31,20($sp)\n"
            " #APP\n"
            " #NO_APP\n"
            "\tlw\t$3,268($4)\n"
            "\tli\t$2,0x00000002\t\t# 2\n"
            "\tbne\t$3,$2,.L2\n"
            "\tnop\n"
            "\tsub.s\t$f4,$f2,$f0\n"
            "\tc.lt.s\t$f6,$f4\n"
            "\tsub.s\t$f0,$f2,$f0\n"
            " #APP\n"
            " #NO_APP\n"
            "\tlbu\t$3,364($4)\n"
            "\t#nop\n"
            "\tandi\t$2,$3,0x0001\n"
            "\t.set\tnoreorder\n"
            "\tbeq\t$2,$0,.L5\n"
            "\tandi\t$2,$3,0x00fb\n"
        )
        normalized = MODULE.schedule_object_distance_probe(source)
        self.assertIn("\tbne\t$3,$2,.L2\n\taddu\t$16,$4,296\n", normalized)
        self.assertIn("\tmtc1\t$0,$f6\n\tc.lt.s\t$f6,$f4\n", normalized)
        self.assertIn("\tbeq\t$2,$0,.L5\n\tsub.s\t$f0,$f2,$f0\n", normalized)

    def test_matrix_basis_inverse_requires_all_patterns(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "float register rewrite fired 0 times"):
            MODULE.schedule_matrix_basis_inverse("func_8009F5AC:\n\tnop\n")

    def test_matrix_basis_inverse_rewrites_each_pattern_once(self) -> None:
        source = (
            "func_8009F5AC:\n"
            "\tl.s\t$f0,4($6)\n"
            "\tl.s\t$f4,40($4)\n"
            "\tsubu\t$sp,$sp,16\n"
            "\tmove\t$11,$5\n"
            "\tadd.s\t$f0,$f0,$f4\n"
            "\tl.s\t$f2,D_80072650\n"
            "\tmove\t$10,$11\n"
            "\tmove\t$8,$0\n"
            "\ts.s\t$f0,4($7)\n"
            ".L5:\n"
            "\tmove\t$9,$11\n"
            "\tmove\t$7,$10\n"
            "\tmove\t$3,$0\n"
            "\taddu\t$2,$3,$11\n"
        )
        normalized = MODULE.schedule_matrix_basis_inverse(source)
        self.assertIn("\tl.s\t$f2,40($4)\n", normalized)
        self.assertIn("\tmove\t$8,$0\n\tadd.s\t$f0,$f0,$f2\n", normalized)
        self.assertIn(".L5:\n\tmove\t$3,$0\n\tmove\t$9,$11\n", normalized)
        self.assertIn("\taddu\t$2,$11,$3\n", normalized)

    def test_record_index_copy_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.preserve_record_index_copy("func_800E82AC:\n\tnop\n")

    def test_record_index_copy_uses_a2(self) -> None:
        source = "func_800E82AC:\n.L2:\n\tsll\t$2,$3,3\n"
        normalized = MODULE.preserve_record_index_copy(source)
        self.assertIn(".L2:\n\tsll\t$2,$6,3\n", normalized)

    def test_collision_query_label_hazard_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.reproduce_collision_query_label_hazard(
                "func_800B4684:\n\tnop\n"
            )

    def test_collision_query_label_hazard_preserves_target(self) -> None:
        source = (
            "func_800B4684:\n"
            "\tj\t.L40\n"
            "\tadd.s\t$f2,$f4,$f0\n"
            "\t.set\tnoreorder\n"
            ".L39:\n"
            "\tmul.s\t$f2,$f2,$f2\n"
        )
        normalized = MODULE.reproduce_collision_query_label_hazard(source)
        self.assertIn(
            "\tj\t.L40\n.L39 = . + 4\n"
            "\tadd.s\t$f2,$f4,$f0\n\t.set\tnoreorder\n"
            "\tmul.s\t$f2,$f2,$f2\n",
            normalized,
        )

    def test_sprite_ring_label_hazard_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.reproduce_sprite_ring_label_hazard(
                "func_800A42C8:\n\tnop\n\t.end\tfunc_800A42C8\n"
            )

    def test_sprite_ring_label_hazard_preserves_target(self) -> None:
        source = (
            "func_800A42C8:\n"
            "\tsub.s\t$f2,$f2,$f0\n"
            ".L47:\n"
            "\tmul.s\t$f2,$f8,$f2\n"
            "\t.end\tfunc_800A42C8\n"
        )
        normalized = MODULE.reproduce_sprite_ring_label_hazard(source)
        self.assertIn(
            ".L47 = . + 4\n"
            "\tsub.s\t$f2,$f2,$f0\n"
            "\tmul.s\t$f2,$f8,$f2\n",
            normalized,
        )

    def test_distance_multiply_hazard_requires_declared_fire_count(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.suppress_distance_multiply_hazard_nops(
                "func_80083230:\n\tnop\n\t.end\tfunc_80083230\n"
            )

    def test_distance_multiply_hazard_encodes_compiler_operands(self) -> None:
        source = (
            "func_80083230:\n"
            "\tbne\t$2,$0,.L7\n"
            "\tadd.s\t$f0,$f0,$f2\n"
            ".L7:\n"
            "\tmul.s\t$f0,$f0,$f24\n"
            "\t.end\tfunc_80083230\n"
        )
        normalized = MODULE.suppress_distance_multiply_hazard_nops(source)
        self.assertIn(
            ".L7:\n\t.word\t0x46180002\t"
            "# mul.s $f0,$f0,$f24; retail as omits hazard nop\n",
            normalized,
        )

    def test_progress_level_loop_setup_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.schedule_progress_level_loop_setup(
                "func_8009C31C:\n\tnop\n"
            )

    def test_progress_level_loop_setup_fills_branch_slot(self) -> None:
        source = (
            "func_8009C31C:\n"
            "\tbeq\t$2,$0,.L12\n"
            "\tli\t$17,0x00000001\t\t# 1\n"
            "\t.set\tnoreorder\n"
            "\tmove\t$16,$4\n"
        )
        normalized = MODULE.schedule_progress_level_loop_setup(source)
        self.assertIn(
            "\tbeq\t$2,$0,.L12\n"
            "\tmove\t$16,$4\n"
            "\t.set\tnoreorder\n"
            "\tli\t$17,0x00000001\t\t# 1\n",
            normalized,
        )

    def test_angle_table_lookup_requires_index_pattern(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "index allocation fired 0 times"):
            MODULE.shape_angle_table_lookup_registers(
                "func_8009D5B4:\n\tnop\n"
            )

    def test_angle_table_lookup_reassigns_temporary_and_adds(self) -> None:
        source = (
            "func_8009D5B4:\n"
            "\txor\t$2,$5,$4\n"
            "\t.set\tnoreorder\n"
            "\tandi\t$3,$2,0xffff\n"
            "\tsll\t$2,$3,2\n"
            "\taddu\t$2,$4,$3\n"
            "\taddu\t$2,$4,$3\n"
        )
        normalized = MODULE.shape_angle_table_lookup_registers(source)
        self.assertIn(
            "\txor\t$3,$5,$4\n"
            "\t.set\tnoreorder\n"
            "\tandi\t$2,$3,0xffff\n"
            "\tsll\t$2,$2,2\n",
            normalized,
        )
        self.assertEqual(normalized.count("\taddu\t$2,$3,$4\n"), 2)

    def test_spotter_frame_setup_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.order_spotter_frame_setup_prologue(
                "func_800A6FD0:\n\tnop\n"
            )

    def test_spotter_frame_setup_reorders_prologue(self) -> None:
        source = (
            "func_800A6FD0:\n"
            "\tsw\t$17,28($sp)\n"
            "\tmove\t$17,$7\n"
            "\tsw\t$31,32($sp)\n"
        )
        normalized = MODULE.order_spotter_frame_setup_prologue(source)
        self.assertIn(
            "\tsw\t$31,32($sp)\n"
            "\tsw\t$17,28($sp)\n"
            "\tmove\t$17,$7\n",
            normalized,
        )

    def test_font_glyph_draw_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "float prologue fired 0 times"):
            MODULE.order_font_glyph_draw_prologue(
                "func_80096A54:\n\tnop\n"
            )

    def test_font_glyph_draw_reorders_prologue_and_address(self) -> None:
        source = (
            "func_80096A54:\n"
            "\ts.d\t$f22,80($sp)\n"
            "\tl.s\t$f22,112($sp)\n"
            "\tsw\t$18,56($sp)\n"
            "\tmove\t$18,$4\n"
            "\tsw\t$19,60($sp)\n"
            "\tmove\t$19,$7\n"
            "\ts.d\t$f20,72($sp)\n"
            "\tl.s\t$f20,108($sp)\n"
            "\taddu\t$2,$2,$17\n"
        )
        normalized = MODULE.order_font_glyph_draw_prologue(source)
        self.assertIn(
            "\ts.d\t$f20,72($sp)\n\tl.s\t$f20,108($sp)\n",
            normalized,
        )
        self.assertIn(
            "\ts.d\t$f22,80($sp)\n\tl.s\t$f22,112($sp)\n",
            normalized,
        )
        self.assertEqual(normalized.count("\taddu\t$2,$17,$2\n"), 1)

    def test_music_stream_queue_requires_dispatch_pattern(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "dispatch prefix fired 0 times"):
            MODULE.shape_music_stream_queue_registers(
                "func_80097D14:\n\tnop\n"
            )

    def test_music_stream_queue_shapes_both_allocations(self) -> None:
        source = (
            "func_80097D14:\n"
            "\tsw\t$17,20($sp)\n\tla\t$17,D_801B4540\n"
            "\tsw\t$31,24($sp)\n\tsw\t$16,16($sp)\n"
            "\tlhu\t$3,0($17)\n\t#nop\n\t.set\tnoreorder\n"
            "\tbeq\t$3,$0,.L3\n\tmove\t$16,$5\n\t.set\tnoreorder\n"
            "\tli\t$2,0x00000001\t\t# 1\n\t.set\tnoreorder\n"
            "\tbeq\t$3,$2,.L4\n\tsll\t$2,$4,3\n"
            "\tlhu\t$3,D_801B4540+2\n\tlui\t$at,%hi(D_80114710)\n"
            "\taddu\t$at,$at,$2\n\tlw\t$4,%lo(D_80114710)($at)\n"
            "\tlui\t$at,%hi(D_80114710+4)\n\taddu\t$at,$at,$2\n"
            "\tlw\t$6,%lo(D_80114710+4)($at)\n\taddu\t$3,$3,1\n"
            "\tandi\t$3,$3,0x0001\n\tsll\t$3,$3,2\n"
            "\taddu\t$3,$3,$17\n\tlw\t$5,8($3)\n"
        )
        normalized = MODULE.shape_music_stream_queue_registers(source)
        self.assertIn("\tlhu\t$5,0($17)\n", normalized)
        self.assertIn("\tbeq\t$5,$2,.L4\n\tsll\t$3,$4,3\n", normalized)
        self.assertIn("\tlhu\t$2,D_801B4540+2\n", normalized)
        self.assertIn("\tlw\t$5,8($2)\n", normalized)

    def test_spotter_update_requires_collision_pattern(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "collision setup fired 0 times"):
            MODULE.schedule_spotter_update_collision_setup(
                "func_800A6C20:\n\tnop\n"
            )

    def test_spotter_update_reorders_setup_and_adds(self) -> None:
        source = (
            "func_800A6C20:\n"
            "\tl.s\t$f2,20($16)\n\taddu\t$4,$16,16\n"
            "\tadd.s\t$f2,$f2,$f0\n\taddu\t$5,$sp,32\n"
            "\tli\t$6,0x00a00000\t\t# 10485760\n"
            "\tsh\t$0,D_80397650\n\ts.s\t$f2,36($sp)\n"
            "\tlbu\t$7,25($16)\n\tori\t$6,$6,0x0403\n"
            "\taddu\t$2,$sp,40\n"
            "\tadd.s\t$f0,$f0,$f2\n"
            "\tadd.s\t$f0,$f0,$f2\n"
            "\tadd.s\t$f0,$f0,$f2\n"
        )
        normalized = MODULE.schedule_spotter_update_collision_setup(source)
        self.assertIn(
            "\taddu\t$2,$sp,40\n\tsh\t$0,D_80397650\n",
            normalized,
        )
        self.assertEqual(normalized.count("\tadd.s\t$f0,$f2,$f0\n"), 3)

    def test_grid_node_remove_requires_table_bases(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "table bases fired 0 times"):
            MODULE.shape_grid_node_remove_loop("func_800B0F4C:\n\tnop\n")

    def test_grid_node_remove_separates_loop_index(self) -> None:
        source = (
            "func_800B0F4C:\n"
            "\tla\t$22,D_801166E8\n\tla\t$21,D_801166E0\n"
            "\tlhu\t$4,10($17)\n\tandi\t$3,$16,0x00ff\n"
            "\tmult\t$3,$4\n\tmflo\t$4\n\t#nop\n"
            "\tsll\t$3,$3,1\n\taddu\t$2,$3,$22\n"
            "\tlhu\t$2,0($2)\n\tlhu\t$5,8($17)\n"
            "\taddu\t$2,$19,$2\n\tsra\t$2,$2,10\n"
            "\taddu\t$2,$2,$4\n\tmult\t$2,$5\n\tmflo\t$2\n"
            "\t#nop\n\taddu\t$3,$3,$21\n\tlhu\t$3,0($3)\n"
            "\tandi\t$6,$16,0x00ff\n"
        )
        normalized = MODULE.shape_grid_node_remove_loop(source)
        self.assertIn("\tla\t$22,D_801166E0\n\tla\t$21,D_801166E8\n", normalized)
        self.assertIn("\tandi\t$6,$16,0x00ff\n\tsll\t$3,$6,1\n", normalized)

    def test_camera_collision_requires_full_function(self) -> None:
        source = "func_800A6320:\n\tnop\n\t.end\tfunc_800A6320\n"
        with self.assertRaisesRegex(RuntimeError, "saved-register counts"):
            MODULE.shape_camera_collision_registers(source)

    def test_mover_reflect_likely_multiply_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.hoist_mover_reflect_likely_multiply(
                "func_800B5B8C:\n\tnop\n"
            )

    def test_path_waypoint_side_registers_are_gated(self) -> None:
        source = (
            "func_8007F59C:\n"
            "\tlhu\t$20,250($18)\n\t#nop\n"
            "\tandi\t$16,$20,0xffff\n"
            "\tjal\tSteps_InitStep_Free\n\tmove\t$5,$16\n"
        )
        normalized = MODULE.shape_path_waypoint_side_registers(source)
        self.assertIn("\tlhu\t$5,250($18)\n", normalized)
        self.assertIn("\tjal\tSteps_InitStep_Free\n\tmove\t$20,$5\n", normalized)

    def test_waypoint_stack_reset_addresses_require_two_fires(self) -> None:
        source = (
            "func_8008A764:\n"
            "\taddu\t$2,$4,$2\n"
            "\taddu\t$2,$4,$2\n"
        )
        normalized = MODULE.shape_waypoint_stack_reset_addresses(source)
        self.assertEqual(normalized.count("\taddu\t$2,$2,$4\n"), 2)
        with self.assertRaisesRegex(RuntimeError, "fired 1 times"):
            MODULE.shape_waypoint_stack_reset_addresses(
                "func_8008A764:\n\taddu\t$2,$4,$2\n"
            )

    def test_new_allocator_rules_reject_pattern_drift(self) -> None:
        cases = (
            (MODULE.swap_model_vertex_offset_loop_registers,
             "func_800EBA98:\n\tnop\n\t.end\tfunc_800EBA98\n", "counts"),
            (MODULE.shape_entity_selection_registers,
             "func_800ED990:\n\tnop\n", "fired 0"),
            (MODULE.shape_turret_angle_delta_registers,
             "func_800E3460:\n\tnop\n", "fired 0"),
            (MODULE.shape_wave_vertex_update_registers,
             "func_800EF770:\n\tnop\n\t.end\tfunc_800EF770\n", "counts"),
            (MODULE.shape_wave_mesh_copy_registers,
             "func_800F1900:\n\tnop\n\t.end\tfunc_800F1900\n", "counts"),
            (MODULE.shape_unit_command_candidate_prologue,
             "func_80086CEC:\n\tnop\n", "prologue"),
            (MODULE.swap_unit_command_search_registers,
             "func_80086FF4:\n\tnop\n\t.end\tfunc_80086FF4\n", "counts"),
            (MODULE.shape_tank_aim_refresh_registers,
             "func_80082198:\n\tnop\n", "prologue"),
            (MODULE.reproduce_vector_angle_join_label,
             "func_800B8310:\n\tnop\n\t.end\tfunc_800B8310\n", "join-label"),
            (MODULE.shape_effect_mesh_draw_registers,
             "func_800F8AAC:\n\tnop\n\t.end\tfunc_800F8AAC\n", "counts"),
            (MODULE.shape_contact_side_test_frame,
             "func_800B739C:\n\tnop\n\t.end\tfunc_800B739C\n", "prologue"),
            (MODULE.shape_curve_mode_dispatches,
             "func_80079AFC:\n\tnop\n\t.end\tfunc_80079AFC\n", "curve-dispatch"),
        )
        for function, source, message in cases:
            with self.subTest(function=function.__name__):
                with self.assertRaisesRegex(RuntimeError, message):
                    function(source)

    def test_mover_reflect_likely_multiply_is_hoisted(self) -> None:
        source = (
            "func_800B5B8C:\n"
            "\t.set\tnoreorder\n"
            "\tbc1fl\t.L25\n"
            "\tmul.s\t$f4,$f4,$f6\n"
        )
        normalized = MODULE.hoist_mover_reflect_likely_multiply(source)
        self.assertIn(
            "\tmul.s\t$f4,$f4,$f6\n\t.set\tnoreorder\n"
            "\tbc1fl\t.L25\n\tnop\n",
            normalized,
        )

    def test_turret_sweep_hazards_reproduce_retail_forms(self) -> None:
        source = (
            "func_800E1BB0:\n"
            "\tmul.s\t$f20,$f0,$f2\n"
            "\t.set\tnoreorder\n"
            "\tjal\tfunc_8009D4B0\n"
            "\tnop\n"
            "\t.set\tnoreorder\n"
            "\tmul.s\t$f0,$f20,$f0\n"
            "\tl.s\t$f0,$LF_lis4\n"
            ".L37:\n"
        )
        normalized = MODULE.reproduce_turret_sweep_assembler_hazards(source)
        self.assertIn(
            "\tjal\tfunc_8009D4B0\n\tnop\n\tnop\n\t.set\tnoreorder\n",
            normalized,
        )
        self.assertIn(
            "\tlui\t$1,%hi($LF_lis4)\n"
            ".L37 = . + 4\n"
            "\tlwc1\t$f0,%lo($LF_lis4)($1)\n",
            normalized,
        )

    def test_crate_burst_hazard_requires_one_pattern(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.reproduce_crate_burst_hilo_hazard("func_800E7768:\n\tnop\n")

    def test_crate_burst_hazard_inserts_two_nops(self) -> None:
        normalized = MODULE.reproduce_crate_burst_hilo_hazard(
            "func_800E7768:\n\tdiv\t$16,$21,$18\n\tmult\t$16,$20\n"
        )
        self.assertIn(
            "\tdiv\t$16,$21,$18\n\tnop\n\tnop\n\tmult\t$16,$20\n",
            normalized,
        )

    def test_color_interpolate_hazard_requires_one_pattern(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.reproduce_color_interpolate_load_hazard(
                "func_800F3B80:\n\tnop\n"
            )

    def test_color_interpolate_hazard_materializes_one_nop(self) -> None:
        source = (
            "func_800F3B80:\n"
            "\tmflo\t$2\n"
            "\tlw\t$8,16($sp)\n"
            "\t#nop\n"
            "\tdiv\t$2,$2,$8\n"
        )
        normalized = MODULE.reproduce_color_interpolate_load_hazard(source)
        self.assertIn(
            "\tlw\t$8,16($sp)\n\tnop\n\tdiv\t$2,$2,$8\n",
            normalized,
        )

    def test_lzari_hazards_require_one_pattern_per_function(self) -> None:
        source = (
            "func_800A0BA8:\n\tnop\n\t.end\tfunc_800A0BA8\n"
            "func_800A0E00:\n\tnop\n\t.end\tfunc_800A0E00\n"
        )
        with self.assertRaisesRegex(RuntimeError, "func_800A0BA8.*fired 0"):
            MODULE.reproduce_lzari_load_hazards(source)

    def test_lzari_hazards_materialize_one_nop_per_function(self) -> None:
        hazard = "\tlw\t$2,0($17)\n\t#nop\n\tdivu\t$4,$4,$2\n"
        source = (
            f"func_800A0BA8:\n{hazard}\t.end\tfunc_800A0BA8\n"
            f"func_800A0E00:\n{hazard}\t.end\tfunc_800A0E00\n"
        )
        normalized = MODULE.reproduce_lzari_load_hazards(source)
        self.assertEqual(normalized.count("\tlw\t$2,0($17)\n\tnop\n\tdivu"), 2)


if __name__ == "__main__":
    unittest.main()
