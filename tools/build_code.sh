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
    -o build/us/asm/us/main_after_8007ADB0.s.o asm/us/main_after_8007ADB0.s

tools/bootstrap_ido.sh
mkdir -p build/us/src/code
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/code/unknown_8007ADB0.c.o src/code/unknown_8007ADB0.c

"${tool_prefix}objcopy" -I binary -O elf32-tradbigmips -B mips \
    assets/extracted/us/ipl3.bin build/us/assets/extracted/us/ipl3.bin.o

"${tool_prefix}nm" -u \
    build/us/asm/us/main_before_8007ADB0.s.o \
    build/us/src/code/unknown_8007ADB0.c.o \
    build/us/asm/us/main_after_8007ADB0.s.o \
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
