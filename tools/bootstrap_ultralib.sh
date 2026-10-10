#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
install_dir="$root_dir/.toolchain/ultralib"
revision="e24c836796df4bf520ff8b11a5c9d2cea3a66cbd"

if [[ -f "$install_dir/include/PR/R4300.h" ]] &&
   [[ "$(git -C "$install_dir" rev-parse HEAD 2>/dev/null || true)" == "$revision" ]]; then
    echo "Local pinned ultralib sources already installed"
    exit 0
fi

command -v git >/dev/null 2>&1 || {
    echo "git is required to install the pinned ultralib sources" >&2
    exit 1
}

rm -rf "$install_dir"
mkdir -p "$install_dir"
git -C "$install_dir" init -q
git -C "$install_dir" remote add origin https://github.com/decompals/ultralib.git
git -C "$install_dir" fetch -q --depth 1 origin "$revision"
git -C "$install_dir" checkout -q --detach FETCH_HEAD
echo "Installed pinned ultralib sources under $install_dir"
