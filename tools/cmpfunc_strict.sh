#!/usr/bin/env bash
set -euo pipefail

if (( $# < 4 )); then
    echo "usage: $0 SOURCE FUNCTION VRAM SIZE [cmpfunc options...]" >&2
    exit 2
fi

root_dir="$(cd "$(dirname "$0")/.." && pwd)"
comparator="$root_dir/claude-work/output/tools/cmpfunc.py"
function_name="$2"
output_file="$(mktemp)"
trap 'rm -f "$output_file"' EXIT

# cmpfunc.py currently exits zero for both MATCH and DIFF.  Its RESULT line is
# authoritative, so convert that textual result into a useful process status.
python3 "$comparator" "$@" | tee "$output_file"

if grep -q "^RESULT ${function_name}: MATCH" "$output_file"; then
    exit 0
fi

if grep -q "^RESULT ${function_name}: DIFF" "$output_file"; then
    exit 1
fi

echo "cmpfunc produced no result for ${function_name}" >&2
exit 2
