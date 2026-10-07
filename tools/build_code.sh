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
    -o build/us/asm/us/main_8007B8EC_to_8007BCF0.s.o asm/us/main_8007B8EC_to_8007BCF0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007B8EC_to_8007BCF0.s.o .text 0x404 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007C364_to_8007D470.s.o asm/us/main_8007C364_to_8007D470.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007C364_to_8007D470.s.o .text 0x110c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8007D500_to_8007D558.s.o asm/us/main_8007D500_to_8007D558.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007D500_to_8007D558.s.o .text 0x58 --alignment 4
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
    -o build/us/asm/us/main_8007D7C4_to_8009C284.s.o asm/us/main_8007D7C4_to_8009C284.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8007D7C4_to_8009C284.s.o .text 0x1eac0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009C31C_to_8009D914.s.o asm/us/main_8009C31C_to_8009D914.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009C31C_to_8009D914.s.o .text 0x15f8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009D960_to_8009DA34.s.o asm/us/main_8009D960_to_8009DA34.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009D960_to_8009DA34.s.o .text 0xd4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009DAB0_to_8009E044.s.o asm/us/main_8009DAB0_to_8009E044.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009DAB0_to_8009E044.s.o .text 0x594 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009E0E8_to_8009EEE0.s.o asm/us/main_8009E0E8_to_8009EEE0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009E0E8_to_8009EEE0.s.o .text 0xdf8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009EF4C_to_8009F1F4.s.o asm/us/main_8009EF4C_to_8009F1F4.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009EF4C_to_8009F1F4.s.o .text 0x2a8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009F334_to_8009F4B4.s.o asm/us/main_8009F334_to_8009F4B4.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009F334_to_8009F4B4.s.o .text 0x180 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009F5AC_to_8009F768.s.o asm/us/main_8009F5AC_to_8009F768.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009F5AC_to_8009F768.s.o .text 0x1bc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009F8A0_to_8009FF1C.s.o asm/us/main_8009F8A0_to_8009FF1C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009F8A0_to_8009FF1C.s.o .text 0x67c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_8009FFB8_to_800A1280.s.o asm/us/main_8009FFB8_to_800A1280.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_8009FFB8_to_800A1280.s.o .text 0x12c8 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A1290_to_800A134C.s.o asm/us/main_800A1290_to_800A134C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A1290_to_800A134C.s.o .text 0xbc --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A1384_to_800A179C.s.o asm/us/main_800A1384_to_800A179C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A1384_to_800A179C.s.o .text 0x418 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A18D0_to_800A1A28.s.o asm/us/main_800A18D0_to_800A1A28.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A18D0_to_800A1A28.s.o .text 0x158 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800A1B44_to_800A2DFC.s.o asm/us/main_800A1B44_to_800A2DFC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800A1B44_to_800A2DFC.s.o .text 0x12b8 --alignment 4
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
    -o build/us/asm/us/main_800B9CAC_to_800BD880.s.o asm/us/main_800B9CAC_to_800BD880.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800B9CAC_to_800BD880.s.o .text 0x3bd4 --alignment 4
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
    -o build/us/asm/us/main_800C0850_to_800C0A64.s.o asm/us/main_800C0850_to_800C0A64.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C0850_to_800C0A64.s.o .text 0x214 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C0A6C_to_800C0C18.s.o asm/us/main_800C0A6C_to_800C0C18.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C0A6C_to_800C0C18.s.o .text 0x1ac --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C0C38_to_800C1124.s.o asm/us/main_800C0C38_to_800C1124.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C0C38_to_800C1124.s.o .text 0x4ec --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C11B8_to_800C1468.s.o asm/us/main_800C11B8_to_800C1468.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C11B8_to_800C1468.s.o .text 0x2b0 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C1484_to_800C1578.s.o asm/us/main_800C1484_to_800C1578.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C1484_to_800C1578.s.o .text 0xf4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C16B0_to_800C17C8.s.o asm/us/main_800C16B0_to_800C17C8.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C16B0_to_800C17C8.s.o .text 0x118 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C180C_to_800C247C.s.o asm/us/main_800C180C_to_800C247C.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C180C_to_800C247C.s.o .text 0xc70 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C2528_to_800C27EC.s.o asm/us/main_800C2528_to_800C27EC.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C2528_to_800C27EC.s.o .text 0x2c4 --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C2924_to_800C30A0.s.o asm/us/main_800C2924_to_800C30A0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C2924_to_800C30A0.s.o .text 0x77c --alignment 4
"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \
    -o build/us/asm/us/main_800C30B4_to_801029D0.s.o asm/us/main_800C30B4_to_801029D0.s
python3 tools/trim_elf32_section.py \
    build/us/asm/us/main_800C30B4_to_801029D0.s.o .text 0x3f91c --alignment 4
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
for unit in render_submit asset_load video_mode texture_tile small_state \
            small_state_copy collision_noop state_noop state_modes session_queries \
            random_integer vector2 vector2_motion matrix_basic \
            matrix_transform vector2_rotate matrix_vector matrix_multiply \
            scheduler_context scheduler_state scheduler_events \
            scheduler_queue scheduler_misc object_timing object_setters \
            object_init object_reset object_flags object_limit \
            object_table_color object_table_reset object_table_lookup \
            gameplay_stub turn_adjust hud_state hud_modes hud_stub \
            hud_secondary hud_primary_modes hud_transition registry_lookup \
            hud_ready hud_clear hud_navigation player_color entry_scan \
            entry_flags hud_callbacks hud_panel_callback hud_list_callback \
            hud_root_callback hud_layout hud_list_trigger \
            hud_entry_values hud_list_reset hud_selection_apply \
            hud_primary_trigger display_registry display_color display_commands; do
    .toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \
        -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude \
        -o "build/us/src/code/${unit}.raw.s" "src/code/${unit}.c"
    python3 tools/normalize_kmc_gcc_asm.py \
        "build/us/src/code/${unit}.raw.s" "build/us/src/code/${unit}.s"
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \
        -o "build/us/src/code/${unit}.c.o" "build/us/src/code/${unit}.s"
done
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
    build/us/src/code/render_submit.c.o \
    build/us/asm/us/main_8007B8EC_to_8007BCF0.s.o \
    build/us/src/code/asset_load.c.o \
    build/us/src/code/video_mode.c.o \
    build/us/src/code/texture_tile.c.o \
    build/us/asm/us/main_8007C364_to_8007D470.s.o \
    build/us/src/code/small_state.c.o \
    build/us/asm/us/main_8007D500_to_8007D558.s.o \
    build/us/src/code/small_state_copy.c.o \
    build/us/asm/us/main_8007D5B0_to_8007D694.s.o \
    build/us/src/code/collision_noop.c.o \
    build/us/asm/us/main_8007D69C_to_8007D710.s.o \
    build/us/src/code/state_noop.c.o \
    build/us/asm/us/main_8007D718_to_8007D720.s.o \
    build/us/src/code/state_modes.c.o \
    build/us/asm/us/main_8007D7C4_to_8009C284.s.o \
    build/us/src/code/session_queries.c.o \
    build/us/asm/us/main_8009C31C_to_8009D914.s.o \
    build/us/src/code/random_integer.c.o \
    build/us/asm/us/main_8009D960_to_8009DA34.s.o \
    build/us/src/code/vector2.c.o \
    build/us/asm/us/main_8009DAB0_to_8009E044.s.o \
    build/us/src/code/vector2_motion.c.o \
    build/us/asm/us/main_8009E0E8_to_8009EEE0.s.o \
    build/us/src/code/matrix_basic.c.o \
    build/us/asm/us/main_8009EF4C_to_8009F1F4.s.o \
    build/us/src/code/matrix_vector.c.o \
    build/us/asm/us/main_8009F334_to_8009F4B4.s.o \
    build/us/src/code/matrix_multiply.c.o \
    build/us/asm/us/main_8009F5AC_to_8009F768.s.o \
    build/us/src/code/matrix_transform.c.o \
    build/us/asm/us/main_8009F8A0_to_8009FF1C.s.o \
    build/us/src/code/vector2_rotate.c.o \
    build/us/asm/us/main_8009FFB8_to_800A1280.s.o \
    build/us/src/code/scheduler_context.c.o \
    build/us/asm/us/main_800A1290_to_800A134C.s.o \
    build/us/src/code/scheduler_state.c.o \
    build/us/asm/us/main_800A1384_to_800A179C.s.o \
    build/us/src/code/scheduler_events.c.o \
    build/us/asm/us/main_800A18D0_to_800A1A28.s.o \
    build/us/src/code/scheduler_queue.c.o \
    build/us/asm/us/main_800A1B44_to_800A2DFC.s.o \
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
    build/us/asm/us/main_800B9CAC_to_800BD880.s.o \
    build/us/src/code/entry_scan.c.o \
    build/us/src/code/entry_flags.c.o \
    build/us/asm/us/main_800BD93C_to_800BFD40.s.o \
    build/us/src/code/hud_state.c.o \
    build/us/asm/us/main_800BFDA4_to_800C03F0.s.o \
    build/us/src/code/hud_root_callback.c.o \
    build/us/asm/us/main_800C041C_to_800C0800.s.o \
    build/us/src/code/hud_modes.c.o \
    build/us/asm/us/main_800C0850_to_800C0A64.s.o \
    build/us/src/code/hud_stub.c.o \
    build/us/asm/us/main_800C0A6C_to_800C0C18.s.o \
    build/us/src/code/hud_layout.c.o \
    build/us/asm/us/main_800C0C38_to_800C1124.s.o \
    build/us/src/code/hud_secondary.c.o \
    build/us/src/code/hud_callbacks.c.o \
    build/us/src/code/hud_primary_modes.c.o \
    build/us/asm/us/main_800C11B8_to_800C1468.s.o \
    build/us/src/code/hud_transition.c.o \
    build/us/asm/us/main_800C1484_to_800C1578.s.o \
    build/us/src/code/hud_panel_callback.c.o \
    build/us/src/code/hud_list_trigger.c.o \
    build/us/src/code/hud_entry_values.c.o \
    build/us/src/code/hud_list_reset.c.o \
    build/us/asm/us/main_800C16B0_to_800C17C8.s.o \
    build/us/src/code/registry_lookup.c.o \
    build/us/asm/us/main_800C180C_to_800C247C.s.o \
    build/us/src/code/hud_ready.c.o \
    build/us/src/code/hud_clear.c.o \
    build/us/src/code/hud_list_callback.c.o \
    build/us/asm/us/main_800C2528_to_800C27EC.s.o \
    build/us/src/code/hud_navigation.c.o \
    build/us/src/code/hud_selection_apply.c.o \
    build/us/asm/us/main_800C2924_to_800C30A0.s.o \
    build/us/src/code/hud_primary_trigger.c.o \
    build/us/asm/us/main_800C30B4_to_801029D0.s.o \
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
