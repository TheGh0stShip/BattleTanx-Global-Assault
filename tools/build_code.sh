#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$root_dir"

if command -v mips-linux-gnu-as >/dev/null 2>&1; then
    tool_prefix="$(dirname "$(command -v mips-linux-gnu-as)")/mips-linux-gnu-"
else
    tools/bootstrap_mips_binutils.sh
    local_root="$root_dir/.toolchain/mips-binutils/usr"
    tool_prefix="$local_root/bin/mips-linux-gnu-"
    export LD_LIBRARY_PATH="$local_root/lib/x86_64-linux-gnu${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
fi

mkdir -p build/us/asm/us build/us/assets/extracted/us

# These bytes spell ASCII "REMA" and are data, despite decoding as a branch.
# Emitting the word directly avoids a false cross-file branch relocation.
for asm_source in asm/us/*.s; do
    sed -i 's/beql       \$s2, \$a1, \.\?L80099664/.word      0x52454D41/' \
        "$asm_source"
done

"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/header.s.o asm/us/header.s
for asm_source in asm/us/*.s; do
    asm_name="$(basename "$asm_source" .s)"
    asm_object="build/us/asm/us/${asm_name}.s.o"
    "${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
        -o "$asm_object" "$asm_source"
    if [[ "$asm_name" =~ ^main_([0-9A-Fa-f]{8})_to_([0-9A-Fa-f]{8})$ ]]; then
        range_start=$((16#${BASH_REMATCH[1]}))
        range_end=$((16#${BASH_REMATCH[2]}))
        python3 tools/trim_elf32_section.py "$asm_object" .text \
            "$((range_end - range_start))" --alignment 4
    fi
done

tools/bootstrap_ido.sh
tools/bootstrap_kmc_gcc.sh
mkdir -p build/us/src/code
.toolchain/ido5.3/cc -c -O2 -g3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/code/unknown_8007A710.c.o src/code/unknown_8007A710.c
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/code/unknown_8007ADB0.c.o src/code/unknown_8007ADB0.c
.toolchain/ido5.3/cc -c -O2 -g3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/code/unknown_8007B020.c.o src/code/unknown_8007B020.c
.toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
    -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude \
    -o build/us/src/code/gfx_pool.raw.s src/code/gfx_pool.c
python3 tools/normalize_kmc_gcc_asm.py \
    build/us/src/code/gfx_pool.raw.s build/us/src/code/gfx_pool.s
.toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
    -o build/us/src/code/gfx_pool.c.o build/us/src/code/gfx_pool.s
python3 tools/trim_elf32_section.py \
    build/us/src/code/gfx_pool.c.o .text 0xb4 --alignment 4
.toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
    -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude \
    -o build/us/src/code/render_queue.raw.s src/code/render_queue.c
python3 tools/normalize_kmc_gcc_asm.py \
    build/us/src/code/render_queue.raw.s build/us/src/code/render_queue.s
.toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
    -o build/us/src/code/render_queue.c.o build/us/src/code/render_queue.s
python3 tools/trim_elf32_section.py \
    build/us/src/code/render_queue.c.o .text 0x46c --alignment 4
for unit in early_hw early_memory_read early_memory_write early_remote_copy \
            early_commands early_command_status display_slot mapped_record display_buffer \
            display_buffer_select controller_state mode_range angle_subtract \
            angle_between angle_distance angle_fold angle_direction \
            render_submit asset_load video_mode texture_tile small_state \
            small_state_copy collision_noop state_noop state_modes \
            collision_fields object_query pair_queue \
            object_defaults mode_owner mode_transition object_disable \
            object_predicates object_direction object_action \
            session_queries \
            random_integer vector2 vector2_scale vector2_motion game_queue \
            matrix_basic matrix_state \
            matrix_transform vector2_rotate matrix_vector matrix_multiply \
            scheduler_context scheduler_state scheduler_events \
            scheduler_queue scheduler_misc object_range object_timing object_setters \
            object_init object_reset object_flags object_limit \
            object_table_color object_table_reset object_table_lookup \
            gameplay_stub turn_adjust hud_state hud_modes hud_stub \
            hud_secondary hud_primary_modes hud_transition registry_lookup \
            hud_ready hud_clear hud_navigation player_color entry_scan \
            entry_flags hud_callbacks hud_panel_callback hud_list_callback \
            hud_root_callback hud_layout hud_list_trigger \
            hud_entry_values hud_list_reset hud_selection_apply \
            hud_primary_trigger display_registry display_color display_commands \
            menu_state menu_toggle menu_countdown table_lookup; do
    .toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
        -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude \
        -o "build/us/src/code/${unit}.raw.s" "src/code/${unit}.c"
    python3 tools/normalize_kmc_gcc_asm.py \
        "build/us/src/code/${unit}.raw.s" "build/us/src/code/${unit}.s"
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
        -o "build/us/src/code/${unit}.c.o" "build/us/src/code/${unit}.s"
done
while IFS=$'\t' read -r function_name address size status; do
    case "$function_name" in
        \#*|function|'') continue ;;
    esac
    unit="code/codex_batch/${function_name}"
    mkdir -p "build/us/src/code/codex_batch"
    .toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
        -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude \
        -o "build/us/src/${unit}.raw.s" "src/${unit}.c"
    python3 tools/normalize_kmc_gcc_asm.py \
        "build/us/src/${unit}.raw.s" "build/us/src/${unit}.s"
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
        -o "build/us/src/${unit}.c.o" "build/us/src/${unit}.s"
    python3 tools/trim_elf32_section.py \
        "build/us/src/${unit}.c.o" .text "$size" --alignment 4
done < config/us/codex_batch_100.tsv
while IFS=$'\t' read -r function_name address size status; do
    case "$function_name" in
        \#*|function|'') continue ;;
    esac
    source_dir="codex_batch_next"
    if [[ "$function_name" == "func_800ACEFC" ]]; then
        source_dir="codex_batch"
    fi
    unit="code/${source_dir}/${function_name}"
    mkdir -p "build/us/src/code/${source_dir}"
    .toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
        -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude \
        -o "build/us/src/${unit}.raw.s" "src/${unit}.c"
    python3 tools/normalize_kmc_gcc_asm.py \
        "build/us/src/${unit}.raw.s" "build/us/src/${unit}.s"
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
        -o "build/us/src/${unit}.c.o" "build/us/src/${unit}.s"
    python3 tools/trim_elf32_section.py \
        "build/us/src/${unit}.c.o" .text "$size" --alignment 4
done < config/us/codex_batch_next.tsv
python3 tools/trim_elf32_section.py \
    build/us/src/code/early_hw.c.o .text 0x164 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/early_memory_read.c.o .text 0x22c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/early_memory_write.c.o .text 0x1b8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/early_remote_copy.c.o .text 0x74 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/early_commands.c.o .text 0x1d4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/early_command_status.c.o .text 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/display_buffer.c.o .text 0x10c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/display_buffer_select.c.o .text 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_state.c.o .text 0x30 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mapped_record.c.o .text 0x58 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/display_slot.c.o .text 0x3c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_range.c.o .text 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/menu_toggle.c.o .text 0x90 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/menu_countdown.c.o .text 0x48 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/table_lookup.c.o .text 0xd8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/game_queue.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/matrix_state.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/menu_state.c.o .text 0x90 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mode_range.c.o .text 0x24 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/angle_subtract.c.o .text 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/angle_between.c.o .text 0x30 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/angle_distance.c.o .text 0x34 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/angle_fold.c.o .text 0x3c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/angle_direction.c.o .text 0x50 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/vector2_scale.c.o .text 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/render_submit.c.o .text 0x290 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/asset_load.c.o .text 0x10c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/video_mode.c.o .text 0xf4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/texture_tile.c.o .text 0x474 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/small_state.c.o .text 0x90 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/small_state_copy.c.o .text 0x58 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/collision_noop.c.o .text 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/state_noop.c.o .text 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/state_modes.c.o .text 0xa4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/collision_fields.c.o .text 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_query.c.o .text 0x30 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_defaults.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mode_owner.c.o .text 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mode_transition.c.o .text 0x48 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/pair_queue.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_disable.c.o .text 0x48 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_predicates.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_direction.c.o .text 0x78 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_action.c.o .text 0x3c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/session_queries.c.o .text 0x98 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/random_integer.c.o .text 0x4c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/vector2.c.o .text 0x7c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/vector2_motion.c.o .text 0xa4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/matrix_basic.c.o .text 0x6c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/matrix_transform.c.o .text 0x138 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/vector2_rotate.c.o .text 0x9c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/matrix_vector.c.o .text 0x140 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/matrix_multiply.c.o .text 0xf8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/scheduler_context.c.o .text 0x10 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/scheduler_state.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/scheduler_events.c.o .text 0x134 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/scheduler_queue.c.o .text 0x11c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/scheduler_misc.c.o .text 0x60 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_timing.c.o .text 0x34 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_setters.c.o .text 0xc0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_init.c.o .text 0x30 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_reset.c.o .text 0x24 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_flags.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_limit.c.o .text 0x30 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_table_color.c.o .text 0x34 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_table_reset.c.o .text 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_table_lookup.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/gameplay_stub.c.o .text 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turn_adjust.c.o .text 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_state.c.o .text 0x64 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_modes.c.o .text 0x50 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_stub.c.o .text 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_secondary.c.o .text 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_primary_modes.c.o .text 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_transition.c.o .text 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/registry_lookup.c.o .text 0x44 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_ready.c.o .text 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_clear.c.o .text 0x58 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_navigation.c.o .text 0x120 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_color.c.o .text 0x30 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/entry_scan.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_callbacks.c.o .text 0x58 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_panel_callback.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_callback.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/entry_flags.c.o .text 0x84 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_root_callback.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_layout.c.o .text 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_trigger.c.o .text 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_selection_apply.c.o .text 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_primary_trigger.c.o .text 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_entry_values.c.o .text 0x80 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_reset.c.o .text 0x78 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/display_registry.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/display_color.c.o .text 0x64 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/display_commands.c.o .text 0x44 --alignment 4
mkdir -p build/us/src/libultra
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_ai_device_busy.c.o src/libultra/os_ai_device_busy.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_ai_get_length.c.o src/libultra/os_ai_get_length.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_ai_get_status.c.o src/libultra/os_ai_get_status.c
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_copy.c.o src/libultra/al_copy.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_create_mesg_queue.c.o src/libultra/os_create_mesg_queue.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_filter_new.c.o src/libultra/al_filter_new.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_heap_alloc.c.o src/libultra/al_heap_alloc.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_heap_init.c.o src/libultra/al_heap_init.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_pi_get_cmd_queue.c.o src/libultra/os_pi_get_cmd_queue.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_pi_access.c.o src/libultra/os_pi_access.c
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/sched.c.o src/libultra/sched.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_get_thread_pri.c.o src/libultra/os_get_thread_pri.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_dp_device_busy.c.o src/libultra/os_dp_device_busy.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_si_device_busy.c.o src/libultra/os_si_device_busy.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_si_raw_read_io.c.o src/libultra/os_si_raw_read_io.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_si_raw_write_io.c.o src/libultra/os_si_raw_write_io.c
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_main.c.o src/libultra/al_main.c
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/sprintf.c.o src/libultra/sprintf.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_sp_set_pc.c.o src/libultra/os_sp_set_pc.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_sp_device_busy.c.o src/libultra/os_sp_device_busy.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_sp_set_status.c.o src/libultra/os_sp_set_status.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_sp_task_yield.c.o src/libultra/os_sp_task_yield.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_sp_get_status.c.o src/libultra/os_sp_get_status.c
.toolchain/ido5.3/cc -c -O2 -Wo,-loopunroll,0 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/string.c.o src/libultra/string.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_syn_delete.c.o src/libultra/al_syn_delete.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_vi_get_current_context.c.o src/libultra/os_vi_get_current_context.c

"${tool_prefix}objcopy" -I binary -O elf32-tradbigmips -B mips \
    assets/extracted/us/ipl3.bin build/us/assets/extracted/us/ipl3.bin.o

mapfile -t object_files < <(find build/us -type f -name '*.o' -print | sort)
"${tool_prefix}nm" -u "${object_files[@]}" \
    > build/us/undefined_object_symbols.txt
python3 tools/generate_linker_symbols.py build/us/symbols.ld \
    config/us/symbol_addrs.txt \
    build/us/undefined_syms_auto.txt \
    build/us/undefined_funcs_auto.txt \
    --undefined-list build/us/undefined_object_symbols.txt \
    --symbol func_8E180004=0x8E180004 \
    --symbol func_80000000=0x80000000

"${tool_prefix}ld.bfd" -EB -T build/us/symbols.ld -T battletanx_ga.ld \
    -Map build/us/battletanx_ga.map \
    -o build/us/battletanx_ga.elf
"${tool_prefix}objcopy" -O binary \
    build/us/battletanx_ga.elf build/us/battletanx_ga.code.bin

actual_size="$(wc -c < build/us/battletanx_ga.code.bin)"
[[ "$actual_size" -eq 1052672 ]] || {
    echo "Reconstructed code region is $actual_size bytes; expected 1052672" >&2
    exit 1
}

echo "Built exact-reconstruction candidate: build/us/battletanx_ga.code.bin"
