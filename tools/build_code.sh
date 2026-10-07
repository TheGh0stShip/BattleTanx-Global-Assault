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

"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/header.s.o asm/us/header.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_before_8007A710.s.o asm/us/main_before_8007A710.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007A720_to_8007ADB0.s.o asm/us/main_8007A720_to_8007ADB0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007ADC0_to_8007B020.s.o asm/us/main_8007ADC0_to_8007B020.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007B0E4_to_8007B1F0.s.o asm/us/main_8007B0E4_to_8007B1F0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007B0E4_to_8007B1F0.s.o .text 0x10c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007B65C_to_801029D0.s.o asm/us/main_8007B65C_to_801029D0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007B65C_to_801029D0.s.o .text 0x87374 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801029F0_to_80103160.s.o asm/us/main_801029F0_to_80103160.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80103190_to_801037B0.s.o asm/us/main_80103190_to_801037B0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80103860_to_801059B0.s.o asm/us/main_80103860_to_801059B0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801059D0_to_80105A70.s.o asm/us/main_801059D0_to_80105A70.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80105B10_to_8010CF70.s.o asm/us/main_80105B10_to_8010CF70.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8010CFA0_to_8010D660.s.o asm/us/main_8010CFA0_to_8010D660.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8010D740_to_8010F3E0.s.o asm/us/main_8010D740_to_8010F3E0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8010F5F0_to_8010F6A0.s.o asm/us/main_8010F5F0_to_8010F6A0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8010F6D0_to_8010FE90.s.o asm/us/main_8010F6D0_to_8010FE90.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801100E0_to_801103D0.s.o asm/us/main_801100E0_to_801103D0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80110400_to_80110490.s.o asm/us/main_80110400_to_80110490.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801104C0_to_80110540.s.o asm/us/main_801104C0_to_80110540.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80110550_to_801106B0.s.o asm/us/main_80110550_to_801106B0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80110760_to_801118C0.s.o asm/us/main_80110760_to_801118C0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_after_801118C0.s.o asm/us/main_after_801118C0.s

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

"${tool_prefix}nm" -u \
    build/us/asm/us/main_before_8007A710.s.o \
    build/us/src/code/unknown_8007A710.c.o \
    build/us/asm/us/main_8007A720_to_8007ADB0.s.o \
    build/us/src/code/unknown_8007ADB0.c.o \
    build/us/asm/us/main_8007ADC0_to_8007B020.s.o \
    build/us/src/code/unknown_8007B020.c.o \
    build/us/src/code/gfx_pool.c.o \
    build/us/asm/us/main_8007B0E4_to_8007B1F0.s.o \
    build/us/src/code/render_queue.c.o \
    build/us/asm/us/main_8007B65C_to_801029D0.s.o \
    build/us/src/libultra/os_ai_get_length.c.o \
    build/us/src/libultra/os_ai_get_status.c.o \
    build/us/asm/us/main_801029F0_to_80103160.s.o \
    build/us/src/libultra/os_ai_device_busy.c.o \
    build/us/asm/us/main_80103190_to_801037B0.s.o \
    build/us/src/libultra/al_copy.c.o \
    build/us/src/libultra/os_create_mesg_queue.c.o \
    build/us/asm/us/main_80103860_to_801059B0.s.o \
    build/us/src/libultra/al_filter_new.c.o \
    build/us/asm/us/main_801059D0_to_80105A70.s.o \
    build/us/src/libultra/al_heap_alloc.c.o \
    build/us/src/libultra/al_heap_init.c.o \
    build/us/asm/us/main_80105B10_to_8010CF70.s.o \
    build/us/src/libultra/os_pi_get_cmd_queue.c.o \
    build/us/asm/us/main_8010CFA0_to_8010D660.s.o \
    build/us/src/libultra/os_get_thread_pri.c.o \
    build/us/src/libultra/os_pi_access.c.o \
    build/us/asm/us/main_8010D740_to_8010F3E0.s.o \
    build/us/src/libultra/sched.c.o \
    build/us/asm/us/main_8010F5F0_to_8010F6A0.s.o \
    build/us/src/libultra/os_dp_device_busy.c.o \
    build/us/asm/us/main_8010F6D0_to_8010FE90.s.o \
    build/us/src/libultra/os_si_device_busy.c.o \
    build/us/src/libultra/os_si_raw_read_io.c.o \
    build/us/src/libultra/os_si_raw_write_io.c.o \
    build/us/src/libultra/al_main.c.o \
    build/us/src/libultra/sprintf.c.o \
    build/us/src/libultra/os_sp_set_pc.c.o \
    build/us/asm/us/main_801100E0_to_801103D0.s.o \
    build/us/src/libultra/os_sp_device_busy.c.o \
    build/us/asm/us/main_80110400_to_80110490.s.o \
    build/us/src/libultra/os_sp_set_status.c.o \
    build/us/src/libultra/os_sp_task_yield.c.o \
    build/us/asm/us/main_801104C0_to_80110540.s.o \
    build/us/src/libultra/os_sp_get_status.c.o \
    build/us/asm/us/main_80110550_to_801106B0.s.o \
    build/us/src/libultra/string.c.o \
    build/us/src/libultra/al_syn_delete.c.o \
    build/us/asm/us/main_80110760_to_801118C0.s.o \
    build/us/src/libultra/os_vi_get_current_context.c.o \
    build/us/asm/us/main_after_801118C0.s.o \
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
