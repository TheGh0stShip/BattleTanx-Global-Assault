#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
install_dir="$root_dir/.toolchain/f3dex2"
revision="bd31393fd02b89e024b043c8c50c9a5a10143fb6"
archive_url="https://github.com/Mr-Wiseguy/f3dex2/archive/$revision.tar.gz"
archive_sha256="1dd133735f8f1793e124a18ee912c4f3cdf7c8cdda8d047b88485203507ca026"

if [[ -f "$install_dir/f3dex2.s" && -f "$install_dir/.revision" ]] &&
   [[ "$(cat "$install_dir/.revision")" == "$revision" ]]; then
    echo "Local pinned F3DEX2 sources already installed"
    exit 0
fi

for command_name in curl sha256sum tar; do
    command -v "$command_name" >/dev/null 2>&1 || {
        echo "$command_name is required to install the F3DEX2 sources" >&2
        exit 1
    }
done

download_dir="$(mktemp -d)"
archive="$download_dir/f3dex2.tar.gz"
source_dir="$download_dir/source"
trap 'rm -rf "$download_dir"' EXIT

curl -L --fail --silent --show-error "$archive_url" -o "$archive"
echo "$archive_sha256  $archive" | sha256sum --check --status || {
    echo "F3DEX2 source archive checksum mismatch" >&2
    exit 1
}
mkdir -p "$source_dir"
tar -xzf "$archive" -C "$source_dir" --strip-components=1
rm -rf "$install_dir"
mkdir -p "$(dirname "$install_dir")"
mv "$source_dir" "$install_dir"
printf '%s\n' "$revision" > "$install_dir/.revision"
echo "Installed pinned F3DEX2 sources under $install_dir"
