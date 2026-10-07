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
    -o build/us/asm/us/main_before_8007ADB0.s.o asm/us/main_before_8007ADB0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007ADC0_to_8007B020.s.o asm/us/main_8007ADC0_to_8007B020.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007B030_to_801029D0.s.o asm/us/main_8007B030_to_801029D0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801029F0_to_801037B0.s.o asm/us/main_801029F0_to_801037B0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80103860_to_801059B0.s.o asm/us/main_80103860_to_801059B0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801059D0_to_80105AD0.s.o asm/us/main_801059D0_to_80105AD0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80105B10_to_8010D660.s.o asm/us/main_80105B10_to_8010D660.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8010D680_to_8010F6A0.s.o asm/us/main_8010D680_to_8010F6A0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8010F6D0_to_8010FE90.s.o asm/us/main_8010F6D0_to_8010FE90.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8010FEC0_to_80110490.s.o asm/us/main_8010FEC0_to_80110490.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801104C0_to_80110540.s.o asm/us/main_801104C0_to_80110540.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80110550_to_80110750.s.o asm/us/main_80110550_to_80110750.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80110760_to_801118C0.s.o asm/us/main_80110760_to_801118C0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_after_801118C0.s.o asm/us/main_after_801118C0.s

tools/bootstrap_ido.sh
mkdir -p build/us/src/code
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/code/unknown_8007ADB0.c.o src/code/unknown_8007ADB0.c
.toolchain/ido5.3/cc -c -O2 -g3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/code/unknown_8007B020.c.o src/code/unknown_8007B020.c
mkdir -p build/us/src/libultra
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
    -o build/us/src/libultra/al_heap_init.c.o src/libultra/al_heap_init.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_get_thread_pri.c.o src/libultra/os_get_thread_pri.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_dp_device_busy.c.o src/libultra/os_dp_device_busy.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_si_device_busy.c.o src/libultra/os_si_device_busy.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_sp_set_status.c.o src/libultra/os_sp_set_status.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_sp_task_yield.c.o src/libultra/os_sp_task_yield.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_sp_get_status.c.o src/libultra/os_sp_get_status.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_syn_delete.c.o src/libultra/al_syn_delete.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_vi_get_current_context.c.o src/libultra/os_vi_get_current_context.c

"${tool_prefix}objcopy" -I binary -O elf32-tradbigmips -B mips \
    assets/extracted/us/ipl3.bin build/us/assets/extracted/us/ipl3.bin.o

"${tool_prefix}nm" -u \
    build/us/asm/us/main_before_8007ADB0.s.o \
    build/us/src/code/unknown_8007ADB0.c.o \
    build/us/asm/us/main_8007ADC0_to_8007B020.s.o \
    build/us/src/code/unknown_8007B020.c.o \
    build/us/asm/us/main_8007B030_to_801029D0.s.o \
    build/us/src/libultra/os_ai_get_length.c.o \
    build/us/src/libultra/os_ai_get_status.c.o \
    build/us/asm/us/main_801029F0_to_801037B0.s.o \
    build/us/src/libultra/al_copy.c.o \
    build/us/src/libultra/os_create_mesg_queue.c.o \
    build/us/asm/us/main_80103860_to_801059B0.s.o \
    build/us/src/libultra/al_filter_new.c.o \
    build/us/asm/us/main_801059D0_to_80105AD0.s.o \
    build/us/src/libultra/al_heap_init.c.o \
    build/us/asm/us/main_80105B10_to_8010D660.s.o \
    build/us/src/libultra/os_get_thread_pri.c.o \
    build/us/asm/us/main_8010D680_to_8010F6A0.s.o \
    build/us/src/libultra/os_dp_device_busy.c.o \
    build/us/asm/us/main_8010F6D0_to_8010FE90.s.o \
    build/us/src/libultra/os_si_device_busy.c.o \
    build/us/asm/us/main_8010FEC0_to_80110490.s.o \
    build/us/src/libultra/os_sp_set_status.c.o \
    build/us/src/libultra/os_sp_task_yield.c.o \
    build/us/asm/us/main_801104C0_to_80110540.s.o \
    build/us/src/libultra/os_sp_get_status.c.o \
    build/us/asm/us/main_80110550_to_80110750.s.o \
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
