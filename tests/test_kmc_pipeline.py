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
            "\tjal\tfunc_8007DB84\n"
            "\tsw\t$0,240($16)\n"
            "\t.set\tnoreorder\n"
            "\tlhu\t$2,246($16)\n"
            "\tsh\t$0,244($16)\n"
        )
        normalized = MODULE.schedule_pool_type4_removal(source)
        self.assertIn(
            "\tsw\t$0,240($16)\n"
            "\t.set\tnoreorder\n"
            "\tjal\tfunc_8007DB84\n"
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


if __name__ == "__main__":
    unittest.main()
