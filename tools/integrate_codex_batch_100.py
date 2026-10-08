#!/usr/bin/env python3
"""Integrate the verified Codex 100-function manifest into splat/build inputs."""

from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "config/us/codex_batch_100.tsv"
SPLAT = ROOT / "config/us/splat.yaml"
BUILD = ROOT / "tools/build_code.sh"
VRAM_BASE = 0x80070000


def functions():
    result = []
    for line in MANIFEST.read_text().splitlines():
        if not line or line.startswith("#") or line.startswith("function"):
            continue
        name, address, size, _ = line.split("\t")
        result.append((int(address, 0) - VRAM_BASE, int(size, 0), name))
    return sorted(result)


def rewrite_splat(funcs):
    lines = SPLAT.read_text().splitlines()
    entry_re = re.compile(
        r"^(\s{6})- \[(0x[0-9A-Fa-f]+), ([^,\]]+)(?:, ([^\]]+))?\]$"
    )
    entries = []
    indices = []
    in_main = False
    in_subsegments = False
    for index, line in enumerate(lines):
        if line == "  - name: main":
            in_main = True
        elif in_main and line.startswith("  - name:"):
            break
        elif in_main and line == "    subsegments:":
            in_subsegments = True
        elif in_subsegments:
            match = entry_re.match(line)
            if match:
                entries.append((int(match.group(2), 0), match.group(3), match.group(4)))
                indices.append(index)

    if len(funcs) != 100 or not entries:
        raise SystemExit("unexpected manifest or splat layout")

    selected = {start: (size, name) for start, size, name in funcs}
    breaks = {start for start, _, _ in entries}
    for start, size, _ in funcs:
        breaks.add(start)
        breaks.add(start + size)
    ordered_breaks = sorted(breaks)

    def original_at(offset):
        current = None
        for entry in entries:
            if entry[0] > offset:
                break
            current = entry
        return current

    output = []
    for pos, start in enumerate(ordered_breaks):
        original = original_at(start)
        if original is None:
            continue
        # This final assembly subsegment intentionally consumes the remainder
        # of the main segment; it has no following break from which to derive a
        # finite range name.
        if start == 0xA18D0:
            output.append("      - [0xA18D0, asm, main_after_801118C0]")
            continue
        if start in selected:
            _, name = selected[start]
            output.append(f"      - [0x{start:X}, c, code/codex_batch/{name}]")
            continue
        # A selected unit ending inside an assembly range starts a new gap.
        prior_selected_end = any(s + z == start for s, z, _ in funcs)
        if original[1] == "asm" and (start == original[0] or prior_selected_end):
            end = ordered_breaks[pos + 1] if pos + 1 < len(ordered_breaks) else start
            output.append(
                f"      - [0x{start:X}, asm, main_{VRAM_BASE + start:08X}_to_{VRAM_BASE + end:08X}]"
            )
        elif start == original[0]:
            suffix = f", {original[2]}" if original[2] else ""
            output.append(f"      - [0x{start:X}, {original[1]}{suffix}]")

    first, last = indices[0], indices[-1]
    lines[first:last + 1] = output
    SPLAT.write_text("\n".join(lines) + "\n")


def rewrite_build():
    text = BUILD.read_text()
    asm_start = text.index('"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \\\n    -o build/us/asm/us/header.s.o asm/us/header.s')
    asm_end_marker = ('"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \\\n'
                      '    -o build/us/asm/us/main_after_801118C0.s.o asm/us/main_after_801118C0.s')
    asm_end = text.index(asm_end_marker, asm_start) + len(asm_end_marker)
    generic_asm = '''"${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \\
    -o build/us/asm/us/header.s.o asm/us/header.s
for asm_source in asm/us/*.s; do
    asm_name="$(basename "$asm_source" .s)"
    asm_object="build/us/asm/us/${asm_name}.s.o"
    "${tool_prefix}as" -EB -march=vr4300 -mabi=32 -I include \\
        -o "$asm_object" "$asm_source"
    if [[ "$asm_name" =~ ^main_([0-9A-Fa-f]{8})_to_([0-9A-Fa-f]{8})$ ]]; then
        range_start=$((16#${BASH_REMATCH[1]}))
        range_end=$((16#${BASH_REMATCH[2]}))
        python3 tools/trim_elf32_section.py "$asm_object" .text \\
            "$((range_end - range_start))" --alignment 4
    fi
done'''
    text = text[:asm_start] + generic_asm + text[asm_end:]

    compile_anchor = 'done\npython3 tools/trim_elf32_section.py \\\n    build/us/src/code/early_hw.c.o'
    batch_compile = '''done
while IFS=$'\\t' read -r function_name address size status; do
    case "$function_name" in
        \\#*|function|'') continue ;;
    esac
    unit="code/codex_batch/${function_name}"
    mkdir -p "build/us/src/code/codex_batch"
    .toolchain/kmc-gcc-2.7.2/gcc -B.toolchain/kmc-gcc-2.7.2/ -S \\
        -O2 -G0 -mips3 -mgp32 -mfp32 -Iinclude \\
        -o "build/us/src/${unit}.raw.s" "src/${unit}.c"
    python3 tools/normalize_kmc_gcc_asm.py \\
        "build/us/src/${unit}.raw.s" "build/us/src/${unit}.s"
    .toolchain/kmc-gcc-2.7.2/as -mips3 -G0 \\
        -o "build/us/src/${unit}.c.o" "build/us/src/${unit}.s"
    python3 tools/trim_elf32_section.py \\
        "build/us/src/${unit}.c.o" .text "$size" --alignment 4
done < config/us/codex_batch_100.tsv
python3 tools/trim_elf32_section.py \\
    build/us/src/code/early_hw.c.o'''
    if compile_anchor not in text:
        raise SystemExit("compile anchor not found")
    text = text.replace(compile_anchor, batch_compile, 1)

    nm_start = text.index('"${tool_prefix}nm" -u \\\n')
    nm_end_marker = '    > build/us/undefined_object_symbols.txt'
    nm_end = text.index(nm_end_marker, nm_start) + len(nm_end_marker)
    generic_nm = '''mapfile -t object_files < <(find build/us -type f -name '*.o' -print | sort)
"${tool_prefix}nm" -u "${object_files[@]}" \\
    > build/us/undefined_object_symbols.txt'''
    text = text[:nm_start] + generic_nm + text[nm_end:]
    BUILD.write_text(text)


def main():
    funcs = functions()
    rewrite_splat(funcs)
    rewrite_build()


if __name__ == "__main__":
    main()
