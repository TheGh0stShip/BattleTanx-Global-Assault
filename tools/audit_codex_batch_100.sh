#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "$0")/.." && pwd)"
manifest="$root_dir/config/us/codex_batch_100.tsv"
source_dir="$root_dir/src/code/codex_batch"
gcc_dir="$root_dir/.toolchain/kmc-gcc-2.7.2"
matches=0
differences=0
missing=0

while IFS=$'\t' read -r function_name address size status; do
    case "$function_name" in
        \#*|function|'') continue ;;
    esac

    source_file="$source_dir/$function_name.c"
    if [[ ! -f "$source_file" ]]; then
        printf 'MISSING\t%s\n' "$function_name"
        missing=$((missing + 1))
        continue
    fi

    if "$root_dir/tools/cmpfunc_strict.sh" \
        "$source_file" "$function_name" "$address" "$size" \
        --gccdir "$gcc_dir" --asdir "$gcc_dir" --normalize \
        --asflags='-mips3 -G0' -q >/dev/null; then
        printf 'MATCH\t%s\n' "$function_name"
        matches=$((matches + 1))
    else
        printf 'DIFF\t%s\n' "$function_name"
        differences=$((differences + 1))
    fi
done < "$manifest"

printf 'SUMMARY\tmatches=%d\tdifferences=%d\tmissing=%d\ttotal=%d\n' \
    "$matches" "$differences" "$missing" "$((matches + differences + missing))"

(( differences == 0 && missing == 0 ))
