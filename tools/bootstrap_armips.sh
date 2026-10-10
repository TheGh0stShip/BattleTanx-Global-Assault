#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
install_dir="$root_dir/.toolchain/armips"
archive_url="https://github.com/Kingcom/armips/archive/62adab4ef30da765f5cf22a451eb08a59c54dc8b.tar.gz"
archive_sha256="f7a65102febb4811a336a06e6d93c7e84cc86548182d54e1b1b4e2bffe562f56"
filesystem_url="https://github.com/Kingcom/filesystem/archive/3f1c185ab414e764c694b8171d1c4d8c5c437517.tar.gz"
filesystem_sha256="cc1c5439d29031477868bdea25b68b4210dc8ed018bb63a65e708afe405db4e1"

if [[ -x "$install_dir/armips" ]]; then
    echo "Local pinned armips already installed"
    exit 0
fi

for command_name in cmake curl sha256sum tar; do
    command -v "$command_name" >/dev/null 2>&1 || {
        echo "$command_name is required to install armips" >&2
        exit 1
    }
done

download_dir="$(mktemp -d)"
archive="$download_dir/armips.tar.gz"
filesystem_archive="$download_dir/filesystem.tar.gz"
source_dir="$download_dir/source"
build_dir="$download_dir/build"
trap 'rm -rf "$download_dir"' EXIT

curl -L --fail --silent --show-error "$archive_url" -o "$archive"
curl -L --fail --silent --show-error "$filesystem_url" \
    -o "$filesystem_archive"
echo "$archive_sha256  $archive" | sha256sum --check --status || {
    echo "armips source archive checksum mismatch" >&2
    exit 1
}
echo "$filesystem_sha256  $filesystem_archive" \
    | sha256sum --check --status || {
        echo "armips filesystem dependency checksum mismatch" >&2
        exit 1
    }
mkdir -p "$source_dir" "$build_dir" "$install_dir"
tar -xzf "$archive" -C "$source_dir" --strip-components=1
mkdir -p "$source_dir/ext/filesystem"
tar -xzf "$filesystem_archive" -C "$source_dir/ext/filesystem" \
    --strip-components=1
cmake -S "$source_dir" -B "$build_dir" -DCMAKE_BUILD_TYPE=Release \
    -DWITH_TESTS=OFF >/dev/null
cmake --build "$build_dir" --parallel >/dev/null
cp "$build_dir/armips" "$install_dir/armips"
chmod +x "$install_dir/armips"
echo "Installed pinned armips under $install_dir"
