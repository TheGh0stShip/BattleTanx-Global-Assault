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

mkdir -p build/us/asm/us build/us/asm/us/data build/us/assets/extracted/us

# These bytes spell ASCII "REMA" and are data, despite decoding as a branch.
# Emitting the word directly avoids a false cross-file branch relocation.
for asm_source in asm/us/*.s; do
    sed -i 's/beql       \$s2, \$a1, \.\?L80099664/.word      0x52454D41/' \
        "$asm_source"
    sed -i -E \
        's/^dlabel (osViClock|__osShutdown|__OSGlobalIntMask|osClockRate|D_80126F80|xlitob_data_0000|xlitob_data_0014)$/dlabel __retail_\1/' \
        "$asm_source"
done

"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/header.s.o asm/us/header.s
if false; then
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_before_80077C40.s.o asm/us/main_before_80077C40.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80077FD0_to_80078048.s.o asm/us/main_80077FD0_to_80078048.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80077FD0_to_80078048.s.o .text 0x78 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80078274_to_80078908.s.o asm/us/main_80078274_to_80078908.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80078274_to_80078908.s.o .text 0x694 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80078ADC_to_80078C68.s.o asm/us/main_80078ADC_to_80078C68.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80078ADC_to_80078C68.s.o .text 0x18c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80078CD8_to_8007A710.s.o asm/us/main_80078CD8_to_8007A710.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80078CD8_to_8007A710.s.o .text 0x1a38 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007A75C_to_8007AC34.s.o asm/us/main_8007A75C_to_8007AC34.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007A75C_to_8007AC34.s.o .text 0x4d8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007AD40_to_8007AD94.s.o asm/us/main_8007AD40_to_8007AD94.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007AD40_to_8007AD94.s.o .text 0x54 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007ADF0_to_8007B020.s.o asm/us/main_8007ADF0_to_8007B020.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007ADF0_to_8007B020.s.o .text 0x230 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007B0E4_to_8007B1F0.s.o asm/us/main_8007B0E4_to_8007B1F0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007B0E4_to_8007B1F0.s.o .text 0x10c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007B8EC_to_8007BCF0.s.o asm/us/main_8007B8EC_to_8007BCF0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007B8EC_to_8007BCF0.s.o .text 0x404 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007C364_to_8007D33C.s.o asm/us/main_8007C364_to_8007D33C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007C364_to_8007D33C.s.o .text 0xfd8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007D468_to_8007D470.s.o asm/us/main_8007D468_to_8007D470.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007D468_to_8007D470.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007D5B0_to_8007D694.s.o asm/us/main_8007D5B0_to_8007D694.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007D5B0_to_8007D694.s.o .text 0xe4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007D69C_to_8007D710.s.o asm/us/main_8007D69C_to_8007D710.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007D69C_to_8007D710.s.o .text 0x74 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007D718_to_8007D720.s.o asm/us/main_8007D718_to_8007D720.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007D718_to_8007D720.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007D7C4_to_8007E1FC.s.o asm/us/main_8007D7C4_to_8007E1FC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007D7C4_to_8007E1FC.s.o .text 0xa38 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007E210_to_8007E778.s.o asm/us/main_8007E210_to_8007E778.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007E210_to_8007E778.s.o .text 0x568 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007E7A8_to_80081FD8.s.o asm/us/main_8007E7A8_to_80081FD8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007E7A8_to_80081FD8.s.o .text 0x3830 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80082004_to_80082B40.s.o asm/us/main_80082004_to_80082B40.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80082004_to_80082B40.s.o .text 0xb3c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80082B68_to_80082BD4.s.o asm/us/main_80082B68_to_80082BD4.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80082B68_to_80082BD4.s.o .text 0x6c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80082C1C_to_80082D60.s.o asm/us/main_80082C1C_to_80082D60.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80082C1C_to_80082D60.s.o .text 0x144 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80082D98_to_80082FE0.s.o asm/us/main_80082D98_to_80082FE0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80082D98_to_80082FE0.s.o .text 0x248 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80083028_to_80083FCC.s.o asm/us/main_80083028_to_80083FCC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80083028_to_80083FCC.s.o .text 0xfa4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80083FF8_to_80084C50.s.o asm/us/main_80083FF8_to_80084C50.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80083FF8_to_80084C50.s.o .text 0xc58 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80084CC8_to_800859A8.s.o asm/us/main_80084CC8_to_800859A8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_80084CC8_to_800859A8.s.o .text 0xce0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800859E4_to_8009C284.s.o asm/us/main_800859E4_to_8009C284.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800859E4_to_8009C284.s.o .text 0x168a0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009C31C_to_8009D144.s.o asm/us/main_8009C31C_to_8009D144.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009C31C_to_8009D144.s.o .text 0xe28 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009D168_to_8009D578.s.o asm/us/main_8009D168_to_8009D578.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009D168_to_8009D578.s.o .text 0x410 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009D5B4_to_8009D6DC.s.o asm/us/main_8009D5B4_to_8009D6DC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009D5B4_to_8009D6DC.s.o .text 0x128 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009D6F8_to_8009D72C.s.o asm/us/main_8009D6F8_to_8009D72C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009D6F8_to_8009D72C.s.o .text 0x34 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009D75C_to_8009D81C.s.o asm/us/main_8009D75C_to_8009D81C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009D75C_to_8009D81C.s.o .text 0xc0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009D8A0_to_8009D914.s.o asm/us/main_8009D8A0_to_8009D914.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009D8A0_to_8009D914.s.o .text 0x74 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009D960_to_8009DA34.s.o asm/us/main_8009D960_to_8009DA34.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009D960_to_8009DA34.s.o .text 0xd4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009DAB0_to_8009DB0C.s.o asm/us/main_8009DAB0_to_8009DB0C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009DAB0_to_8009DB0C.s.o .text 0x5c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009DB2C_to_8009E044.s.o asm/us/main_8009DB2C_to_8009E044.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009DB2C_to_8009E044.s.o .text 0x518 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009E0E8_to_8009EEA0.s.o asm/us/main_8009E0E8_to_8009EEA0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009E0E8_to_8009EEA0.s.o .text 0xdb8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009EED8_to_8009EEE0.s.o asm/us/main_8009EED8_to_8009EEE0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009EED8_to_8009EEE0.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009EF4C_to_8009F064.s.o asm/us/main_8009EF4C_to_8009F064.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009EF4C_to_8009F064.s.o .text 0x118 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009F5AC_to_8009F768.s.o asm/us/main_8009F5AC_to_8009F768.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009F5AC_to_8009F768.s.o .text 0x1bc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009FCCC_to_8009FF1C.s.o asm/us/main_8009FCCC_to_8009FF1C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009FCCC_to_8009FF1C.s.o .text 0x250 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009FFB8_to_800A1280.s.o asm/us/main_8009FFB8_to_800A1280.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009FFB8_to_800A1280.s.o .text 0x12c8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A1384_to_800A179C.s.o asm/us/main_800A1384_to_800A179C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A1384_to_800A179C.s.o .text 0x418 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A18D0_to_800A1A28.s.o asm/us/main_800A18D0_to_800A1A28.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A18D0_to_800A1A28.s.o .text 0x158 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A1B44_to_800A2B74.s.o asm/us/main_800A1B44_to_800A2B74.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A1B44_to_800A2B74.s.o .text 0x1030 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A2B9C_to_800A2DFC.s.o asm/us/main_800A2B9C_to_800A2DFC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A2B9C_to_800A2DFC.s.o .text 0x260 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A2E5C_to_800A4098.s.o asm/us/main_800A2E5C_to_800A4098.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A2E5C_to_800A4098.s.o .text 0x123c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A40CC_to_800A6ABC.s.o asm/us/main_800A40CC_to_800A6ABC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A40CC_to_800A6ABC.s.o .text 0x29f0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A6B7C_to_800A7290.s.o asm/us/main_800A6B7C_to_800A7290.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A6B7C_to_800A7290.s.o .text 0x714 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A72C0_to_800A8B14.s.o asm/us/main_800A72C0_to_800A8B14.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A72C0_to_800A8B14.s.o .text 0x1854 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A8B38_to_800A9054.s.o asm/us/main_800A8B38_to_800A9054.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A8B38_to_800A9054.s.o .text 0x51c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A9080_to_800A974C.s.o asm/us/main_800A9080_to_800A974C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A9080_to_800A974C.s.o .text 0x6cc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A977C_to_800A9BF0.s.o asm/us/main_800A977C_to_800A9BF0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A977C_to_800A9BF0.s.o .text 0x474 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A9C24_to_800A9D50.s.o asm/us/main_800A9C24_to_800A9D50.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A9C24_to_800A9D50.s.o .text 0x12c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A9D78_to_800AA598.s.o asm/us/main_800A9D78_to_800AA598.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A9D78_to_800AA598.s.o .text 0x820 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800AA5C4_to_800B0444.s.o asm/us/main_800AA5C4_to_800B0444.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800AA5C4_to_800B0444.s.o .text 0x5e80 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800B044C_to_800B06A8.s.o asm/us/main_800B044C_to_800B06A8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800B044C_to_800B06A8.s.o .text 0x25c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800B06D8_to_800B5F30.s.o asm/us/main_800B06D8_to_800B5F30.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800B06D8_to_800B5F30.s.o .text 0x5858 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800B5F70_to_800B99C0.s.o asm/us/main_800B5F70_to_800B99C0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800B5F70_to_800B99C0.s.o .text 0x3a50 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800B99F8_to_800B9A4C.s.o asm/us/main_800B99F8_to_800B9A4C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800B99F8_to_800B9A4C.s.o .text 0x54 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800B9AB0_to_800B9C68.s.o asm/us/main_800B9AB0_to_800B9C68.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800B9AB0_to_800B9C68.s.o .text 0x1b8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800B9CAC_to_800B9D4C.s.o asm/us/main_800B9CAC_to_800B9D4C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800B9CAC_to_800B9D4C.s.o .text 0xa0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800B9E24_to_800BD880.s.o asm/us/main_800B9E24_to_800BD880.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800B9E24_to_800BD880.s.o .text 0x3a5c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800BD93C_to_800BFD40.s.o asm/us/main_800BD93C_to_800BFD40.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800BD93C_to_800BFD40.s.o .text 0x2404 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800BFDA4_to_800C03F0.s.o asm/us/main_800BFDA4_to_800C03F0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800BFDA4_to_800C03F0.s.o .text 0x64c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C041C_to_800C0800.s.o asm/us/main_800C041C_to_800C0800.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C041C_to_800C0800.s.o .text 0x3e4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C08E0_to_800C0A64.s.o asm/us/main_800C08E0_to_800C0A64.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C08E0_to_800C0A64.s.o .text 0x184 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C0A6C_to_800C0C18.s.o asm/us/main_800C0A6C_to_800C0C18.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C0A6C_to_800C0C18.s.o .text 0x1ac --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C11B8_to_800C1420.s.o asm/us/main_800C11B8_to_800C1420.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C11B8_to_800C1420.s.o .text 0x268 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C1484_to_800C1578.s.o asm/us/main_800C1484_to_800C1578.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C1484_to_800C1578.s.o .text 0xf4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C16B0_to_800C17C8.s.o asm/us/main_800C16B0_to_800C17C8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C16B0_to_800C17C8.s.o .text 0x118 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C2528_to_800C27EC.s.o asm/us/main_800C2528_to_800C27EC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C2528_to_800C27EC.s.o .text 0x2c4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C2924_to_800C30A0.s.o asm/us/main_800C2924_to_800C30A0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C2924_to_800C30A0.s.o .text 0x77c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C4BB4_to_800C4E24.s.o asm/us/main_800C4BB4_to_800C4E24.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C4BB4_to_800C4E24.s.o .text 0x270 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C6914_to_800C6918.s.o asm/us/main_800C6914_to_800C6918.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C6914_to_800C6918.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/data/main_800C6918_textbin.s.o asm/us/data/main_800C6918_textbin.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/data/main_800C6918_textbin.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C7594_to_800C7650.s.o asm/us/main_800C7594_to_800C7650.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C7594_to_800C7650.s.o .text 0xbc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C8350_to_800C8484.s.o asm/us/main_800C8350_to_800C8484.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C8350_to_800C8484.s.o .text 0x134 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C98E8_to_800CA1A8.s.o asm/us/main_800C98E8_to_800CA1A8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C98E8_to_800CA1A8.s.o .text 0x8c0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800CA620_to_800CAA0C.s.o asm/us/main_800CA620_to_800CAA0C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800CA620_to_800CAA0C.s.o .text 0x3ec --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800CAA48_to_800CAB44.s.o asm/us/main_800CAA48_to_800CAB44.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800CAA48_to_800CAB44.s.o .text 0xfc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800CB10C_to_800CB110.s.o asm/us/main_800CB10C_to_800CB110.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800CB10C_to_800CB110.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800CD57C_to_800CD85C.s.o asm/us/main_800CD57C_to_800CD85C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800CD57C_to_800CD85C.s.o .text 0x2e0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800CD96C_to_800CDD70.s.o asm/us/main_800CD96C_to_800CDD70.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800CD96C_to_800CDD70.s.o .text 0x404 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800CE610_to_800CE814.s.o asm/us/main_800CE610_to_800CE814.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800CE610_to_800CE814.s.o .text 0x204 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800CFA84_to_800CFBD8.s.o asm/us/main_800CFA84_to_800CFBD8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800CFA84_to_800CFBD8.s.o .text 0x154 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800CFD18_to_800CFDD0.s.o asm/us/main_800CFD18_to_800CFDD0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800CFD18_to_800CFDD0.s.o .text 0xb8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D0070_to_800D05E0.s.o asm/us/main_800D0070_to_800D05E0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D0070_to_800D05E0.s.o .text 0x570 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D0858_to_800D0960.s.o asm/us/main_800D0858_to_800D0960.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D0858_to_800D0960.s.o .text 0x108 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D0A74_to_800D0DF8.s.o asm/us/main_800D0A74_to_800D0DF8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D0A74_to_800D0DF8.s.o .text 0x384 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D3C64_to_800D404C.s.o asm/us/main_800D3C64_to_800D404C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D3C64_to_800D404C.s.o .text 0x3e8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D5C7C_to_800D5C80.s.o asm/us/main_800D5C7C_to_800D5C80.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D5C7C_to_800D5C80.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D5E2C_to_800D67F0.s.o asm/us/main_800D5E2C_to_800D67F0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D5E2C_to_800D67F0.s.o .text 0x9c4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D6E34_to_800D6E40.s.o asm/us/main_800D6E34_to_800D6E40.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D6E34_to_800D6E40.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D7DD4_to_800D7DE0.s.o asm/us/main_800D7DD4_to_800D7DE0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D7DD4_to_800D7DE0.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D84D8_to_800D84DC.s.o asm/us/main_800D84D8_to_800D84DC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D84D8_to_800D84DC.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D88E8_to_800D88F0.s.o asm/us/main_800D88E8_to_800D88F0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D88E8_to_800D88F0.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D92D4_to_800D92E0.s.o asm/us/main_800D92D4_to_800D92E0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D92D4_to_800D92E0.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D9B48_to_800D9B50.s.o asm/us/main_800D9B48_to_800D9B50.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D9B48_to_800D9B50.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800D9D50_to_800DA340.s.o asm/us/main_800D9D50_to_800DA340.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800D9D50_to_800DA340.s.o .text 0x5f0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DA7D0_to_800DA984.s.o asm/us/main_800DA7D0_to_800DA984.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DA7D0_to_800DA984.s.o .text 0x1b4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DAAE0_to_800DAC34.s.o asm/us/main_800DAAE0_to_800DAC34.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DAAE0_to_800DAC34.s.o .text 0x154 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DB4E8_to_800DB6F0.s.o asm/us/main_800DB4E8_to_800DB6F0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DB4E8_to_800DB6F0.s.o .text 0x208 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DB850_to_800DC214.s.o asm/us/main_800DB850_to_800DC214.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DB850_to_800DC214.s.o .text 0x9c4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DC7A4_to_800DC7B0.s.o asm/us/main_800DC7A4_to_800DC7B0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DC7A4_to_800DC7B0.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DD28C_to_800DD290.s.o asm/us/main_800DD28C_to_800DD290.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DD28C_to_800DD290.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DD75C_to_800DDF04.s.o asm/us/main_800DD75C_to_800DDF04.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DD75C_to_800DDF04.s.o .text 0x7a8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DF0C4_to_800DF0D0.s.o asm/us/main_800DF0C4_to_800DF0D0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DF0C4_to_800DF0D0.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800DF89C_to_800E1540.s.o asm/us/main_800DF89C_to_800E1540.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800DF89C_to_800E1540.s.o .text 0x1ca4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E16D8_to_800E18D8.s.o asm/us/main_800E16D8_to_800E18D8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E16D8_to_800E18D8.s.o .text 0x200 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E1BA4_to_800E2018.s.o asm/us/main_800E1BA4_to_800E2018.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E1BA4_to_800E2018.s.o .text 0x474 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E2520_to_800E26A8.s.o asm/us/main_800E2520_to_800E26A8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E2520_to_800E26A8.s.o .text 0x188 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E2F9C_to_800E3404.s.o asm/us/main_800E2F9C_to_800E3404.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E2F9C_to_800E3404.s.o .text 0x468 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E3460_to_800E44C8.s.o asm/us/main_800E3460_to_800E44C8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E3460_to_800E44C8.s.o .text 0x1068 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E48A8_to_800E48E0.s.o asm/us/main_800E48A8_to_800E48E0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E48A8_to_800E48E0.s.o .text 0x38 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E4DA0_to_800E4ECC.s.o asm/us/main_800E4DA0_to_800E4ECC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E4DA0_to_800E4ECC.s.o .text 0x12c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E56C8_to_800E56D0.s.o asm/us/main_800E56C8_to_800E56D0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E56C8_to_800E56D0.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E58C4_to_800E58D0.s.o asm/us/main_800E58C4_to_800E58D0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E58C4_to_800E58D0.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E5BB8_to_800E6040.s.o asm/us/main_800E5BB8_to_800E6040.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E5BB8_to_800E6040.s.o .text 0x488 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E64EC_to_800E64F0.s.o asm/us/main_800E64EC_to_800E64F0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E64EC_to_800E64F0.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E6B04_to_800E713C.s.o asm/us/main_800E6B04_to_800E713C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E6B04_to_800E713C.s.o .text 0x638 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E73B0_to_800E759C.s.o asm/us/main_800E73B0_to_800E759C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E73B0_to_800E759C.s.o .text 0x1ec --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E7768_to_800E7968.s.o asm/us/main_800E7768_to_800E7968.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E7768_to_800E7968.s.o .text 0x200 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E7A10_to_800E7DF8.s.o asm/us/main_800E7A10_to_800E7DF8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E7A10_to_800E7DF8.s.o .text 0x3e8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E7F5C_to_800E7F60.s.o asm/us/main_800E7F5C_to_800E7F60.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E7F5C_to_800E7F60.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E80F4_to_800E8318.s.o asm/us/main_800E80F4_to_800E8318.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E80F4_to_800E8318.s.o .text 0x224 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E8378_to_800E8C88.s.o asm/us/main_800E8378_to_800E8C88.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E8378_to_800E8C88.s.o .text 0x910 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E92E8_to_800E92F0.s.o asm/us/main_800E92E8_to_800E92F0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E92E8_to_800E92F0.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E9990_to_800E9B50.s.o asm/us/main_800E9990_to_800E9B50.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E9990_to_800E9B50.s.o .text 0x1c0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800E9E78_to_800E9E80.s.o asm/us/main_800E9E78_to_800E9E80.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800E9E78_to_800E9E80.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EA224_to_800EA714.s.o asm/us/main_800EA224_to_800EA714.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EA224_to_800EA714.s.o .text 0x4f0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EAC2C_to_800EAC30.s.o asm/us/main_800EAC2C_to_800EAC30.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EAC2C_to_800EAC30.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EAEB4_to_800EAF6C.s.o asm/us/main_800EAEB4_to_800EAF6C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EAEB4_to_800EAF6C.s.o .text 0xb8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EB308_to_800EB4FC.s.o asm/us/main_800EB308_to_800EB4FC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EB308_to_800EB4FC.s.o .text 0x1f4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EB714_to_800EB720.s.o asm/us/main_800EB714_to_800EB720.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EB714_to_800EB720.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EB838_to_800EB934.s.o asm/us/main_800EB838_to_800EB934.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EB838_to_800EB934.s.o .text 0xfc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EBA98_to_800EBCA8.s.o asm/us/main_800EBA98_to_800EBCA8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EBA98_to_800EBCA8.s.o .text 0x210 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EBDA0_to_800EC1C0.s.o asm/us/main_800EBDA0_to_800EC1C0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EBDA0_to_800EC1C0.s.o .text 0x420 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EC1F8_to_800EC4A8.s.o asm/us/main_800EC1F8_to_800EC4A8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EC1F8_to_800EC4A8.s.o .text 0x2b0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EC788_to_800EC790.s.o asm/us/main_800EC788_to_800EC790.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EC788_to_800EC790.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EC8E8_to_800ECEBC.s.o asm/us/main_800EC8E8_to_800ECEBC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EC8E8_to_800ECEBC.s.o .text 0x5d4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800ED4F4_to_800ED63C.s.o asm/us/main_800ED4F4_to_800ED63C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800ED4F4_to_800ED63C.s.o .text 0x148 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800ED694_to_800ED698.s.o asm/us/main_800ED694_to_800ED698.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800ED694_to_800ED698.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800ED804_to_800EDC00.s.o asm/us/main_800ED804_to_800EDC00.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800ED804_to_800EDC00.s.o .text 0x3fc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EDDCC_to_800EDF14.s.o asm/us/main_800EDDCC_to_800EDF14.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EDDCC_to_800EDF14.s.o .text 0x148 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EE288_to_800EE688.s.o asm/us/main_800EE288_to_800EE688.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EE288_to_800EE688.s.o .text 0x400 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EED90_to_800EF400.s.o asm/us/main_800EED90_to_800EF400.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EED90_to_800EF400.s.o .text 0x670 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EF770_to_800EFB30.s.o asm/us/main_800EF770_to_800EFB30.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EF770_to_800EFB30.s.o .text 0x3c0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800EFC68_to_800EFC70.s.o asm/us/main_800EFC68_to_800EFC70.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800EFC68_to_800EFC70.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F01F0_to_800F0618.s.o asm/us/main_800F01F0_to_800F0618.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F01F0_to_800F0618.s.o .text 0x428 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F0A20_to_800F0B08.s.o asm/us/main_800F0A20_to_800F0B08.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F0A20_to_800F0B08.s.o .text 0xe8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F1278_to_800F1770.s.o asm/us/main_800F1278_to_800F1770.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F1278_to_800F1770.s.o .text 0x4f8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F17AC_to_800F17B0.s.o asm/us/main_800F17AC_to_800F17B0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F17AC_to_800F17B0.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F1900_to_800F1B30.s.o asm/us/main_800F1900_to_800F1B30.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F1900_to_800F1B30.s.o .text 0x230 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F2184_to_800F2288.s.o asm/us/main_800F2184_to_800F2288.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F2184_to_800F2288.s.o .text 0x104 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F2BB8_to_800F2BC0.s.o asm/us/main_800F2BB8_to_800F2BC0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F2BB8_to_800F2BC0.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F3208_to_800F32EC.s.o asm/us/main_800F3208_to_800F32EC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F3208_to_800F32EC.s.o .text 0xe4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F3B7C_to_800F3B80.s.o asm/us/main_800F3B7C_to_800F3B80.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F3B7C_to_800F3B80.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F4D74_to_800F4D80.s.o asm/us/main_800F4D74_to_800F4D80.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F4D74_to_800F4D80.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F5264_to_800F5270.s.o asm/us/main_800F5264_to_800F5270.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F5264_to_800F5270.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F6648_to_800F6650.s.o asm/us/main_800F6648_to_800F6650.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F6648_to_800F6650.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F67F0_to_800F6ED0.s.o asm/us/main_800F67F0_to_800F6ED0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F67F0_to_800F6ED0.s.o .text 0x6e0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F7230_to_800F756C.s.o asm/us/main_800F7230_to_800F756C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F7230_to_800F756C.s.o .text 0x33c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F7648_to_800F7870.s.o asm/us/main_800F7648_to_800F7870.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F7648_to_800F7870.s.o .text 0x228 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F7C24_to_800F7C30.s.o asm/us/main_800F7C24_to_800F7C30.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F7C24_to_800F7C30.s.o .text 0xc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F7EB8_to_800F7EC0.s.o asm/us/main_800F7EB8_to_800F7EC0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F7EB8_to_800F7EC0.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F809C_to_800F80A0.s.o asm/us/main_800F809C_to_800F80A0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F809C_to_800F80A0.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F83B8_to_800F83C0.s.o asm/us/main_800F83B8_to_800F83C0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F83B8_to_800F83C0.s.o .text 0x8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800F865C_to_800F8660.s.o asm/us/main_800F865C_to_800F8660.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800F865C_to_800F8660.s.o .text 0x4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801029F0_to_80103160.s.o asm/us/main_801029F0_to_80103160.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80103190_to_801037B0.s.o asm/us/main_80103190_to_801037B0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_801039B0_to_80104EC0.s.o asm/us/main_801039B0_to_80104EC0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80104FA0_to_801058B0.s.o asm/us/main_80104FA0_to_801058B0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80105A60_to_80105A70.s.o asm/us/main_80105A60_to_80105A70.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80105B10_to_80105F20.s.o asm/us/main_80105B10_to_80105F20.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80106070_to_801068F0.s.o asm/us/main_80106070_to_801068F0.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80106EE0_to_80107A30.s.o asm/us/main_80106EE0_to_80107A30.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80107D60_to_80108530.s.o asm/us/main_80107D60_to_80108530.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80109030_to_80109090.s.o asm/us/main_80109030_to_80109090.s
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_80109490_to_8010A9A0.s.o asm/us/main_80109490_to_8010A9A0.s
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

fi

# Assemble the generated segment set rather than maintaining a second,
# hand-written copy of every split in splat.yaml.
while IFS= read -r -d '' asm_source; do
    asm_relative="${asm_source#asm/us/}"
    asm_name="$(basename "$asm_relative" .s)"
    asm_object="build/us/asm/us/${asm_relative}.o"
    mkdir -p "$(dirname "$asm_object")"
    "${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
        -o "$asm_object" "$asm_source"
    if [[ "$asm_name" =~ ^main_([0-9A-Fa-f]{8})_to_([0-9A-Fa-f]{8})$ ]]; then
        range_start=$((16#${BASH_REMATCH[1]}))
        range_end=$((16#${BASH_REMATCH[2]}))
        python3 tools/trim_elf32_section.py "$asm_object" .text \
            "$((range_end - range_start))" --alignment 4
    elif [[ "$asm_name" == "main_800C6918_textbin" ]]; then
        python3 tools/trim_elf32_section.py "$asm_object" .text 0x8 --alignment 4
    fi
done < <(find asm/us -type f -name '*.s' -print0)

tools/bootstrap_ido.sh
tools/bootstrap_kmc_gcc.sh
mkdir -p build/us/src/code
mkdir -p build/us/src/code/libmus
# libmus was assembled with reorder enabled, so preserve KMC's raw assembly and
# its assembler-scheduled delay slots instead of applying the gameplay normalizer.
while read -r unit text_size; do
    .toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
        -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude -Isrc/code/libmus \
        -o "build/us/src/code/libmus/${unit}.raw.s" \
        "src/code/libmus/${unit}.c"
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
        -o "build/us/src/code/libmus/${unit}.c.o" \
        "build/us/src/code/libmus/${unit}.raw.s"
    python3 tools/trim_elf32_section.py \
        "build/us/src/code/libmus/${unit}.c.o" .text "$text_size" --alignment 4
    case "$unit" in
        800FBF94_player_main)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/libmus/${unit}.c.o" .rodata 0x8 --alignment 8 ;;
        800FD2A0_remap_ptr_bank)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/libmus/${unit}.c.o" .rodata 0x8 --alignment 8 ;;
        800FDD20_player_commands)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/libmus/${unit}.c.o" .rodata 0x40 --alignment 4 ;;
        800FE710_n_syn_custom_fx)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/libmus/${unit}.c.o" .rodata 0x10 --alignment 4 ;;
    esac
done <<'LIBMUS_UNITS'
800FB244_master_volume 0x2C
800FB270_start_song 0x30
800FB2A0_start_song_marker 0x250
func_800FB4F0 0x80
800FB570_player_api 0x264
800FB7D4_player_api_handles 0x5AC
800FBD80_player_api_misc_pre 0x48
func_800FBDC8 0x64
800FBE2C_player_api_misc_post 0x168
800FBF94_player_main 0x8D0
800FC864_player_voice 0x34C
800FCBB0_player_effects 0x55C
800FD10C_player_bank_math 0x194
800FD2A0_remap_ptr_bank 0x198
800FD438_random_range 0x94
func_800FD4CC 0x13C
800FD608_player_start_pre 0x458
800FDCCC_channel_flags 0x54
800FDD20_player_commands 0x9F0
800FE710_n_syn_custom_fx 0x850
800FEF60_mus_dma_sched 0x600
800FF560_mus_audio_thread 0x2C0
800FF820_mus_frame_size 0x130
800FF950_mus_heap_mem 0x140
800FFA90_n_auxbus_pull 0xA0
LIBMUS_UNITS
mkdir -p build/us/src/code/n_audio
# The customized n_audio library uses the same raw KMC assembler scheduling as
# libmus. Keep this separate from gameplay's normalized assembly pipeline.
while read -r unit text_size; do
    .toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
        -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude -Isrc/code/n_audio \
        -o "build/us/src/code/n_audio/${unit}.raw.s" \
        "src/code/n_audio/${unit}.c"
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
        -o "build/us/src/code/n_audio/${unit}.c.o" \
        "build/us/src/code/n_audio/${unit}.raw.s"
    python3 tools/trim_elf32_section.py \
        "build/us/src/code/n_audio/${unit}.c.o" .text "$text_size" --alignment 4
    case "$unit" in
        800FFB30_n_drvrnew)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/n_audio/${unit}.c.o" .rodata 0x30 --alignment 16 ;;
        80100050_n_env)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/n_audio/${unit}.c.o" .rodata 0x70 --alignment 16 ;;
        80101140_n_resample)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/n_audio/${unit}.c.o" .rodata 0x10 --alignment 16 ;;
        80101320_n_reverb)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/n_audio/${unit}.c.o" .rodata 0x40 --alignment 16 ;;
        80102350_n_synthesizer)
            python3 tools/trim_elf32_section.py \
                "build/us/src/code/n_audio/${unit}.c.o" .rodata 0x20 --alignment 16 ;;
    esac
done <<'N_AUDIO_UNITS'
800FFB30_n_drvrnew 0x520
80100050_n_env 0x9D0
80100E68_n_load_param 0x18C
80100FF4_decode_chunk 0x14C
80101140_n_resample 0x1E0
80101320_n_reverb 0x9F0
80101D10_n_alinit 0x80
80101D90_n_synaddplayer 0x50
80101DE0_n_synallocvoice 0x1E0
80101FC0_n_syndelete 0x10
80101FD0_n_synsetfxmix 0xA0
80102070_n_synsetpan 0x90
80102100_n_synsetpitch 0x90
80102190_n_synsetvol 0xB0
80102240_n_synstartvoice 0x90
801022D0_n_synstopvoice 0x80
80102350_n_synthesizer 0x560
801028B0_n_save 0x50
80102900_n_mainbus 0x80
80102980_n_synallocfx 0x50
N_AUDIO_UNITS
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
            gameplay_stub turn_adjust hud_state hud_tree_update hud_modes hud_stub \
            race_assets_init race_mode_query race_hud_layout race_marker_project race_map_draw race_state_init controller_menu_state \
            hud_volume_adjust hud_controls_page hud_page_next controller_save_slots \
            results_menu_callbacks results_panel_setup results_split_setup spawn_color_lookup \
            hud_key_nav hud_slots_layout controls_config controls_preset hud_binding_select \
            hud_tree_remove hud_tree_draw hud_frame hud_mode_table hud_mode_cycle_full hud_exit_dispatch \
            hud_menu_open hud_option_select hud_list_select_full hud_player_page \
            race_popup_setup results_bar_step results_rank_label \
            results_time_text cheat_codes \
            hud_secondary hud_primary_modes hud_transition registry_lookup \
            hud_ready hud_clear hud_navigation player_color entry_scan \
            entry_flags hud_callbacks hud_panel_callback hud_list_callback \
            hud_root_callback hud_layout hud_list_trigger \
            hud_entry_values hud_list_reset hud_selection_apply \
            hud_primary_trigger display_registry display_color display_commands \
            menu_state menu_toggle menu_countdown table_lookup \
            hud_row_preset \
            hud_row_defaults \
            hud_exit_request \
            hud_list_refresh \
            hud_list_focus_cycle \
            hud_list_open \
            hud_list_close \
            hud_alt_exit_request \
            hud_page_select \
            hud_page_focus_cycle \
            game_mode8_toggled \
            hud_page_focus \
            hud_page_exit_request \
            hud_page_show \
            hud_page_open \
            hud_page_close \
            race_assets_load \
            race_hud_init \
            race_result_setup \
            race_object_timestamps \
            race_state_reset \
            race_event_push \
            race_slot_clear \
            race_player_panel \
            race_player_message \
            race_player_pair_set \
            race_display_value_set \
            race_player_reset \
            race_timer_queries \
            race_popup_show \
            race_flag_queries \
            race_player_widget \
            race_widget_values \
            race_widget_clear \
            race_widget_reset \
            race_widget_flag_set \
            results_assets_load \
            controls_button_index_map \
            results_record_write \
            results_time_format \
            results_time_compare \
            results_screen_setup \
            results_continue \
            results_return \
            results_option_codes \
            results_ready_flags_a \
            results_ready_flags_b \
            results_ready_flags_c \
            results_status_flags \
            results_assets_reload \
            game_state_clear \
            spawn_ctl_index \
            spawn_ctl_alloc \
            audio_listener_update \
            race_buffers_alloc \
            player_attach_effect_create \
            player_attach_effect_matrix \
            player_attach_effect_matrix_cb \
            actor_collision_free \
            turret_message_handlers \
            single_player_check \
            trigger_zone_query \
            minimap_layer_build \
            model_texture_find \
            model_bounds_center \
            debris_piece_draw \
            debris_burst_spawn \
            debris_burst_spawn_tinted \
            debris_scatter_spawn \
            projectile_model_draw \
            projectile_model_create \
            projectile_model_matrix \
            projectile_model_matrix_cb \
            projectile_trail_draw \
            projectile_launch_delayed \
            projectile_launch_delayed_tick \
            model_mesh_partition \
            structure_create \
            spawner_target_query \
            unit_spawner_create \
            unit_spawner_update \
            prop_multiplayer_filter \
            level_prop_points_find \
            foliage_prop_create \
            foliage_prop_handlers \
            turret_destroy \
            turret_draw_create \
            turret_message_handler \
            turret_line_of_sight \
            wreck_timer_duration \
            flag_capture_attempt \
            actor_kind16_link_clear \
            sound_emitter_delayed_spawn \
            ambient_sound_stop_first \
            bridge_create_handlers \
            lightning_arc_update \
            crate_model_randomize \
            crate_actor_attach \
            pending_list_flush \
            powerup_flag_players \
            powerup_count_refresh \
            pickup_actor_count \
            game_mode_has_pickups \
            slist_remove_count \
            mission_select_init \
            mission_entry_query \
            mission_time_bonus \
            mission_event_forward \
            building_target_query \
            building_destroy_on_hit \
            building_damage_apply \
            building_create \
            building_anim_frame_tick \
            building_destroy \
            impact_flash_spawn \
            impact_flash_expire \
            player_projectile_launch \
            mission_counter_release \
            splash_damage_falloff \
            projectile_spawn_typed \
            shockwave_ring_spawn \
            shockwave_ring_draw \
            destructible_prop_create \
            destructible_anim_frame_tick \
            projectile_target_query \
            destructible_prop_on_hit \
            destructible_prop_on_hit_alt \
            prop_destroy_slot_release \
            prop_target_query \
            prop_debris_burst_on_hit \
            prop_debris_burst_on_hit_alt \
            barrier_break_open \
            particle_emitter_tail \
            particle_emitter_create \
            particle_pool_init \
            particle_emitter_update \
            particle_free_list \
            particle_node_list \
            anim_list_register_global \
            hazard_actor_spawn \
            hazard_actor_spawn_simple \
            object_state_query \
            mine_message_handlers \
            powerup_pad_create \
            powerup_pad_draw \
            barrel_handlers \
            falling_crate_update \
            decal_spawn_draw \
            color_interpolate \
            mesh_vertex_transform \
            tracer_spawn \
            effect_table_reset \
            generator_damage_apply \
            generator_message_damage \
            generator_hit_query \
            generator_splash_damage \
            generator_damage_tick \
            generator_dispatch_bonus \
            effect_expire_100 \
            wreck_debris_spawn \
            wreck_debris_update \
            anim_colors_set \
            anim_list_create \
            anim_list_reset \
            anim_list_register \
            spark_spawn \
            smoke_puff_spawn \
            flicker_prop_create \
            flicker_prop_draw \
            flicker_prop_message \
            flicker_prop_message_cb \
            static_prop_create \
            static_prop_expire \
            static_prop_draw \
            model_variant_get \
            model_cache_globals \
            artillery_emplacement_update \
            artillery_emplacement_disable \
            artillery_emplacement_create \
            artillery_projectile_create \
            hud_slot_icons_update \
            hud_menu_entries_apply_settings \
            hud_entry_set_label \
            race_player_set_vehicle_label \
            race_type_name_lookup \
            race_player_set_layout \
            race_mode_icon_get \
            race_player_set_mode_icon \
            race_slot_set_digit \
            controls_bindings_save \
            controls_bindings_load \
            spawn_effect_by_type \
            800D12B0_script_command_decode \
            seq_event_spawn_tick \
            seq_event_start \
            seq_script_load \
            seq_level_header_load \
            seq_events_update \
            800D3C64_seq_point_track \
            800D5E2C_script_particle_update \
            800D9D50_debris_spawn \
            800DA1D8_debris_piece_update \
            800DA7D0_debris_group_spawn \
            800DAAE0_projectile_spawn \
            800DB1B0_projectile_segment_spawn \
            800DB6F0_track_angle_unit \
            800DD75C_structure_hit_dispatch \
            800DD82C_structure_debris_spawn \
            800DE374_structure_message \
            8008C5D8_tank_fire_weapon \
            800E16D8_foliage_prop_destroy \
            800E1BB0_turret_sweep \
            800E2520_turret_message_handlers \
            800E2F9C_wreck_update \
            800E48A8_flag_position_query \
            800E66A8_crate_particle_attach \
            800E6B04_crate_unit \
            800E73B0_crate_part_release \
            800E7768_crate_burst_spawn \
            800E7A10_powerup_flag_messages \
            slot_condition_check \
            player_message_hit \
            player_dispatch \
            wreck_message_hit \
            slot_mode_name_get \
            slot_mode_title_get \
            slot_mode_subtitle_get \
            slot_mode_description_get \
            slot_mode_value_get \
            model_pickup_create \
            model_mode_objective_message \
            model_message_damage_u8 \
            model_turret_dispatch \
            model_shielded_dispatch \
            model_message_damage_destroy \
            prop_message_shatter \
            effect_segment_append \
            effect_segments_draw \
            effect_message_damage \
            effect_message_damage_guarded \
            hud_player_slots_cycle \
            hud_marker_update \
            hud_mode_cycle \
            hud_panel_layout \
            hud_panel_refresh \
            hud_panel_select \
            hud_value_to_byte \
            race_frame_border_draw \
            race_start_check \
            race_timer_update \
            race_player_icons_draw \
            race_lap_times_clamp \
            controller_slots_scan \
            controller_menu_input \
            controller_queue_push \
            controller_slot_find \
            results_kills_tick \
            results_deaths_tick \
            results_score_tick \
            results_score_finish \
            results_bonus_tick \
            results_bonus_finish \
            results_time_tick \
            results_columns_draw \
            hud_element_unlink \
            hud_element_alloc \
            spawn_nearest_type1 \
            spawn_nearest_type28 \
            spawn_nearest_type31 \
            spawn_counters \
            spawn_object_command \
            seq_script_tick \
            seq_spawn_script_tick \
            seq_event_script_tick \
            seq_spawn_events_update \
            seq_command_dispatch \
            script_event_finish \
            hud_draw_list \
            script_particle_spawn \
            effect_draw \
            effect_code_table \
            effect_code_parse \
            player_menu_update \
            player_cursor_anim \
            player_menu_draw \
            effect_node_spawn \
            actor_model_draw \
            actor_timer_update \
            actor_message_handler \
            turret_state_messages \
            turret_weights_total \
            turret_weighted_pick \
            turret_random_slot \
            turret_model_draw \
            turret_spawn_random \
            single_spawn_timer_update \
            trigger_particles_spawn \
            trigger_particles_update \
            minimap_blips_spawn \
            model_bounds_compute \
            projectile_bounce \
            projectile_life_tick \
            projectile_fire \
            structure_flicker_roll \
            structure_model_draw \
            structure_owner_award \
            spawner_state_update \
            spawner_owner_score \
            spawner_message_handler \
            slot_corners_build \
            slot_entity_attach \
            slot_angle_classify \
            slot_entry_lookup \
            target_search \
            wreck_spawn \
            turret_spawn_at \
            wreck_ctl \
            object_damage \
            flag_owner_capture \
            flag_owner_score \
            actor_wreck_spawn_at \
            sound_emitter_draw \
            bridge_segments_init \
            bridge_piece_spawn \
            crate_owner_check \
            crate_contents_spawn \
            pending_spawns_flush \
            pickup_find_by_id \
            game_team_compare \
            mission_code_parse \
            mission_flag_set \
            mission_props_spawn \
            mission_targets_query \
            mission_marker_draw \
            impact_debris_spray \
            mission_objectives_update \
            projectile_shell_draw \
            shockwave_expand \
            prop_spawn_child \
            prop_spawn_pickup \
            prop_height_update \
            particle_emitter_free \
            particle_smoke_update \
            particle_hit_spawn \
            particle_system_create \
            particle_owner_message \
            anim_keyframes_apply \
            hazard_state_update \
            hazard_model_draw \
            hazard_message_handler \
            mine_trigger_update \
            powerup_timer_update \
            barrel_explode \
            mesh_wave_update \
            tracer_trail_update \
            effect_prop_spawn \
            effect_targets_query \
            effect_targets_damage \
            effect_burn_update \
            effect_beam_draw \
            generator_spawn \
            anim_alpha_fade_draw \
            spark_burst_update \
            static_in_radius \
            model_in_draw_range \
            artillery_target_track \
            mesh_rings_draw; do
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
        "build/us/src/${unit}.raw.s" "build/us/src/${unit}.s" --legacy
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
        "build/us/src/${unit}.raw.s" "build/us/src/${unit}.s" --legacy
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
        -o "build/us/src/${unit}.c.o" "build/us/src/${unit}.s"
    python3 tools/trim_elf32_section.py \
        "build/us/src/${unit}.c.o" .text "$size" --alignment 4
done < config/us/codex_batch_next.tsv
for function_name in \
        func_8007A7B4 \
        func_8007A818 \
        func_8007A8F0 \
        func_8007A9EC \
        func_8007AAA8 \
        func_8007AB64 \
        func_8007ADF0 \
        func_8007B0E4 \
        func_8007D39C \
        func_8007D7C4 \
        func_8007D884 \
        func_8007D998 \
        func_8007DACC \
        func_8007DD54 \
        func_8007DE3C \
        func_8007E024 \
        func_8007E118 \
        func_8007E8C8 \
        func_80079FF0 \
        func_8007B8EC \
        func_8007AF84 \
        func_8007D33C \
        func_80080818 \
        func_80082A90 \
        func_80083DF0 \
        func_80083EBC \
        func_8008518C \
        func_80085250 \
        func_80085DA8 \
        func_80085F7C \
        func_80086700 \
        func_8008865C \
        func_80088ABC \
        func_80089E84 \
        func_80089DBC \
        func_8008A350 \
        func_8008A5E4 \
        func_8008B6D0 \
        func_8008B788 \
        func_8008BEC4 \
        func_8008E620 \
        func_8008F3FC \
        func_800947C4 \
        func_8009660C \
        func_80096F48 \
        func_800976AC \
        func_80097844 \
        func_800979F4 \
        func_80097BC4 \
        func_80097CC8 \
        func_80098334 \
        func_8009836C \
        func_80098F24 \
        func_80098B58 \
        func_80098BF8 \
        func_80099028 \
        func_8009A650 \
        func_8009ACDC \
        func_8009AD7C \
        func_8009D4B0 \
        func_8009DAB0 \
        func_8009DFAC \
        func_8009E0E8 \
        func_8009E19C \
        func_8009E9C8 \
        func_8009EA70 \
        func_8009ED00 \
        func_8009F090 \
        func_8009F334 \
        func_8009F8A0 \
        func_800A1150 \
        func_800A1290 \
        func_800A140C \
        func_800A15F0 \
        func_800A18D0 \
        func_800A19DC \
        func_800A1B44 \
        func_800A2B9C \
        func_800A4924 \
        func_800A49D0 \
        func_800A60E0 \
        func_800A6B7C \
        func_800A8E84 \
        func_800A8F34 \
        func_800A9660 \
        func_800A96B8 \
        func_800ACF20 \
        func_800A97FC \
        func_800A9A44 \
        func_800A9A98 \
        func_800AA5D0 \
        func_800ABE6C \
        func_800ACFE0 \
        func_800B22F8 \
        func_800B9F44 \
        func_800BEE0C \
        func_800BF1A4 \
        func_800BF3EC \
        func_800BFCA4 \
        func_800BFDA4 \
        func_800BFE4C \
        func_800C04C8 \
        func_800C0564 \
        func_800C0ADC \
        func_800C0B78 \
        func_800C0C38 \
        func_800C13BC \
        func_800C180C \
        func_800C1E48; do
    case "$function_name" in
        func_80079FF0|func_8007A7B4|func_8007A818|func_8007A8F0|func_8007A9EC|func_8007AAA8|func_8007AB64|func_8007ADF0|func_8007AF84|func_8007B0E4|func_8007B8EC|func_8007D33C|func_8007D39C|func_8007D7C4|func_8007D884|func_8007D998|func_8007DACC|func_8007DD54|func_8007DE3C|func_8007E024|func_8007E118|func_8007E8C8|\
        func_80080818|func_80082A90|func_80083DF0|func_80083EBC|func_8008518C|func_80085250|func_80085DA8|func_80085F7C|func_80086700|func_8008865C|func_80088ABC|func_80089E84|\
        func_8008A350|func_8008B6D0|func_8008B788|func_8008BEC4|func_8008E620|func_8008F3FC|func_800947C4|func_8009660C|func_80096F48|func_800976AC|func_80097844|func_800979F4|func_80098F24|\
        func_80097CC8|func_80098334|func_8009836C|func_80098BF8|func_80099028|func_8009A650|func_8009ACDC|func_8009AD7C|func_8009D4B0|func_8009F090|func_8009F334|func_8009F8A0|func_800A1150|func_800A1290|\
        func_8009DAB0|func_8009DFAC|func_8009E0E8|func_8009E19C|func_800A140C|func_800A15F0|func_800A18D0|func_800A19DC|\
        func_800A1B44|func_800A2B9C|func_800A4924|func_800A49D0|func_800A60E0|func_800A6B7C|func_800A8E84|func_800A8F34|func_800A9660|func_800A96B8|func_800A9A98|func_800ACF20|func_800ACFE0|func_800AA5D0|func_800ABE6C|func_800B22F8|\
        func_800B9F44|func_800BEE0C|func_800C04C8|func_800C0564|\
        func_800C0ADC|func_800C0B78|func_800C0C38|func_800C13BC|\
        func_800C180C)
            unit="code/codex_batch_next2/${function_name}" ;;
        *) unit="code/${function_name}" ;;
    esac
    optimization=-O2
    if [ "$function_name" = func_80079FF0 ]; then
        optimization=-O0
    fi
    .toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
        "$optimization" -G0 -mips3 -mgp32 -mfp32 -Iinclude \
        -o "build/us/src/${unit}.raw.s" "src/${unit}.c"
    python3 tools/normalize_kmc_gcc_asm.py \
        "build/us/src/${unit}.raw.s" "build/us/src/${unit}.s"
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
        -o "build/us/src/${unit}.c.o" "build/us/src/${unit}.s"
    case "$function_name" in
        func_80079FF0) size=0xB0 ;;
        func_8007A7B4) size=0x64 ;;
        func_8007A818) size=0xD8 ;;
        func_8007A8F0) size=0xFC ;;
        func_8007A9EC) size=0xBC ;;
        func_8007AAA8) size=0xBC ;;
        func_8007AB64) size=0xD0 ;;
        func_8007ADF0) size=0x194 ;;
        func_8007AF84) size=0x98 ;;
        func_8007B0E4) size=0xAC ;;
        func_8007B8EC) size=0x400 ;;
        func_8007D33C) size=0x60 ;;
        func_8007D39C) size=0xCC ;;
        func_8007D7C4) size=0xC0 ;;
        func_8007D884) size=0x114 ;;
        func_8007D998) size=0xC4 ;;
        func_8007DACC) size=0xB8 ;;
        func_8007DD54) size=0xE8 ;;
        func_8007DE3C) size=0x164 ;;
        func_8007E024) size=0xF4 ;;
        func_8007E118) size=0x4C ;;
        func_8007E8C8) size=0xC0 ;;
        func_80080818) size=0x9C ;;
        func_80082A90) size=0xAC ;;
        func_80083DF0) size=0xCC ;;
        func_80083EBC) size=0xB8 ;;
        func_8008518C) size=0xC4 ;;
        func_80085250) size=0xA8 ;;
        func_80085DA8) size=0xA4 ;;
        func_80085F7C) size=0xB8 ;;
        func_80086700) size=0xC4 ;;
        func_8008865C) size=0xC4 ;;
        func_80088ABC) size=0x5C ;;
        func_80089E84) size=0xB4 ;;
        func_80089DBC) size=0x64 ;;
        func_8008A350) size=0x50 ;;
        func_8008A5E4) size=0x48 ;;
        func_8008B6D0) size=0xB8 ;;
        func_8008B788) size=0xA4 ;;
        func_8008BEC4) size=0x98 ;;
        func_8008E620) size=0x9C ;;
        func_8008F3FC) size=0xB0 ;;
        func_800947C4) size=0xBC ;;
        func_8009660C) size=0xC8 ;;
        func_80096F48) size=0xC4 ;;
        func_800976AC) size=0xE8 ;;
        func_80097844) size=0xC8 ;;
        func_800979F4) size=0x78 ;;
        func_80097BC4) size=0x58 ;;
        func_80097CC8) size=0x4C ;;
        func_80098334) size=0x38 ;;
        func_8009836C) size=0xC4 ;;
        func_80098F24) size=0x98 ;;
        func_80098B58) size=0x70 ;;
        func_80098BF8) size=0xD0 ;;
        func_80099028) size=0x58 ;;
        func_8009A650) size=0xA0 ;;
        func_8009ACDC) size=0xA0 ;;
        func_8009AD7C) size=0xBC ;;
        func_800A4924) size=0xAC ;;
        func_800A49D0) size=0xAC ;;
        func_800A60E0) size=0xBC ;;
        func_8009D4B0) size=0x60 ;;
        func_8009DAB0) size=0x5C ;;
        func_8009DFAC) size=0x98 ;;
        func_8009E0E8) size=0xB4 ;;
        func_8009E19C) size=0xB4 ;;
        func_8009E9C8) size=0xA8 ;;
        func_8009EA70) size=0xF8 ;;
        func_8009ED00) size=0x9C ;;
        func_8009F090) size=0x164 ;;
        func_8009F334) size=0x110 ;;
        func_8009F8A0) size=0x42C ;;
        func_800A1150) size=0x130 ;;
        func_800A1290) size=0xBC ;;
        func_800A140C) size=0x1E4 ;;
        func_800A15F0) size=0x108 ;;
        func_800A18D0) size=0x10C ;;
        func_800A19DC) size=0x4C ;;
        func_800A1B44) size=0x9C ;;
        func_800A2B9C) size=0xD0 ;;
        func_800A6B7C) size=0xA4 ;;
        func_800A8E84) size=0xB0 ;;
        func_800A8F34) size=0x120 ;;
        func_800A9660) size=0x58 ;;
        func_800A96B8) size=0x94 ;;
        func_800A9A98) size=0xCC ;;
        func_800ACF20) size=0xC0 ;;
        func_800ACFE0) size=0xA8 ;;
        func_800ABE6C) size=0xCC ;;
        func_800A97FC) size=0xBC ;;
        func_800A9A44) size=0x54 ;;
        func_800AA5D0) size=0x94 ;;
        func_800B22F8) size=0x6C ;;
        func_800B9F44) size=0x90 ;;
        func_800BEE0C) size=0xA8 ;;
        func_800BF1A4) size=0x60 ;;
        func_800BF3EC) size=0x98 ;;
        func_800BFCA4) size=0x9C ;;
        func_800BFDA4) size=0xA8 ;;
        func_800BFE4C) size=0x48 ;;
        func_800C04C8) size=0x9C ;;
        func_800C0564) size=0xA4 ;;
        func_800C0ADC) size=0x9C ;;
        func_800C0B78) size=0xA0 ;;
        func_800C0C38) size=0xC8 ;;
        func_800C13BC) size=0x64 ;;
        func_800C180C) size=0x12C ;;
        func_800C1E48) size=0xC0 ;;
    esac
    python3 tools/trim_elf32_section.py \
        "build/us/src/${unit}.c.o" .text "$size" --alignment 4
done
python3 tools/trim_elf32_section.py \
    build/us/src/code/codex_batch_next2/func_800A1290.c.o .rodata 0x14 --alignment 4
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
    build/us/src/code/results_assets_reload.c.o .text 0x288 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/game_state_clear.c.o .text 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_ctl_index.c.o .text 0x44 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_ctl_alloc.c.o .text 0x80 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/audio_listener_update.c.o .text 0xd4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_buffers_alloc.c.o .text 0xe0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_attach_effect_create.c.o .text 0xac --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_attach_effect_matrix.c.o .text 0x6c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_attach_effect_matrix_cb.c.o .text 0x74 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/actor_collision_free.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_message_handlers.c.o .text 0xe4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/single_player_check.c.o .text 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/trigger_zone_query.c.o .text 0x114 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/minimap_layer_build.c.o .text 0x114 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_texture_find.c.o .text 0xc0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_bounds_center.c.o .text 0x118 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/debris_piece_draw.c.o .text 0x1b0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/debris_burst_spawn.c.o .text 0x16c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/debris_burst_spawn_tinted.c.o .text 0x174 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/debris_scatter_spawn.c.o .text 0x15c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_model_draw.c.o .text 0x194 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_model_create.c.o .text 0x28c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_model_matrix.c.o .text 0xb8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_model_matrix_cb.c.o .text 0xb8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_trail_draw.c.o .text 0x164 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_launch_delayed.c.o .text 0xe8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_launch_delayed_tick.c.o .text 0xcc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_mesh_partition.c.o .text 0x110 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/structure_create.c.o .text 0x354 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawner_target_query.c.o .text 0x50 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/unit_spawner_create.c.o .text 0x190 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/unit_spawner_update.c.o .text 0x604 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_multiplayer_filter.c.o .text 0x7c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/level_prop_points_find.c.o .text 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/foliage_prop_create.c.o .text 0x198 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/foliage_prop_handlers.c.o .text 0xcc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_destroy.c.o .text 0x188 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_draw_create.c.o .text 0x380 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_message_handler.c.o .text 0x168 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_line_of_sight.c.o .text 0x98 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/wreck_timer_duration.c.o .text 0x5c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/flag_capture_attempt.c.o .text 0x27c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/actor_kind16_link_clear.c.o .text 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/sound_emitter_delayed_spawn.c.o .text 0x84 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/ambient_sound_stop_first.c.o .text 0x9c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/bridge_create_handlers.c.o .text 0x1f4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/lightning_arc_update.c.o .text 0x304 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/crate_model_randomize.c.o .text 0x50 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/crate_actor_attach.c.o .text 0x168 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/pending_list_flush.c.o .text 0x58 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/powerup_flag_players.c.o .text 0xa8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/powerup_count_refresh.c.o .text 0xe8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/pickup_actor_count.c.o .text 0x7c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/game_mode_has_pickups.c.o .text 0x44 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slist_remove_count.c.o .text 0x60 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_select_init.c.o .text 0x134 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_entry_query.c.o .text 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_time_bonus.c.o .text 0x98 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_event_forward.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/building_target_query.c.o .text 0x5c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/building_destroy_on_hit.c.o .text 0xa8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/building_damage_apply.c.o .text 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/building_create.c.o .text 0x228 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/building_anim_frame_tick.c.o .text 0x5c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/building_destroy.c.o .text 0x39c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/impact_flash_spawn.c.o .text 0x9c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/impact_flash_expire.c.o .text 0x164 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_projectile_launch.c.o .text 0xf8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_counter_release.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/splash_damage_falloff.c.o .text 0x5c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_spawn_typed.c.o .text 0x158 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/shockwave_ring_spawn.c.o .text 0x9c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/shockwave_ring_draw.c.o .text 0x170 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/destructible_prop_create.c.o .text 0x1d4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/destructible_anim_frame_tick.c.o .text 0x58 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_target_query.c.o .text 0x44 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/destructible_prop_on_hit.c.o .text 0x48 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/destructible_prop_on_hit_alt.c.o .text 0x48 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_destroy_slot_release.c.o .text 0x1cc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_target_query.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_debris_burst_on_hit.c.o .text 0xd0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_debris_burst_on_hit_alt.c.o .text 0xd0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/barrier_break_open.c.o .text 0x2b4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_emitter_tail.c.o .text 0x6c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_emitter_create.c.o .text 0x1a0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_pool_init.c.o .text 0xb4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_emitter_update.c.o .text 0x18c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_free_list.c.o .text 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_node_list.c.o .text 0xf0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/anim_list_register_global.c.o .text 0x3c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hazard_actor_spawn.c.o .text 0x198 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hazard_actor_spawn_simple.c.o .text 0xcc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mine_message_handlers.c.o .text 0x324 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/powerup_pad_create.c.o .text 0x1f4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/powerup_pad_draw.c.o .text 0x170 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/barrel_handlers.c.o .text 0x240 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/falling_crate_update.c.o .text 0x1f0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/decal_spawn_draw.c.o .text 0x2bc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/color_interpolate.c.o .text 0x184 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mesh_vertex_transform.c.o .text 0x274 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/tracer_spawn.c.o .text 0xc8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_table_reset.c.o .text 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/generator_damage_apply.c.o .text 0xf8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/generator_message_damage.c.o .text 0x1f0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/generator_hit_query.c.o .text 0x44 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/generator_splash_damage.c.o .text 0x200 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/generator_damage_tick.c.o .text 0x174 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/generator_dispatch_bonus.c.o .text 0x190 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_expire_100.c.o .text 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/wreck_debris_spawn.c.o .text 0x1bc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/wreck_debris_update.c.o .text 0xc4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/anim_colors_set.c.o .text 0xac --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/anim_list_create.c.o .text 0x34 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/anim_list_reset.c.o .text 0x10 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/anim_list_register.c.o .text 0xcc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spark_spawn.c.o .text 0xe4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/smoke_puff_spawn.c.o .text 0x80 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/flicker_prop_create.c.o .text 0xf4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/flicker_prop_draw.c.o .text 0xf0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/flicker_prop_message.c.o .text 0x50 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/flicker_prop_message_cb.c.o .text 0x54 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/static_prop_create.c.o .text 0xac --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/static_prop_expire.c.o .text 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/static_prop_draw.c.o .text 0x118 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_variant_get.c.o .text 0x120 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_cache_globals.c.o .text 0x5c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/artillery_emplacement_update.c.o .text 0x218 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/artillery_emplacement_disable.c.o .text 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/artillery_emplacement_create.c.o .text 0x6c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/artillery_projectile_create.c.o .text 0xcc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_status_flags.c.o .text 0x34 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_ready_flags_c.c.o .text 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_ready_flags_b.c.o .text 0x3c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_ready_flags_a.c.o .text 0x3c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_option_codes.c.o .text 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_return.c.o .text 0x80 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_continue.c.o .text 0xcc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_screen_setup.c.o .text 0x18c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_time_compare.c.o .text 0x488 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_time_format.c.o .text 0x418 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_record_write.c.o .text 0x110 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controls_button_index_map.c.o .text 0x150 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_assets_load.c.o .text 0xd4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_widget_flag_set.c.o .text 0x4c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_widget_reset.c.o .text 0x118 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_widget_clear.c.o .text 0xd8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_widget_values.c.o .text 0x134 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_widget.c.o .text 0x1dc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_flag_queries.c.o .text 0x7c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_popup_show.c.o .text 0x3c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_timer_queries.c.o .text 0xb0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_reset.c.o .text 0x114 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_display_value_set.c.o .text 0x10 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_pair_set.c.o .text 0x54 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_message.c.o .text 0x8c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_panel.c.o .text 0x11c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_slot_clear.c.o .text 0x24 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_event_push.c.o .text 0x118 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_state_reset.c.o .text 0x80 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_object_timestamps.c.o .text 0x104 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_result_setup.c.o .text 0x13c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_hud_init.c.o .text 0x504 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_assets_load.c.o .text 0x448 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_page_close.c.o .text 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_page_open.c.o .text 0x88 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_page_show.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_page_exit_request.c.o .text 0x44 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_page_focus.c.o .text 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_slot_icons_update.c.o .text 0xba4 --alignment 4
# Trim KMC tail padding so .rodata is exactly the original range (unit_rodata.tsv).
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_slot_icons_update.c.o .rodata 0xe0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_message_damage_guarded.c.o .text 0x6c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_message_damage_guarded.c.o .rodata 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mesh_rings_draw.c.o .text 0xdfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mesh_rings_draw.c.o .rodata 0x4c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/artillery_target_track.c.o .text 0x380 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_in_draw_range.c.o .text 0x154 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/static_in_radius.c.o .text 0x48 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spark_burst_update.c.o .text 0x124 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/anim_alpha_fade_draw.c.o .text 0x12c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/generator_spawn.c.o .text 0x178 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_beam_draw.c.o .text 0x2dc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_burn_update.c.o .text 0x3c0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_burn_update.c.o .rodata 0x10 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_targets_damage.c.o .text 0x234 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_targets_query.c.o .text 0xd8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_prop_spawn.c.o .text 0x1d8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/tracer_trail_update.c.o .text 0x1ac --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mesh_wave_update.c.o .text 0x270 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/barrel_explode.c.o .text 0x1a4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/barrel_explode.c.o .rodata 0x4c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/powerup_timer_update.c.o .text 0x2e4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/powerup_timer_update.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mine_trigger_update.c.o .text 0x238 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mine_trigger_update.c.o .rodata 0xc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hazard_message_handler.c.o .text 0x270 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hazard_message_handler.c.o .rodata 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_state_query.c.o .text 0x164 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hazard_model_draw.c.o .text 0x134 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hazard_state_update.c.o .text 0x2bc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hazard_state_update.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/anim_keyframes_apply.c.o .text 0x150 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/anim_keyframes_apply.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_owner_message.c.o .text 0xa8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_owner_message.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_system_create.c.o .text 0x300 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_system_create.c.o .rodata 0x4c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_hit_spawn.c.o .text 0x3c8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_hit_spawn.c.o .rodata 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_smoke_update.c.o .text 0x2d8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_smoke_update.c.o .rodata 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_emitter_free.c.o .text 0x1a0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/particle_emitter_free.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_height_update.c.o .text 0x21c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_spawn_pickup.c.o .text 0xc8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_spawn_child.c.o .text 0x170 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/shockwave_expand.c.o .text 0x104 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_shell_draw.c.o .text 0x154 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_objectives_update.c.o .text 0x284 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/impact_debris_spray.c.o .text 0x7c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_marker_draw.c.o .text 0x1e0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_marker_draw.c.o .rodata 0x60 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_targets_query.c.o .text 0xd8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_props_spawn.c.o .text 0x2cc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_flag_set.c.o .text 0x244 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/mission_code_parse.c.o .text 0x268 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/game_team_compare.c.o .text 0x64 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/pickup_find_by_id.c.o .text 0xec --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/pending_spawns_flush.c.o .text 0x174 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/pending_spawns_flush.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/crate_contents_spawn.c.o .text 0x274 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/crate_owner_check.c.o .text 0x248 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/crate_owner_check.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/bridge_piece_spawn.c.o .text 0x1a8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/bridge_piece_spawn.c.o .rodata 0xc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/bridge_segments_init.c.o .text 0x2e8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/bridge_segments_init.c.o .rodata 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/sound_emitter_draw.c.o .text 0x444 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/sound_emitter_draw.c.o .rodata 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/actor_wreck_spawn_at.c.o .text 0x228 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/actor_wreck_spawn_at.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/flag_owner_score.c.o .text 0x21c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/flag_owner_capture.c.o .text 0x2a4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/object_damage.c.o .text 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/wreck_ctl.c.o .text 0x190 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_spawn_at.c.o .text 0x288 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/wreck_spawn.c.o .text 0x148 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/target_search.c.o .text 0x194 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_entry_lookup.c.o .text 0x48 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_angle_classify.c.o .text 0x11c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_entity_attach.c.o .text 0xe4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_corners_build.c.o .text 0x26c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawner_message_handler.c.o .text 0x148 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawner_owner_score.c.o .text 0xa0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawner_state_update.c.o .text 0x21c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/structure_owner_award.c.o .text 0x78 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/structure_model_draw.c.o .text 0x3f8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/structure_model_draw.c.o .rodata 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/structure_flicker_roll.c.o .text 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_fire.c.o .text 0x770 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_fire.c.o .rodata 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_life_tick.c.o .text 0x54 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_bounce.c.o .text 0x57c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/projectile_bounce.c.o .rodata 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_bounds_compute.c.o .text 0xe8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/minimap_blips_spawn.c.o .text 0x188 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/minimap_blips_spawn.c.o .rodata 0x24 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/trigger_particles_update.c.o .text 0x3c0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/trigger_particles_update.c.o .rodata 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/trigger_particles_spawn.c.o .text 0x14c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/trigger_particles_spawn.c.o .rodata 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/single_spawn_timer_update.c.o .text 0x278 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/single_spawn_timer_update.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_spawn_random.c.o .text 0x2c8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_spawn_random.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_model_draw.c.o .text 0x1e0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_random_slot.c.o .text 0x88 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_weighted_pick.c.o .text 0xb8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_weights_total.c.o .text 0x50 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_state_messages.c.o .text 0x190 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/turret_state_messages.c.o .rodata 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/actor_message_handler.c.o .text 0x198 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/actor_message_handler.c.o .rodata 0xc8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/actor_timer_update.c.o .text 0x158 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/actor_model_draw.c.o .text 0x2f8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_node_spawn.c.o .text 0x270 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_menu_draw.c.o .text 0x6bc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_cursor_anim.c.o .text 0xf4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_menu_update.c.o .text 0x658 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_code_parse.c.o .text 0x2b4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_code_table.c.o .text 0x1a0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_draw.c.o .text 0x1f0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/script_particle_spawn.c.o .text 0x1ac --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/script_particle_spawn.c.o .rodata 0xc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_draw_list.c.o .text 0x580 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_draw_list.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/script_event_finish.c.o .text 0x134 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/script_event_finish.c.o .rodata 0xd8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_command_dispatch.c.o .text 0x73c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_command_dispatch.c.o .rodata 0xc8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_spawn_events_update.c.o .text 0x3d0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_spawn_events_update.c.o .rodata 0xc8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_event_script_tick.c.o .text 0x378 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_event_script_tick.c.o .rodata 0xec --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_spawn_script_tick.c.o .text 0x698 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_spawn_script_tick.c.o .rodata 0xf0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_script_tick.c.o .text 0x564 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_script_tick.c.o .rodata 0xc8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_object_command.c.o .text 0x87c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_object_command.c.o .rodata 0x18c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_counters.c.o .text 0x7c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_nearest_type31.c.o .text 0xf4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_nearest_type28.c.o .text 0xf4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_nearest_type1.c.o .text 0xf4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_element_alloc.c.o .text 0x94 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_element_unlink.c.o .text 0x234 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_columns_draw.c.o .text 0x140 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_columns_draw.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_time_tick.c.o .text 0x2a8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_time_tick.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_bonus_finish.c.o .text 0xa8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_bonus_finish.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_bonus_tick.c.o .text 0xc8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_bonus_tick.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_score_finish.c.o .text 0xa8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_score_finish.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_score_tick.c.o .text 0xc8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_score_tick.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_deaths_tick.c.o .text 0xb4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_deaths_tick.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_kills_tick.c.o .text 0xd4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_kills_tick.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_slot_find.c.o .text 0x6b8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_slot_find.c.o .rodata 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_queue_push.c.o .text 0x458 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_queue_push.c.o .rodata 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_menu_input.c.o .text 0x4f8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_menu_input.c.o .rodata 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_slots_scan.c.o .text 0x27c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_lap_times_clamp.c.o .text 0x134 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_icons_draw.c.o .text 0x368 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_icons_draw.c.o .rodata 0x10 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_timer_update.c.o .text 0x2f8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_marker_project.c.o .text 0x3f8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_marker_project.c.o .rodata 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_map_draw.c.o .text 0x628 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_map_draw.c.o .rodata 0x34 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_state_init.c.o .text 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_volume_adjust.c.o .text 0x394 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_volume_adjust.c.o .rodata 0xC --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_controls_page.c.o .text 0xF78 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_controls_page.c.o .rodata 0x1D0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_page_next.c.o .text 0x198 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_save_slots.c.o .text 0x4B8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_start_check.c.o .text 0x170 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_start_check.c.o .rodata 0x34 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_frame_border_draw.c.o .text 0x450 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_frame_border_draw.c.o .rodata 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_value_to_byte.c.o .text 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_value_to_byte.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_panel_select.c.o .text 0x2c4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_panel_select.c.o .rodata 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_panel_refresh.c.o .text 0x2cc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_panel_refresh.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_panel_layout.c.o .text 0x234 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_panel_layout.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_mode_cycle.c.o .text 0x244 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_mode_cycle.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_marker_update.c.o .text 0x240 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_marker_update.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_player_slots_cycle.c.o .text 0x1e0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_message_damage.c.o .text 0x60 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_message_damage.c.o .rodata 0x100 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_segments_draw.c.o .text 0x1f8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_segments_draw.c.o .rodata 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_segment_append.c.o .text 0x178 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/effect_segment_append.c.o .rodata 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_message_shatter.c.o .text 0x19c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/prop_message_shatter.c.o .rodata 0xbc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_message_damage_destroy.c.o .text 0x98 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_message_damage_destroy.c.o .rodata 0x100 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_shielded_dispatch.c.o .text 0x218 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_shielded_dispatch.c.o .rodata 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_turret_dispatch.c.o .text 0x1b0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_turret_dispatch.c.o .rodata 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_message_damage_u8.c.o .text 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_message_damage_u8.c.o .rodata 0x100 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_mode_objective_message.c.o .text 0x1a0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_mode_objective_message.c.o .rodata 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_pickup_create.c.o .text 0x15c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/model_pickup_create.c.o .rodata 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_value_get.c.o .text 0x15c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_value_get.c.o .rodata 0x6c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_description_get.c.o .text 0x15c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_description_get.c.o .rodata 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_subtitle_get.c.o .text 0xa0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_subtitle_get.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_title_get.c.o .text 0x15c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_title_get.c.o .rodata 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_name_get.c.o .text 0x1ac --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_mode_name_get.c.o .rodata 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/wreck_message_hit.c.o .text 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/wreck_message_hit.c.o .rodata 0xbc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_dispatch.c.o .text 0x158 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_dispatch.c.o .rodata 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_message_hit.c.o .text 0xa8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/player_message_hit.c.o .rodata 0x100 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_condition_check.c.o .text 0x1a0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/slot_condition_check.c.o .rodata 0xb8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800D12B0_script_command_decode.c.o .text 0x9e0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800D12B0_script_command_decode.c.o .rodata 0xf8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800D3C64_seq_point_track.c.o .text 0x3e8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800D3C64_seq_point_track.c.o .rodata 0xc0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800D5E2C_script_particle_update.c.o .text 0x6dc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800D9D50_debris_spawn.c.o .text 0x488 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800D9D50_debris_spawn.c.o .rodata 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DA1D8_debris_piece_update.c.o .text 0x168 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DA7D0_debris_group_spawn.c.o .text 0x1b4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DAAE0_projectile_spawn.c.o .text 0x154 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DB1B0_projectile_segment_spawn.c.o .text 0x338 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DB1B0_projectile_segment_spawn.c.o .rodata 0x5c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DB6F0_track_angle_unit.c.o .text 0xb24 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DB6F0_track_angle_unit.c.o .rodata 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DD75C_structure_hit_dispatch.c.o .text 0xd0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DD82C_structure_debris_spawn.c.o .text 0x6d8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DD82C_structure_debris_spawn.c.o .rodata 0x70 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DE374_structure_message.c.o .text 0x168 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800DE374_structure_message.c.o .rodata 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/8008C5D8_tank_fire_weapon.c.o .text 0x2048 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/8008C5D8_tank_fire_weapon.c.o .rodata 0x16c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E16D8_foliage_prop_destroy.c.o .text 0x200 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E16D8_foliage_prop_destroy.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E1BB0_turret_sweep.c.o .text 0x468 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E1BB0_turret_sweep.c.o .rodata 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E2520_turret_message_handlers.c.o .text 0x188 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E2F9C_wreck_update.c.o .text 0x468 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E2F9C_wreck_update.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E48A8_flag_position_query.c.o .text 0x38 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E66A8_crate_particle_attach.c.o .text 0x214 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E6B04_crate_unit.c.o .text 0x638 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E6B04_crate_unit.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E73B0_crate_part_release.c.o .text 0x1ec --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E7768_crate_burst_spawn.c.o .text 0x200 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E7768_crate_burst_spawn.c.o .rodata 0xc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E7A10_powerup_flag_messages.c.o .text 0x3e8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/800E7A10_powerup_flag_messages.c.o .rodata 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_events_update.c.o .text 0x1f4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_events_update.c.o .rodata 0xd8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_level_header_load.c.o .text 0x1c8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_level_header_load.c.o .rodata 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_script_load.c.o .text 0x42c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_script_load.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_event_start.c.o .text 0x1a8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_event_start.c.o .rodata 0xd8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_event_spawn_tick.c.o .text 0x710 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/seq_event_spawn_tick.c.o .rodata 0x240 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_effect_by_type.c.o .text 0x160 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_effect_by_type.c.o .rodata 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controls_bindings_load.c.o .text 0x50c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controls_bindings_load.c.o .rodata 0x9c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controls_bindings_save.c.o .text 0x600 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controls_bindings_save.c.o .rodata 0x28 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_slot_set_digit.c.o .text 0x180 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_slot_set_digit.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_set_mode_icon.c.o .text 0x17c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_set_mode_icon.c.o .rodata 0x3c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_mode_icon_get.c.o .text 0xdc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_mode_icon_get.c.o .rodata 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_set_layout.c.o .text 0x354 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_set_layout.c.o .rodata 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_type_name_lookup.c.o .text 0x120 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_type_name_lookup.c.o .rodata 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_set_vehicle_label.c.o .text 0x110 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_player_set_vehicle_label.c.o .rodata 0x30 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_entry_set_label.c.o .text 0x90 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_entry_set_label.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_menu_entries_apply_settings.c.o .text 0x1cc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_menu_entries_apply_settings.c.o .rodata 0x60 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/game_mode8_toggled.c.o .text 0x48 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_page_focus_cycle.c.o .text 0x12c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_page_select.c.o .text 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_alt_exit_request.c.o .text 0x2c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_close.c.o .text 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_open.c.o .text 0x88 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_focus_cycle.c.o .text 0x248 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_refresh.c.o .text 0x68 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_exit_request.c.o .text 0x44 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_row_defaults.c.o .text 0x44 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_row_preset.c.o .text 0x50 --alignment 4
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
    build/us/src/code/hud_tree_update.c.o .text 0x1e8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_tree_remove.c.o .text 0x130 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_mode_query.c.o .text 0xbc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_hud_layout.c.o .text 0x134 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controller_menu_state.c.o .text 0x2e0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_assets_init.c.o .text 0x550 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_menu_callbacks.c.o .text 0x26c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_panel_setup.c.o .text 0x204 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_split_setup.c.o .text 0x72C --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/spawn_color_lookup.c.o .text 0x108 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_key_nav.c.o .text 0x108 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_slots_layout.c.o .text 0x118 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controls_preset.c.o .text 0x2a4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/controls_config.c.o .text 0x510 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_binding_select.c.o .text 0x1f8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_tree_draw.c.o .text 0x314 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_tree_draw.c.o .rodata 0x8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_frame.c.o .text 0x498 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_frame.c.o .rodata 0x10 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_mode_table.c.o .text 0xac --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_mode_table.c.o .rodata 0x20 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_mode_cycle_full.c.o .text 0x1f8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_mode_cycle_full.c.o .rodata 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_exit_dispatch.c.o .text 0x184 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_exit_dispatch.c.o .rodata 0x14 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_menu_open.c.o .text 0xf4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_menu_open.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_option_select.c.o .text 0x2c4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_option_select.c.o .rodata 0x18 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_select_full.c.o .text 0x77c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_list_select_full.c.o .rodata 0xc0 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_player_page.c.o .text 0x270 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/hud_player_page.c.o .rodata 0x40 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_popup_setup.c.o .text 0xfc --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/race_popup_setup.c.o .rodata 0x4 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_bar_step.c.o .text 0x154 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_bar_step.c.o .rodata 0x10 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_rank_label.c.o .text 0xb8 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_time_text.c.o .text 0x190 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/results_time_text.c.o .rodata 0x1c --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/cheat_codes.c.o .text 0x568 --alignment 4
python3 tools/trim_elf32_section.py \
    build/us/src/code/cheat_codes.c.o .rodata 0x84 --alignment 4
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
    -o build/us/src/libultra/os_pi_manager.c.o src/libultra/os_pi_manager.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_dev_mgr.c.o src/libultra/os_dev_mgr.c
python3 tools/trim_elf32_section.py \
    build/us/src/libultra/os_dev_mgr.c.o .text 0x490 --alignment 16
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/al_resample.c.o src/libultra/al_resample.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/al_reverb.c.o src/libultra/al_reverb.c
.toolchain/ido5.3/cc -c -O3 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/al_synthesizer.c.o src/libultra/al_synthesizer.c
.toolchain/ido5.3/cc -c -O3 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/al_syn_alloc_fx.c.o src/libultra/al_syn_alloc_fx.c
# Retail kept the final multiply before `jr ra` and aligned the following
# function to eight bytes. Apply that verified assembler layout exactly once.
python3 tools/insert_elf32_section_bytes.py \
    build/us/src/libultra/al_reverb.c.o .text _filterBuffer 4 \
    --unschedule-return
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_ai_set_frequency.c.o src/libultra/os_ai_set_frequency.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_ai_set_next_buffer.c.o src/libultra/os_ai_set_next_buffer.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_cart_rom_init.c.o src/libultra/os_cart_rom_init.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_cont_query.c.o src/libultra/os_cont_query.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_cont_read_data.c.o src/libultra/os_cont_read_data.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_controller.c.o src/libultra/os_controller.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_epi_raw_read_io.c.o src/libultra/os_epi_raw_read_io.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_epi_raw_write_io.c.o src/libultra/os_epi_raw_write_io.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pi_raw_read_io.c.o src/libultra/os_pi_raw_read_io.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pi_raw_start_dma.c.o src/libultra/os_pi_raw_start_dma.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_recv_mesg.c.o src/libultra/os_recv_mesg.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_send_mesg.c.o src/libultra/os_send_mesg.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_set_event_mesg.c.o src/libultra/os_set_event_mesg.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_set_thread_pri.c.o src/libultra/os_set_thread_pri.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_set_timer.c.o src/libultra/os_set_timer.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_sp_task_yielded.c.o src/libultra/os_sp_task_yielded.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_dp_set_next_buffer.c.o src/libultra/os_dp_set_next_buffer.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_si_raw_start_dma.c.o src/libultra/os_si_raw_start_dma.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_sp_raw_start_dma.c.o src/libultra/os_sp_raw_start_dma.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_dequeue_thread.c.o src/libultra/os_dequeue_thread.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_black.c.o src/libultra/os_vi_black.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_init.c.o src/libultra/os_vi_init.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_get_current_framebuffer.c.o src/libultra/os_vi_get_current_framebuffer.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_get_next_framebuffer.c.o src/libultra/os_vi_get_next_framebuffer.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_manager.c.o src/libultra/os_vi_manager.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_timer.c.o src/libultra/os_timer.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_set_event.c.o src/libultra/os_vi_set_event.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_set_mode.c.o src/libultra/os_vi_set_mode.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_set_special_features.c.o src/libultra/os_vi_set_special_features.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_swap_buffer.c.o src/libultra/os_vi_swap_buffer.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_vi_swap_context.c.o src/libultra/os_vi_swap_context.c
.toolchain/ido5.3/cc -c -Wab,-r4300_mul -G 0 -nostdinc -Xcpluscomm \
    -fullwarn -woff 516,649,838,712 -mips2 -o32 -D_MIPS_SZLONG=32 \
    -DBUILD_VERSION=VERSION_I -DBUILD_VERSION_STRING=\"2.0I\" -non_shared \
    -DNDEBUG -D_FINALROM -O3 -Isrc/libultra/ido -Isrc/libultra/ido/PR \
    -Isrc/libultra -o build/us/src/libultra/xprintf.c.o src/libultra/xprintf.c
.toolchain/ido5.3/cc -c -Wab,-r4300_mul -G 0 -nostdinc -Xcpluscomm \
    -fullwarn -woff 516,649,838,712 -mips2 -o32 -D_MIPS_SZLONG=32 \
    -DBUILD_VERSION=VERSION_I -DBUILD_VERSION_STRING=\"2.0I\" -non_shared \
    -DNDEBUG -D_FINALROM -O3 -Isrc/libultra/ido -Isrc/libultra/ido/PR \
    -Isrc/libultra -o build/us/src/libultra/xldtob.c.o src/libultra/xldtob.c
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/ldiv.c.o src/libultra/ldiv.c
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/litob.c.o src/libultra/litob.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_yield_thread.c.o src/libultra/os_yield_thread.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_virtual_to_physical.c.o src/libultra/os_virtual_to_physical.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_reset_global_int_mask.c.o src/libultra/os_reset_global_int_mask.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_set_global_int_mask.c.o src/libultra/os_set_global_int_mask.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_si_access_queue.c.o src/libultra/os_si_access_queue.c
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_save_param.c.o src/libultra/al_save_param.c
python3 tools/trim_elf32_section.py \
    build/us/src/libultra/al_save_param.c.o .text 0x34 --alignment 4
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_save_pull.c.o src/libultra/al_save_pull.c
python3 tools/trim_elf32_section.py \
    build/us/src/libultra/al_save_pull.c.o .text 0x8C --alignment 4
.toolchain/ido5.3/cc -c -O2 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/gu_normalize.c.o src/libultra/gu_normalize.c
.toolchain/ido5.3/cc -c -O3 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/gu_rotate.c.o src/libultra/gu_rotate.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_sp_task.c.o src/libultra/os_sp_task.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_start_thread.c.o src/libultra/os_start_thread.c
.toolchain/ido5.3/cc -c -O2 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/sinf.c.o src/libultra/sinf.c
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
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_create_thread.c.o src/libultra/os_create_thread.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_epi_start_dma.c.o src/libultra/os_epi_start_dma.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_destroy_thread.c.o src/libultra/os_destroy_thread.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_get_time.c.o src/libultra/os_get_time.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_jam_mesg.c.o src/libultra/os_jam_mesg.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_epi_raw_start_dma.c.o src/libultra/os_epi_raw_start_dma.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_leo_disk_init.c.o src/libultra/os_leo_disk_init.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_cont_crc.c.o src/libultra/os_cont_crc.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pi_start_dma.c.o src/libultra/os_pi_start_dma.c
.toolchain/ido5.3/cc -c -O2 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/gu_mtx.c.o src/libultra/gu_mtx.c
.toolchain/ido5.3/cc -c -O2 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/gu_perspective.c.o src/libultra/gu_perspective.c
.toolchain/ido5.3/cc -c -O2 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/cosf.c.o src/libultra/cosf.c
.toolchain/ido5.3/cc -c -O3 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/gu_look_at.c.o src/libultra/gu_look_at.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_cont_ram_write.c.o src/libultra/os_cont_ram_write.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_cont_ram_read.c.o src/libultra/os_cont_ram_read.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_initialize.c.o src/libultra/os_initialize.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_motor.c.o src/libultra/os_motor.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_delete_file.c.o src/libultra/os_pfs_delete_file.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_file_state.c.o src/libultra/os_pfs_file_state.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_free_blocks.c.o src/libultra/os_pfs_free_blocks.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_get_status.c.o src/libultra/os_pfs_get_status.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_init_pak.c.o src/libultra/os_pfs_init_pak.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_is_plug.c.o src/libultra/os_pfs_is_plug.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_checker.c.o src/libultra/os_pfs_checker.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_read_write_file.c.o src/libultra/os_pfs_read_write_file.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_get_id_copy.c.o src/libultra/os_pfs_get_id_copy.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude -Isrc/libultra \
    -o build/us/src/libultra/os_pfs_find_file.c.o src/libultra/os_pfs_find_file.c
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
.toolchain/ido5.3/cc -c -O1 -mips3 -32 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/ll.c.o src/libultra/ll.c
python3 tools/set_elf32_mips_o32.py build/us/src/libultra/ll.c.o
.toolchain/ido5.3/cc -c -Wab,-r4300_mul -G 0 -nostdinc -Xcpluscomm \
    -fullwarn -woff 516,649,838,712 -mips2 -o32 -D_MIPS_SZLONG=32 \
    -DF3DEX_GBI -DBUILD_VERSION=VERSION_I \
    -DBUILD_VERSION_STRING=\"2.0I\" -non_shared -DNDEBUG -D_FINALROM -O3 \
    -Isrc/libultra -o build/us/src/libultra/sched.c.o src/libultra/sched.c
python3 tools/trim_elf32_section.py \
    build/us/src/libultra/sched.c.o .text 0x940 --alignment 4
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
    -Isrc/libultra -o build/us/src/libultra/al_drvrnew.c.o src/libultra/al_drvrnew.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -Isrc/libultra -o build/us/src/libultra/al_aux_bus.c.o src/libultra/al_aux_bus.c
.toolchain/ido5.3/cc -c -O3 -mips2 -Wab,-r4300_mul -non_shared -G 0 -Iinclude \
    -Isrc/libultra -o build/us/src/libultra/al_env.c.o src/libultra/al_env.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -Isrc/libultra -o build/us/src/libultra/al_load.c.o src/libultra/al_load.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -Isrc/libultra -o build/us/src/libultra/al_main_bus.c.o src/libultra/al_main_bus.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -Isrc/libultra -o build/us/src/libultra/os_pfs_allocate_file.c.o src/libultra/os_pfs_allocate_file.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -Isrc/libultra -o build/us/src/libultra/os_cont_pfs.c.o src/libultra/os_cont_pfs.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Xcpluscomm -Iinclude \
    -Isrc/libultra -o build/us/src/libultra/os_leo_interrupt.c.o src/libultra/os_leo_interrupt.c
.toolchain/ido5.3/cc -c -O3 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/al_syn_delete.c.o src/libultra/al_syn_delete.c
.toolchain/ido5.3/cc -c -O1 -mips2 -non_shared -G 0 -Iinclude \
    -o build/us/src/libultra/os_vi_get_current_context.c.o src/libultra/os_vi_get_current_context.c

"${tool_prefix}objcopy" -I binary -O elf32-tradbigmips -B mips \
    assets/extracted/us/ipl3.bin build/us/assets/extracted/us/ipl3.bin.o

if false; then
"${tool_prefix}nm" -u \
    build/us/asm/us/main_before_80077C40.s.o \
    build/us/src/code/early_hw.c.o \
    build/us/src/code/early_memory_read.c.o \
    build/us/asm/us/main_80077FD0_to_80078048.s.o \
    build/us/src/code/early_memory_write.c.o \
    build/us/src/code/early_remote_copy.c.o \
    build/us/asm/us/main_80078274_to_80078908.s.o \
    build/us/src/code/early_commands.c.o \
    build/us/asm/us/main_80078ADC_to_80078C68.s.o \
    build/us/src/code/early_command_status.c.o \
    build/us/asm/us/main_80078CD8_to_8007A710.s.o \
    build/us/src/code/unknown_8007A710.c.o \
    build/us/src/code/display_slot.c.o \
    build/us/asm/us/main_8007A75C_to_8007AC34.s.o \
    build/us/src/code/display_buffer.c.o \
    build/us/asm/us/main_8007AD40_to_8007AD94.s.o \
    build/us/src/code/display_buffer_select.c.o \
    build/us/src/code/unknown_8007ADB0.c.o \
    build/us/src/code/controller_state.c.o \
    build/us/asm/us/main_8007ADF0_to_8007B020.s.o \
    build/us/src/code/unknown_8007B020.c.o \
    build/us/src/code/gfx_pool.c.o \
    build/us/asm/us/main_8007B0E4_to_8007B1F0.s.o \
    build/us/src/code/render_queue.c.o \
    build/us/src/code/render_submit.c.o \
    build/us/asm/us/main_8007B8EC_to_8007BCF0.s.o \
    build/us/src/code/asset_load.c.o \
    build/us/src/code/video_mode.c.o \
    build/us/src/code/texture_tile.c.o \
    build/us/asm/us/main_8007C364_to_8007D470.s.o \
    build/us/src/code/small_state.c.o \
    build/us/src/code/mapped_record.c.o \
    build/us/src/code/small_state_copy.c.o \
    build/us/asm/us/main_8007D5B0_to_8007D694.s.o \
    build/us/src/code/collision_noop.c.o \
    build/us/asm/us/main_8007D69C_to_8007D710.s.o \
    build/us/src/code/state_noop.c.o \
    build/us/asm/us/main_8007D718_to_8007D720.s.o \
    build/us/src/code/state_modes.c.o \
    build/us/asm/us/main_8007D7C4_to_8007E1FC.s.o \
    build/us/src/code/collision_fields.c.o \
    build/us/asm/us/main_8007E210_to_8007E778.s.o \
    build/us/src/code/object_query.c.o \
    build/us/asm/us/main_8007E7A8_to_80081FD8.s.o \
    build/us/src/code/object_defaults.c.o \
    build/us/asm/us/main_80082004_to_80082B40.s.o \
    build/us/src/code/mode_owner.c.o \
    build/us/asm/us/main_80082B68_to_80082BD4.s.o \
    build/us/src/code/mode_transition.c.o \
    build/us/asm/us/main_80082C1C_to_80082D60.s.o \
    build/us/src/code/pair_queue.c.o \
    build/us/asm/us/main_80082D98_to_80082FE0.s.o \
    build/us/src/code/object_disable.c.o \
    build/us/asm/us/main_80083028_to_80083FCC.s.o \
    build/us/src/code/object_predicates.c.o \
    build/us/asm/us/main_80083FF8_to_80084C50.s.o \
    build/us/src/code/object_direction.c.o \
    build/us/asm/us/main_80084CC8_to_800859A8.s.o \
    build/us/src/code/object_action.c.o \
    build/us/asm/us/main_800859E4_to_8009C284.s.o \
    build/us/src/code/session_queries.c.o \
    build/us/asm/us/main_8009C31C_to_8009D144.s.o \
    build/us/src/code/mode_range.c.o \
    build/us/asm/us/main_8009D168_to_8009D578.s.o \
    build/us/src/code/angle_fold.c.o \
    build/us/asm/us/main_8009D5B4_to_8009D6DC.s.o \
    build/us/src/code/angle_subtract.c.o \
    build/us/asm/us/main_8009D6F8_to_8009D72C.s.o \
    build/us/src/code/angle_between.c.o \
    build/us/asm/us/main_8009D75C_to_8009D81C.s.o \
    build/us/src/code/angle_distance.c.o \
    build/us/src/code/angle_direction.c.o \
    build/us/asm/us/main_8009D8A0_to_8009D914.s.o \
    build/us/src/code/random_integer.c.o \
    build/us/asm/us/main_8009D960_to_8009DA34.s.o \
    build/us/src/code/vector2.c.o \
    build/us/asm/us/main_8009DAB0_to_8009DB0C.s.o \
    build/us/src/code/vector2_scale.c.o \
    build/us/asm/us/main_8009DB2C_to_8009E044.s.o \
    build/us/src/code/vector2_motion.c.o \
    build/us/asm/us/main_8009E0E8_to_8009EEA0.s.o \
    build/us/src/code/game_queue.c.o \
    build/us/asm/us/main_8009EED8_to_8009EEE0.s.o \
    build/us/src/code/matrix_basic.c.o \
    build/us/asm/us/main_8009EF4C_to_8009F064.s.o \
    build/us/src/code/matrix_state.c.o \
    build/us/src/code/codex_batch_next2/func_8009F090.c.o \
    build/us/src/code/matrix_vector.c.o \
    build/us/src/code/codex_batch_next2/func_8009F334.c.o \
    build/us/src/code/matrix_multiply.c.o \
    build/us/asm/us/main_8009F5AC_to_8009F768.s.o \
    build/us/src/code/matrix_transform.c.o \
    build/us/src/code/codex_batch_next2/func_8009F8A0.c.o \
    build/us/asm/us/main_8009FCCC_to_8009FF1C.s.o \
    build/us/src/code/vector2_rotate.c.o \
    build/us/asm/us/main_8009FFB8_to_800A1280.s.o \
    build/us/src/code/scheduler_context.c.o \
    build/us/src/code/codex_batch_next2/func_800A1290.c.o \
    build/us/src/code/scheduler_state.c.o \
    build/us/asm/us/main_800A1384_to_800A179C.s.o \
    build/us/src/code/scheduler_events.c.o \
    build/us/asm/us/main_800A18D0_to_800A1A28.s.o \
    build/us/src/code/scheduler_queue.c.o \
    build/us/src/code/codex_batch_next2/func_800A1B44.c.o \
    build/us/asm/us/main_800A1BE0_to_800A2B74.s.o \
    build/us/src/code/object_range.c.o \
    build/us/asm/us/main_800A2B9C_to_800A2DFC.s.o \
    build/us/src/code/scheduler_misc.c.o \
    build/us/asm/us/main_800A2E5C_to_800A4098.s.o \
    build/us/src/code/object_timing.c.o \
    build/us/asm/us/main_800A40CC_to_800A6ABC.s.o \
    build/us/src/code/object_setters.c.o \
    build/us/asm/us/main_800A6B7C_to_800A7290.s.o \
    build/us/src/code/object_init.c.o \
    build/us/asm/us/main_800A72C0_to_800A8B14.s.o \
    build/us/src/code/object_reset.c.o \
    build/us/asm/us/main_800A8B38_to_800A9054.s.o \
    build/us/src/code/object_flags.c.o \
    build/us/asm/us/main_800A9080_to_800A974C.s.o \
    build/us/src/code/object_limit.c.o \
    build/us/asm/us/main_800A977C_to_800A9BF0.s.o \
    build/us/src/code/object_table_color.c.o \
    build/us/asm/us/main_800A9C24_to_800A9D50.s.o \
    build/us/src/code/object_table_reset.c.o \
    build/us/asm/us/main_800A9D78_to_800AA598.s.o \
    build/us/src/code/object_table_lookup.c.o \
    build/us/asm/us/main_800AA5C4_to_800B0444.s.o \
    build/us/src/code/gameplay_stub.c.o \
    build/us/asm/us/main_800B044C_to_800B06A8.s.o \
    build/us/src/code/player_color.c.o \
    build/us/asm/us/main_800B06D8_to_800B5F30.s.o \
    build/us/src/code/turn_adjust.c.o \
    build/us/asm/us/main_800B5F70_to_800B99C0.s.o \
    build/us/src/code/display_registry.c.o \
    build/us/asm/us/main_800B99F8_to_800B9A4C.s.o \
    build/us/src/code/display_color.c.o \
    build/us/asm/us/main_800B9AB0_to_800B9C68.s.o \
    build/us/src/code/display_commands.c.o \
    build/us/asm/us/main_800B9CAC_to_800B9D4C.s.o \
    build/us/src/code/table_lookup.c.o \
    build/us/asm/us/main_800B9E24_to_800BD880.s.o \
    build/us/src/code/entry_scan.c.o \
    build/us/src/code/entry_flags.c.o \
    build/us/asm/us/main_800BD93C_to_800BFD40.s.o \
    build/us/src/code/hud_state.c.o \
    build/us/asm/us/main_800BFDA4_to_800C03F0.s.o \
    build/us/src/code/hud_root_callback.c.o \
    build/us/asm/us/main_800C041C_to_800C0800.s.o \
    build/us/src/code/hud_modes.c.o \
    build/us/src/code/menu_state.c.o \
    build/us/asm/us/main_800C08E0_to_800C0A64.s.o \
    build/us/src/code/hud_stub.c.o \
    build/us/asm/us/main_800C0A6C_to_800C0C18.s.o \
    build/us/src/code/hud_layout.c.o \
    build/us/asm/us/main_800C0C38_to_800C1094.s.o \
    build/us/src/code/menu_toggle.c.o \
    build/us/src/code/hud_secondary.c.o \
    build/us/src/code/hud_callbacks.c.o \
    build/us/src/code/hud_primary_modes.c.o \
    build/us/asm/us/main_800C11B8_to_800C1420.s.o \
    build/us/src/code/menu_countdown.c.o \
    build/us/src/code/hud_transition.c.o \
    build/us/asm/us/main_800C1484_to_800C1578.s.o \
    build/us/src/code/hud_panel_callback.c.o \
    build/us/src/code/hud_list_trigger.c.o \
    build/us/src/code/hud_entry_values.c.o \
    build/us/src/code/hud_list_reset.c.o \
    build/us/asm/us/main_800C16B0_to_800C17C8.s.o \
    build/us/src/code/registry_lookup.c.o \
    build/us/src/code/controls_config.c.o \
    build/us/src/code/hud_ready.c.o \
    build/us/src/code/hud_clear.c.o \
    build/us/src/code/hud_list_callback.c.o \
    build/us/asm/us/main_800C2528_to_800C27EC.s.o \
    build/us/src/code/hud_navigation.c.o \
    build/us/src/code/hud_selection_apply.c.o \
    build/us/asm/us/main_800C2924_to_800C30A0.s.o \
    build/us/src/code/hud_primary_trigger.c.o \
    build/us/src/code/hud_controls_page.c.o \
    build/us/src/code/hud_player_slots_cycle.c.o \
    build/us/src/code/hud_row_preset.c.o \
    build/us/src/code/hud_row_defaults.c.o \
    build/us/src/code/hud_exit_request.c.o \
    build/us/src/code/hud_list_refresh.c.o \
    build/us/src/code/hud_list_focus_cycle.c.o \
    build/us/src/code/hud_marker_update.c.o \
    build/us/src/code/hud_list_open.c.o \
    build/us/src/code/hud_list_close.c.o \
    build/us/src/code/hud_alt_exit_request.c.o \
    build/us/src/code/hud_menu_entries_apply_settings.c.o \
    build/us/src/code/hud_page_select.c.o \
    build/us/src/code/hud_entry_set_label.c.o \
    build/us/asm/us/main_800C4BB4_to_800C4E24.s.o \
    build/us/src/code/hud_page_focus_cycle.c.o \
    build/us/src/code/game_mode8_toggled.c.o \
    build/us/src/code/hud_slot_icons_update.c.o \
    build/us/src/code/hud_mode_cycle.c.o \
    build/us/src/code/hud_panel_layout.c.o \
    build/us/src/code/hud_panel_refresh.c.o \
    build/us/src/code/hud_panel_select.c.o \
    build/us/src/code/hud_page_focus.c.o \
    build/us/src/code/hud_page_next.c.o \
    build/us/src/code/hud_page_exit_request.c.o \
    build/us/src/code/hud_page_show.c.o \
    build/us/src/code/hud_page_open.c.o \
    build/us/src/code/hud_page_close.c.o \
    build/us/src/code/hud_value_to_byte.c.o \
    build/us/asm/us/main_800C6914_to_800C6918.s.o \
    build/us/asm/us/data/main_800C6918_textbin.s.o \
    build/us/src/code/race_assets_load.c.o \
    build/us/src/code/race_hud_init.c.o \
    build/us/src/code/race_result_setup.c.o \
    build/us/src/code/race_object_timestamps.c.o \
    build/us/src/code/race_state_init.c.o \
    build/us/src/code/race_state_reset.c.o \
    build/us/asm/us/main_800C7594_to_800C7650.s.o \
    build/us/src/code/race_frame_border_draw.c.o \
    build/us/src/code/race_start_check.c.o \
    build/us/src/code/race_map_draw.c.o \
    build/us/src/code/race_event_push.c.o \
    build/us/asm/us/main_800C8350_to_800C8484.s.o \
    build/us/src/code/race_timer_update.c.o \
    build/us/src/code/race_marker_project.c.o \
    build/us/src/code/race_player_icons_draw.c.o \
    build/us/src/code/race_slot_clear.c.o \
    build/us/src/code/race_player_set_vehicle_label.c.o \
    build/us/src/code/race_type_name_lookup.c.o \
    build/us/src/code/race_player_panel.c.o \
    build/us/src/code/race_player_set_layout.c.o \
    build/us/src/code/race_player_message.c.o \
    build/us/src/code/race_player_pair_set.c.o \
    build/us/src/code/race_mode_icon_get.c.o \
    build/us/src/code/race_player_set_mode_icon.c.o \
    build/us/src/code/race_display_value_set.c.o \
    build/us/asm/us/main_800C98E8_to_800CA1A8.s.o \
    build/us/src/code/race_player_reset.c.o \
    build/us/src/code/race_lap_times_clamp.c.o \
    build/us/src/code/race_timer_queries.c.o \
    build/us/src/code/race_slot_set_digit.c.o \
    build/us/asm/us/main_800CA620_to_800CAA0C.s.o \
    build/us/src/code/race_popup_show.c.o \
    build/us/asm/us/main_800CAA48_to_800CAB44.s.o \
    build/us/src/code/race_flag_queries.c.o \
    build/us/src/code/race_player_widget.c.o \
    build/us/src/code/race_widget_values.c.o \
    build/us/src/code/race_widget_clear.c.o \
    build/us/src/code/race_widget_reset.c.o \
    build/us/src/code/race_widget_flag_set.c.o \
    build/us/asm/us/main_800CB10C_to_800CB110.s.o \
    build/us/src/code/results_assets_load.c.o \
    build/us/src/code/controls_button_index_map.c.o \
    build/us/src/code/controls_bindings_save.c.o \
    build/us/src/code/controls_bindings_load.c.o \
    build/us/src/code/controller_save_slots.c.o \
    build/us/src/code/controller_slots_scan.c.o \
    build/us/src/code/controller_menu_input.c.o \
    build/us/src/code/controller_queue_push.c.o \
    build/us/src/code/controller_slot_find.c.o \
    build/us/asm/us/main_800CD57C_to_800CD85C.s.o \
    build/us/src/code/results_record_write.c.o \
    build/us/asm/us/main_800CD96C_to_800CDD70.s.o \
    build/us/src/code/results_time_format.c.o \
    build/us/src/code/results_time_compare.c.o \
    build/us/asm/us/main_800CE610_to_800CE814.s.o \
    build/us/src/code/results_screen_setup.c.o \
    build/us/src/code/results_split_setup.c.o \
    build/us/src/code/results_continue.c.o \
    build/us/src/code/results_return.c.o \
    build/us/src/code/results_option_codes.c.o \
    build/us/src/code/results_kills_tick.c.o \
    build/us/src/code/results_deaths_tick.c.o \
    build/us/src/code/results_ready_flags_a.c.o \
    build/us/src/code/results_score_tick.c.o \
    build/us/src/code/results_score_finish.c.o \
    build/us/src/code/results_ready_flags_b.c.o \
    build/us/src/code/results_bonus_tick.c.o \
    build/us/src/code/results_bonus_finish.c.o \
    build/us/src/code/results_ready_flags_c.c.o \
    build/us/src/code/results_time_tick.c.o \
    build/us/src/code/results_status_flags.c.o \
    build/us/asm/us/main_800CFA84_to_800CFBD8.s.o \
    build/us/src/code/results_columns_draw.c.o \
    build/us/asm/us/main_800CFD18_to_800CFDD0.s.o \
    build/us/src/code/results_assets_reload.c.o \
    build/us/src/code/game_state_clear.c.o \
    build/us/asm/us/main_800D0070_to_800D05E0.s.o \
    build/us/src/code/hud_element_unlink.c.o \
    build/us/src/code/spawn_ctl_index.c.o \
    build/us/asm/us/main_800D0858_to_800D0960.s.o \
    build/us/src/code/spawn_ctl_alloc.c.o \
    build/us/src/code/hud_element_alloc.c.o \
    build/us/asm/us/main_800D0A74_to_800D0DF8.s.o \
    build/us/src/code/spawn_effect_by_type.c.o \
    build/us/src/code/spawn_nearest_type1.c.o \
    build/us/src/code/spawn_nearest_type28.c.o \
    build/us/src/code/spawn_nearest_type31.c.o \
    build/us/src/code/spawn_counters.c.o \
    build/us/src/code/800D12B0_script_command_decode.c.o \
    build/us/src/code/spawn_object_command.c.o \
    build/us/src/code/audio_listener_update.c.o \
    build/us/src/code/seq_script_tick.c.o \
    build/us/src/code/seq_spawn_script_tick.c.o \
    build/us/src/code/seq_event_script_tick.c.o \
    build/us/src/code/seq_event_spawn_tick.c.o \
    build/us/src/code/800D3C64_seq_point_track.c.o \
    build/us/src/code/seq_spawn_events_update.c.o \
    build/us/src/code/seq_command_dispatch.c.o \
    build/us/src/code/seq_event_start.c.o \
    build/us/src/code/race_buffers_alloc.c.o \
    build/us/src/code/seq_script_load.c.o \
    build/us/src/code/seq_level_header_load.c.o \
    build/us/src/code/script_event_finish.c.o \
    build/us/src/code/seq_events_update.c.o \
    build/us/src/code/hud_draw_list.c.o \
    build/us/asm/us/main_800D5C7C_to_800D5C80.s.o \
    build/us/src/code/script_particle_spawn.c.o \
    build/us/src/code/800D5E2C_script_particle_update.c.o \
    build/us/asm/us/main_800D6508_to_800D67F0.s.o \
    build/us/src/code/effect_draw.c.o \
    build/us/src/code/effect_code_table.c.o \
    build/us/src/code/effect_code_parse.c.o \
    build/us/asm/us/main_800D6E34_to_800D6E40.s.o \
    build/us/src/code/player_attach_effect_create.c.o \
    build/us/src/code/player_menu_update.c.o \
    build/us/src/code/player_cursor_anim.c.o \
    build/us/src/code/player_menu_draw.c.o \
    build/us/src/code/player_attach_effect_matrix.c.o \
    build/us/src/code/player_attach_effect_matrix_cb.c.o \
    build/us/asm/us/main_800D7DD4_to_800D7DE0.s.o \
    build/us/src/code/effect_node_spawn.c.o \
    build/us/src/code/actor_collision_free.c.o \
    build/us/src/code/actor_model_draw.c.o \
    build/us/src/code/actor_timer_update.c.o \
    build/us/asm/us/main_800D84D8_to_800D84DC.s.o \
    build/us/src/code/actor_message_handler.c.o \
    build/us/src/code/turret_message_handlers.c.o \
    build/us/src/code/turret_state_messages.c.o \
    build/us/asm/us/main_800D88E8_to_800D88F0.s.o \
    build/us/src/code/turret_weights_total.c.o \
    build/us/src/code/turret_weighted_pick.c.o \
    build/us/src/code/turret_random_slot.c.o \
    build/us/src/code/turret_model_draw.c.o \
    build/us/src/code/turret_spawn_random.c.o \
    build/us/src/code/single_player_check.c.o \
    build/us/src/code/single_spawn_timer_update.c.o \
    build/us/src/code/trigger_zone_query.c.o \
    build/us/asm/us/main_800D92D4_to_800D92E0.s.o \
    build/us/src/code/trigger_particles_spawn.c.o \
    build/us/src/code/trigger_particles_update.c.o \
    build/us/src/code/minimap_layer_build.c.o \
    build/us/src/code/minimap_blips_spawn.c.o \
    build/us/src/code/model_texture_find.c.o \
    build/us/asm/us/main_800D9B48_to_800D9B50.s.o \
    build/us/src/code/model_bounds_compute.c.o \
    build/us/src/code/model_bounds_center.c.o \
    build/us/src/code/800D9D50_debris_spawn.c.o \
    build/us/src/code/800DA1D8_debris_piece_update.c.o \
    build/us/src/code/debris_piece_draw.c.o \
    build/us/src/code/debris_burst_spawn.c.o \
    build/us/src/code/debris_burst_spawn_tinted.c.o \
    build/us/src/code/800DA7D0_debris_group_spawn.c.o \
    build/us/src/code/debris_scatter_spawn.c.o \
    build/us/src/code/800DAAE0_projectile_spawn.c.o \
    build/us/src/code/projectile_bounce.c.o \
    build/us/src/code/800DB1B0_projectile_segment_spawn.c.o \
    build/us/asm/us/main_800DB4E8_to_800DB6F0.s.o \
    build/us/src/code/800DB6F0_track_angle_unit.c.o \
    build/us/src/code/projectile_model_draw.c.o \
    build/us/src/code/projectile_model_create.c.o \
    build/us/src/code/projectile_model_matrix.c.o \
    build/us/src/code/projectile_model_matrix_cb.c.o \
    build/us/asm/us/main_800DC7A4_to_800DC7B0.s.o \
    build/us/src/code/projectile_life_tick.c.o \
    build/us/src/code/projectile_trail_draw.c.o \
    build/us/src/code/projectile_fire.c.o \
    build/us/src/code/projectile_launch_delayed.c.o \
    build/us/src/code/projectile_launch_delayed_tick.c.o \
    build/us/asm/us/main_800DD28C_to_800DD290.s.o \
    build/us/src/code/model_mesh_partition.c.o \
    build/us/src/code/structure_create.c.o \
    build/us/src/code/structure_flicker_roll.c.o \
    build/us/src/code/800DD75C_structure_hit_dispatch.c.o \
    build/us/src/code/800DD82C_structure_debris_spawn.c.o \
    build/us/src/code/structure_model_draw.c.o \
    build/us/src/code/structure_owner_award.c.o \
    build/us/src/code/800DE374_structure_message.c.o \
    build/us/src/code/spawner_target_query.c.o \
    build/us/src/code/spawner_state_update.c.o \
    build/us/src/code/spawner_owner_score.c.o \
    build/us/src/code/spawner_message_handler.c.o \
    build/us/src/code/unit_spawner_create.c.o \
    build/us/src/code/unit_spawner_update.c.o \
    build/us/asm/us/main_800DF0C4_to_800DF0D0.s.o \
    build/us/src/code/slot_corners_build.c.o \
    build/us/src/code/prop_multiplayer_filter.c.o \
    build/us/src/code/slot_condition_check.c.o \
    build/us/src/code/slot_entity_attach.c.o \
    build/us/src/code/slot_angle_classify.c.o \
    build/us/src/code/slot_entry_lookup.c.o \
    build/us/src/code/level_prop_points_find.c.o \
    build/us/asm/us/main_800DF89C_to_800E1540.s.o \
    build/us/src/code/foliage_prop_create.c.o \
    build/us/src/code/800E16D8_foliage_prop_destroy.c.o \
    build/us/src/code/player_message_hit.c.o \
    build/us/src/code/foliage_prop_handlers.c.o \
    build/us/src/code/player_dispatch.c.o \
    build/us/asm/us/main_800E1BA4_to_800E1BB0.s.o \
    build/us/src/code/800E1BB0_turret_sweep.c.o \
    build/us/src/code/turret_destroy.c.o \
    build/us/src/code/turret_draw_create.c.o \
    build/us/src/code/800E2520_turret_message_handlers.c.o \
    build/us/src/code/turret_message_handler.c.o \
    build/us/src/code/target_search.c.o \
    build/us/src/code/wreck_spawn.c.o \
    build/us/src/code/turret_spawn_at.c.o \
    build/us/src/code/turret_line_of_sight.c.o \
    build/us/src/code/wreck_ctl.c.o \
    build/us/src/code/800E2F9C_wreck_update.c.o \
    build/us/src/code/wreck_timer_duration.c.o \
    build/us/asm/us/main_800E3460_to_800E44C8.s.o \
    build/us/src/code/wreck_message_hit.c.o \
    build/us/src/code/object_damage.c.o \
    build/us/src/code/flag_capture_attempt.c.o \
    build/us/src/code/800E48A8_flag_position_query.c.o \
    build/us/src/code/flag_owner_capture.c.o \
    build/us/src/code/flag_owner_score.c.o \
    build/us/asm/us/main_800E4DA0_to_800E4ECC.s.o \
    build/us/src/code/actor_kind16_link_clear.c.o \
    build/us/src/code/actor_wreck_spawn_at.c.o \
    build/us/src/code/sound_emitter_delayed_spawn.c.o \
    build/us/src/code/sound_emitter_draw.c.o \
    build/us/src/code/ambient_sound_stop_first.c.o \
    build/us/asm/us/main_800E56C8_to_800E56D0.s.o \
    build/us/src/code/bridge_create_handlers.c.o \
    build/us/asm/us/main_800E58C4_to_800E58D0.s.o \
    build/us/src/code/bridge_segments_init.c.o \
    build/us/asm/us/main_800E5BB8_to_800E6040.s.o \
    build/us/src/code/bridge_piece_spawn.c.o \
    build/us/src/code/lightning_arc_update.c.o \
    build/us/asm/us/main_800E64EC_to_800E64F0.s.o \
    build/us/src/code/crate_model_randomize.c.o \
    build/us/src/code/crate_actor_attach.c.o \
    build/us/src/code/800E66A8_crate_particle_attach.c.o \
    build/us/src/code/crate_owner_check.c.o \
    build/us/src/code/800E6B04_crate_unit.c.o \
    build/us/src/code/crate_contents_spawn.c.o \
    build/us/src/code/800E73B0_crate_part_release.c.o \
    build/us/src/code/pending_list_flush.c.o \
    build/us/src/code/pending_spawns_flush.c.o \
    build/us/src/code/800E7768_crate_burst_spawn.c.o \
    build/us/src/code/powerup_flag_players.c.o \
    build/us/src/code/800E7A10_powerup_flag_messages.c.o \
    build/us/src/code/powerup_count_refresh.c.o \
    build/us/src/code/pickup_actor_count.c.o \
    build/us/asm/us/main_800E7F5C_to_800E7F60.s.o \
    build/us/src/code/pickup_find_by_id.c.o \
    build/us/src/code/game_mode_has_pickups.c.o \
    build/us/src/code/game_team_compare.c.o \
    build/us/asm/us/main_800E80F4_to_800E8318.s.o \
    build/us/src/code/slist_remove_count.c.o \
    build/us/asm/us/main_800E8378_to_800E8C88.s.o \
    build/us/src/code/slot_mode_name_get.c.o \
    build/us/src/code/slot_mode_title_get.c.o \
    build/us/src/code/slot_mode_subtitle_get.c.o \
    build/us/src/code/slot_mode_description_get.c.o \
    build/us/src/code/slot_mode_value_get.c.o \
    build/us/asm/us/main_800E92E8_to_800E92F0.s.o \
    build/us/src/code/mission_select_init.c.o \
    build/us/src/code/mission_entry_query.c.o \
    build/us/src/code/mission_code_parse.c.o \
    build/us/src/code/mission_flag_set.c.o \
    build/us/src/code/mission_time_bonus.c.o \
    build/us/asm/us/main_800E9990_to_800E9B50.s.o \
    build/us/src/code/model_pickup_create.c.o \
    build/us/src/code/model_mode_objective_message.c.o \
    build/us/src/code/mission_event_forward.c.o \
    build/us/asm/us/main_800E9E78_to_800E9E80.s.o \
    build/us/src/code/mission_props_spawn.c.o \
    build/us/src/code/mission_targets_query.c.o \
    build/us/asm/us/main_800EA224_to_800EA714.s.o \
    build/us/src/code/mission_marker_draw.c.o \
    build/us/src/code/model_message_damage_u8.c.o \
    build/us/src/code/building_target_query.c.o \
    build/us/src/code/building_destroy_on_hit.c.o \
    build/us/src/code/building_damage_apply.c.o \
    build/us/src/code/model_turret_dispatch.c.o \
    build/us/asm/us/main_800EAC2C_to_800EAC30.s.o \
    build/us/src/code/building_create.c.o \
    build/us/src/code/building_anim_frame_tick.c.o \
    build/us/asm/us/main_800EAEB4_to_800EAF6C.s.o \
    build/us/src/code/building_destroy.c.o \
    build/us/asm/us/main_800EB308_to_800EB4FC.s.o \
    build/us/src/code/model_shielded_dispatch.c.o \
    build/us/asm/us/main_800EB714_to_800EB720.s.o \
    build/us/src/code/impact_flash_spawn.c.o \
    build/us/src/code/impact_debris_spray.c.o \
    build/us/asm/us/main_800EB838_to_800EB934.s.o \
    build/us/src/code/impact_flash_expire.c.o \
    build/us/asm/us/main_800EBA98_to_800EBCA8.s.o \
    build/us/src/code/player_projectile_launch.c.o \
    build/us/asm/us/main_800EBDA0_to_800EC1C0.s.o \
    build/us/src/code/mission_counter_release.c.o \
    build/us/asm/us/main_800EC1F8_to_800EC4A8.s.o \
    build/us/src/code/mission_objectives_update.c.o \
    build/us/src/code/splash_damage_falloff.c.o \
    build/us/asm/us/main_800EC788_to_800EC790.s.o \
    build/us/src/code/projectile_spawn_typed.c.o \
    build/us/asm/us/main_800EC8E8_to_800ECEBC.s.o \
    build/us/src/code/projectile_shell_draw.c.o \
    build/us/src/code/shockwave_ring_spawn.c.o \
    build/us/src/code/shockwave_expand.c.o \
    build/us/src/code/shockwave_ring_draw.c.o \
    build/us/src/code/destructible_prop_create.c.o \
    build/us/asm/us/main_800ED4F4_to_800ED63C.s.o \
    build/us/src/code/destructible_anim_frame_tick.c.o \
    build/us/asm/us/main_800ED694_to_800ED698.s.o \
    build/us/src/code/model_message_damage_destroy.c.o \
    build/us/src/code/projectile_target_query.c.o \
    build/us/src/code/destructible_prop_on_hit.c.o \
    build/us/src/code/destructible_prop_on_hit_alt.c.o \
    build/us/asm/us/main_800ED804_to_800EDC00.s.o \
    build/us/src/code/prop_destroy_slot_release.c.o \
    build/us/asm/us/main_800EDDCC_to_800EDF14.s.o \
    build/us/src/code/prop_message_shatter.c.o \
    build/us/src/code/prop_target_query.c.o \
    build/us/src/code/prop_debris_burst_on_hit.c.o \
    build/us/src/code/prop_debris_burst_on_hit_alt.c.o \
    build/us/asm/us/main_800EE288_to_800EE688.s.o \
    build/us/src/code/prop_spawn_child.c.o \
    build/us/src/code/prop_spawn_pickup.c.o \
    build/us/src/code/prop_height_update.c.o \
    build/us/src/code/barrier_break_open.c.o \
    build/us/asm/us/main_800EED90_to_800EF400.s.o \
    build/us/src/code/effect_segment_append.c.o \
    build/us/src/code/effect_segments_draw.c.o \
    build/us/asm/us/main_800EF770_to_800EFB30.s.o \
    build/us/src/code/effect_message_damage.c.o \
    build/us/src/code/effect_message_damage_guarded.c.o \
    build/us/src/code/particle_emitter_tail.c.o \
    build/us/asm/us/main_800EFC68_to_800EFC70.s.o \
    build/us/src/code/particle_emitter_create.c.o \
    build/us/src/code/particle_pool_init.c.o \
    build/us/src/code/particle_emitter_free.c.o \
    build/us/src/code/particle_emitter_update.c.o \
    build/us/asm/us/main_800F01F0_to_800F0618.s.o \
    build/us/src/code/particle_free_list.c.o \
    build/us/src/code/particle_smoke_update.c.o \
    build/us/src/code/particle_node_list.c.o \
    build/us/asm/us/main_800F0A20_to_800F0B08.s.o \
    build/us/src/code/particle_hit_spawn.c.o \
    build/us/src/code/particle_system_create.c.o \
    build/us/src/code/particle_owner_message.c.o \
    build/us/asm/us/main_800F1278_to_800F1770.s.o \
    build/us/src/code/anim_list_register_global.c.o \
    build/us/asm/us/main_800F17AC_to_800F17B0.s.o \
    build/us/src/code/anim_keyframes_apply.c.o \
    build/us/asm/us/main_800F1900_to_800F1B30.s.o \
    build/us/src/code/hazard_actor_spawn.c.o \
    build/us/src/code/hazard_actor_spawn_simple.c.o \
    build/us/src/code/hazard_state_update.c.o \
    build/us/src/code/hazard_model_draw.c.o \
    build/us/asm/us/main_800F2184_to_800F2288.s.o \
    build/us/src/code/hazard_message_handler.c.o \
    build/us/src/code/object_state_query.c.o \
    build/us/src/code/mine_message_handlers.c.o \
    build/us/src/code/mine_trigger_update.c.o \
    build/us/asm/us/main_800F2BB8_to_800F2BC0.s.o \
    build/us/src/code/powerup_pad_create.c.o \
    build/us/src/code/powerup_timer_update.c.o \
    build/us/src/code/powerup_pad_draw.c.o \
    build/us/asm/us/main_800F3208_to_800F32EC.s.o \
    build/us/src/code/barrel_handlers.c.o \
    build/us/src/code/barrel_explode.c.o \
    build/us/src/code/falling_crate_update.c.o \
    build/us/src/code/decal_spawn_draw.c.o \
    build/us/asm/us/main_800F3B7C_to_800F3B80.s.o \
    build/us/src/code/color_interpolate.c.o \
    build/us/src/code/mesh_vertex_transform.c.o \
    build/us/src/code/mesh_rings_draw.c.o \
    build/us/asm/us/main_800F4D74_to_800F4D80.s.o \
    build/us/src/code/mesh_wave_update.c.o \
    build/us/src/code/tracer_spawn.c.o \
    build/us/src/code/tracer_trail_update.c.o \
    build/us/asm/us/main_800F5264_to_800F5270.s.o \
    build/us/src/code/effect_table_reset.c.o \
    build/us/src/code/effect_prop_spawn.c.o \
    build/us/src/code/effect_targets_query.c.o \
    build/us/src/code/effect_targets_damage.c.o \
    build/us/src/code/effect_burn_update.c.o \
    build/us/src/code/effect_beam_draw.c.o \
    build/us/src/code/generator_damage_apply.c.o \
    build/us/src/code/generator_message_damage.c.o \
    build/us/src/code/generator_hit_query.c.o \
    build/us/src/code/generator_splash_damage.c.o \
    build/us/src/code/generator_damage_tick.c.o \
    build/us/src/code/generator_dispatch_bonus.c.o \
    build/us/asm/us/main_800F6648_to_800F6650.s.o \
    build/us/src/code/generator_spawn.c.o \
    build/us/src/code/effect_expire_100.c.o \
    build/us/asm/us/main_800F67F0_to_800F6ED0.s.o \
    build/us/src/code/wreck_debris_spawn.c.o \
    build/us/src/code/wreck_debris_update.c.o \
    build/us/src/code/anim_colors_set.c.o \
    build/us/src/code/anim_list_create.c.o \
    build/us/asm/us/main_800F7230_to_800F756C.s.o \
    build/us/src/code/anim_list_reset.c.o \
    build/us/src/code/anim_list_register.c.o \
    build/us/asm/us/main_800F7648_to_800F7870.s.o \
    build/us/src/code/anim_alpha_fade_draw.c.o \
    build/us/src/code/spark_spawn.c.o \
    build/us/src/code/spark_burst_update.c.o \
    build/us/src/code/smoke_puff_spawn.c.o \
    build/us/asm/us/main_800F7C24_to_800F7C30.s.o \
    build/us/src/code/flicker_prop_create.c.o \
    build/us/src/code/flicker_prop_draw.c.o \
    build/us/src/code/flicker_prop_message.c.o \
    build/us/src/code/flicker_prop_message_cb.c.o \
    build/us/asm/us/main_800F7EB8_to_800F7EC0.s.o \
    build/us/src/code/static_prop_create.c.o \
    build/us/src/code/static_prop_expire.c.o \
    build/us/src/code/static_prop_draw.c.o \
    build/us/asm/us/main_800F809C_to_800F80A0.s.o \
    build/us/src/code/static_in_radius.c.o \
    build/us/src/code/model_variant_get.c.o \
    build/us/src/code/model_cache_globals.c.o \
    build/us/src/code/model_in_draw_range.c.o \
    build/us/asm/us/main_800F83B8_to_800F83C0.s.o \
    build/us/src/code/artillery_emplacement_update.c.o \
    build/us/src/code/artillery_emplacement_disable.c.o \
    build/us/src/code/artillery_emplacement_create.c.o \
    build/us/asm/us/main_800F865C_to_800F8660.s.o \
    build/us/src/code/artillery_projectile_create.c.o \
    build/us/src/code/artillery_target_track.c.o \
    build/us/asm/us/main_800F8AAC_to_801029D0.s.o \
    build/us/src/libultra/os_ai_get_length.c.o \
    build/us/src/libultra/os_ai_get_status.c.o \
    build/us/asm/us/main_801029F0_to_80103160.s.o \
    build/us/src/libultra/os_ai_device_busy.c.o \
    build/us/asm/us/main_80103190_to_801037B0.s.o \
    build/us/src/libultra/al_copy.c.o \
    build/us/src/libultra/os_create_mesg_queue.c.o \
    build/us/src/libultra/os_create_thread.c.o \
    build/us/asm/us/main_801039B0_to_80104EC0.s.o \
    build/us/src/libultra/os_epi_start_dma.c.o \
    build/us/asm/us/main_80104FA0_to_801058B0.s.o \
    build/us/src/libultra/os_destroy_thread.c.o \
    build/us/src/libultra/al_filter_new.c.o \
    build/us/src/libultra/os_get_time.c.o \
    build/us/asm/us/main_80105A60_to_80105A70.s.o \
    build/us/src/libultra/al_heap_alloc.c.o \
    build/us/src/libultra/al_heap_init.c.o \
    build/us/asm/us/main_80105B10_to_80105F20.s.o \
    build/us/src/libultra/os_jam_mesg.c.o \
    build/us/asm/us/main_80106070_to_801068F0.s.o \
    build/us/src/libultra/os_epi_raw_start_dma.c.o \
    build/us/src/libultra/os_leo_disk_init.c.o \
    build/us/src/libultra/ll.c.o \
    build/us/asm/us/main_80106EE0_to_80107A30.s.o \
    build/us/src/libultra/gu_look_at.c.o \
    build/us/asm/us/main_80107D60_to_80108530.s.o \
    build/us/src/libultra/os_cont_ram_write.c.o \
    build/us/src/libultra/os_cont_ram_read.c.o \
    build/us/src/libultra/os_cont_crc.c.o \
    build/us/src/libultra/gu_mtx.c.o \
    build/us/asm/us/main_80109030_to_80109090.s.o \
    build/us/src/libultra/gu_perspective.c.o \
    build/us/src/libultra/cosf.c.o \
    build/us/asm/us/main_80109490_to_8010A9A0.s.o \
    build/us/src/libultra/os_pfs_delete_file.c.o \
    build/us/src/libultra/os_pfs_file_state.c.o \
    build/us/src/libultra/os_pfs_free_blocks.c.o \
    build/us/src/libultra/os_pfs_get_status.c.o \
    build/us/src/libultra/os_pfs_init_pak.c.o \
    build/us/src/libultra/os_pfs_checker.c.o \
    build/us/src/libultra/os_pfs_is_plug.c.o \
    build/us/src/libultra/os_pfs_read_write_file.c.o \
    build/us/src/libultra/os_pfs_get_id_copy.c.o \
    build/us/src/libultra/os_pfs_find_file.c.o \
    build/us/src/libultra/os_pi_start_dma.c.o \
    build/us/src/libultra/os_pi_get_cmd_queue.c.o \
    build/us/asm/us/main_8010CFA0_to_8010D660.s.o \
    build/us/src/libultra/os_get_thread_pri.c.o \
    build/us/src/libultra/os_pi_access.c.o \
    build/us/src/libultra/os_pi_raw_start_dma.c.o \
    build/us/src/libultra/os_recv_mesg.c.o \
    build/us/asm/us/main_8010D9C0_to_8010DCC0.s.o \
    build/us/src/libultra/os_reset_global_int_mask.c.o \
    build/us/src/libultra/al_reverb.c.o \
    build/us/src/libultra/gu_rotate.c.o \
    build/us/src/libultra/gu_normalize.c.o \
    build/us/src/libultra/al_save_param.c.o \
    build/us/src/libultra/al_save_pull.c.o \
    build/us/src/libultra/sched.c.o \
    build/us/src/libultra/os_dp_set_next_buffer.c.o \
    build/us/src/libultra/os_dp_device_busy.c.o \
    build/us/src/libultra/os_send_mesg.c.o \
    build/us/src/libultra/os_set_event_mesg.c.o \
    build/us/asm/us/main_8010F890_to_8010F8A0.s.o \
    build/us/src/libultra/os_set_global_int_mask.c.o \
    build/us/asm/us/main_8010F8F0_to_8010F9A0.s.o \
    build/us/src/libultra/os_set_thread_pri.c.o \
    build/us/src/libultra/os_set_timer.c.o \
    build/us/src/libultra/os_si_access_queue.c.o \
    build/us/src/libultra/sinf.c.o \
    build/us/src/libultra/os_si_raw_start_dma.c.o \
    build/us/src/libultra/os_si_device_busy.c.o \
    build/us/src/libultra/os_si_raw_read_io.c.o \
    build/us/src/libultra/os_si_raw_write_io.c.o \
    build/us/src/libultra/al_main.c.o \
    build/us/src/libultra/sprintf.c.o \
    build/us/src/libultra/os_sp_set_pc.c.o \
    build/us/src/libultra/os_sp_task.c.o \
    build/us/src/libultra/os_sp_device_busy.c.o \
    build/us/src/libultra/os_sp_raw_start_dma.c.o \
    build/us/src/libultra/os_sp_set_status.c.o \
    build/us/src/libultra/os_sp_task_yield.c.o \
    build/us/src/libultra/os_sp_task_yielded.c.o \
    build/us/src/libultra/os_sp_get_status.c.o \
    build/us/asm/us/main_80110550_to_80110560.s.o \
    build/us/src/libultra/os_start_thread.c.o \
    build/us/src/libultra/string.c.o \
    build/us/src/libultra/al_syn_delete.c.o \
    build/us/src/libultra/al_synthesizer.c.o \
    build/us/src/libultra/al_syn_alloc_fx.c.o \
    build/us/src/libultra/os_dequeue_thread.c.o \
    build/us/src/libultra/os_timer.c.o \
    build/us/asm/us/main_80111320_to_80111330.s.o \
    build/us/src/libultra/os_vi_black.c.o \
    build/us/src/libultra/os_vi_init.c.o \
    build/us/src/libultra/os_vi_get_current_framebuffer.c.o \
    build/us/src/libultra/os_vi_get_next_framebuffer.c.o \
    build/us/src/libultra/os_vi_manager.c.o \
    build/us/src/libultra/os_vi_get_current_context.c.o \
    build/us/src/libultra/os_virtual_to_physical.c.o \
    build/us/asm/us/main_80111950_to_80111A10.s.o \
    build/us/src/libultra/os_vi_set_event.c.o \
    build/us/src/libultra/os_vi_set_mode.c.o \
    build/us/src/libultra/os_vi_set_special_features.c.o \
    build/us/src/libultra/os_vi_swap_buffer.c.o \
    build/us/src/libultra/os_vi_swap_context.c.o \
    build/us/asm/us/main_80112060_to_80112110.s.o \
    build/us/src/libultra/xprintf.c.o \
    build/us/src/libultra/xldtob.c.o \
    build/us/src/libultra/ldiv.c.o \
    build/us/src/libultra/litob.c.o \
    build/us/src/libultra/os_yield_thread.c.o \
    build/us/asm/us/main_after_80113D10.s.o \
    > build/us/undefined_object_symbols.txt
fi
find build/us/asm build/us/src -type f -name '*.o' -print0 \
    | xargs -0 "${tool_prefix}nm" -u \
    > build/us/undefined_object_symbols.txt
python3 tools/generate_linker_symbols.py build/us/symbols.ld \
    config/us/symbol_addrs.txt \
    build/us/undefined_syms_auto.txt \
    build/us/undefined_funcs_auto.txt \
    --undefined-list build/us/undefined_object_symbols.txt \
    --symbol func_8E180004=0x8E180004 \
    --symbol func_80000000=0x80000000 \
    --symbol D_803A66C0=0x803A66C0

python3 tools/place_unit_rodata.py battletanx_ga.ld config/us/unit_rodata.tsv \
    build/us/battletanx_ga.placed.ld --bss-table config/us/unit_bss.tsv \
    --data-table config/us/unit_data.tsv
"${tool_prefix}ld.bfd" -EB --no-check-sections \
    -T build/us/symbols.ld -T build/us/battletanx_ga.placed.ld \
    -Map build/us/battletanx_ga.map \
    -o build/us/battletanx_ga.elf
"${tool_prefix}objcopy" -O binary -R '.unit_rodata_*' -R '.unit_data_*' \
    build/us/battletanx_ga.elf build/us/battletanx_ga.code.bin
python3 tools/splice_unit_rodata.py build/us/battletanx_ga.elf \
    config/us/unit_rodata.tsv build/us/battletanx_ga.code.bin \
    --data-table config/us/unit_data.tsv

actual_size="$(wc -c < build/us/battletanx_ga.code.bin)"
[[ "$actual_size" -eq 1052672 ]] || {
    echo "Reconstructed code region is $actual_size bytes; expected 1052672" >&2
    exit 1
}

echo "Built exact-reconstruction candidate: build/us/battletanx_ga.code.bin"
