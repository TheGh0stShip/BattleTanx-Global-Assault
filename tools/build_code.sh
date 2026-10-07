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
    -o build/us/asm/us/main.s.o asm/us/main.s
"${tool_prefix}objcopy" -I binary -O elf32-tradbigmips -B mips \
    assets/extracted/us/ipl3.bin build/us/assets/extracted/us/ipl3.bin.o

# The first two are BSS bounds outside the ROM-backed segment. The latter two
# are false jump targets decoded from embedded data and retain their raw values.
"${tool_prefix}ld.bfd" -EB -T battletanx_ga.ld \
    -Map build/us/battletanx_ga.map \
    --defsym D_8021E0B8=0x8021E0B8 \
    --defsym D_803B17B0=0x803B17B0 \
    --defsym func_8E180004=0x8E180004 \
    --defsym func_80000000=0x80000000 \
    -o build/us/battletanx_ga.elf
"${tool_prefix}objcopy" -O binary \
    build/us/battletanx_ga.elf build/us/battletanx_ga.code.bin

actual_size="$(wc -c < build/us/battletanx_ga.code.bin)"
[[ "$actual_size" -eq 1052672 ]] || {
    echo "Reconstructed code region is $actual_size bytes; expected 1052672" >&2
    exit 1
}

echo "Built exact-reconstruction candidate: build/us/battletanx_ga.code.bin"
