#!/usr/bin/env python3
"""Rebuild the contiguous libmus store from its independently split files."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

from inventory_libmus_assets import AudioInventoryError, validate_payloads


class AudioPackError(ValueError):
    """The component set cannot reproduce the inventoried libmus store."""


def _read_component(path: Path, item: dict, require_original: bool) -> bytes:
    try:
        payload = path.read_bytes()
    except FileNotFoundError as exc:
        raise AudioPackError(f"missing component: {path}") from exc
    if len(payload) != item["size"]:
        raise AudioPackError(
            f"{path.name} is {len(payload)} bytes; expected {item['size']}"
        )
    if require_original and hashlib.sha256(payload).hexdigest() != item["sha256"]:
        raise AudioPackError(f"{path.name} does not match the retail hash")
    return payload


def _rebuild_sample_bank(source: Path, bank: dict,
                         require_original: bool) -> bytes:
    parts = []
    cursor = 0
    for item in sorted(bank["components"], key=lambda entry: entry["start"]):
        if item["start"] != cursor:
            relation = "overlap" if item["start"] < cursor else "uncovered range"
            raise AudioPackError(
                f"{bank['name']} sample component creates an {relation} at 0x{cursor:X}"
            )
        path = source / item["path"]
        payload = _read_component(path, item, require_original)
        if item["end"] - item["start"] != len(payload):
            raise AudioPackError(f"{path.name} has inconsistent manifest bounds")
        parts.append(payload)
        cursor = item["end"]
    if cursor != bank["size"]:
        raise AudioPackError(
            f"{bank['name']} sample bank ends at 0x{cursor:X}, expected 0x{bank['size']:X}"
        )
    return b"".join(parts)


def rebuild(manifest: dict, source: Path, require_original: bool = False) -> bytes:
    files = manifest.get("files", [])
    gaps = manifest.get("gaps", [])
    if len(files) != manifest.get("file_count") or not files:
        raise AudioPackError("manifest has an invalid file table")
    start = manifest["store_start"]
    end = manifest["store_end"]
    if end <= start or end - start != manifest.get("store_span_bytes"):
        raise AudioPackError("manifest has an invalid store span")

    parts: list[tuple[int, int, bytes, str]] = []
    payloads = []
    semantic_banks = {
        item["file_index"]: item for item in manifest.get("sample_banks", [])
    }
    for expected_index, item in enumerate(files):
        if item.get("index") != expected_index:
            raise AudioPackError("manifest file indices are not contiguous")
        if expected_index in semantic_banks:
            payload = _rebuild_sample_bank(
                source, semantic_banks[expected_index], require_original
            )
            if len(payload) != item["size"]:
                raise AudioPackError(
                    f"semantic file {expected_index} is {len(payload)} bytes; "
                    f"expected {item['size']}"
                )
            if (require_original and
                    hashlib.sha256(payload).hexdigest() != item["sha256"]):
                raise AudioPackError(
                    f"semantic file {expected_index} does not match the retail hash"
                )
        else:
            payload = _read_component(
                source / f"file_{expected_index:02d}.bin", item, require_original
            )
        payloads.append(payload)
        parts.append((item["start"], item["end"], payload,
                      f"file {expected_index}"))
    for item in gaps:
        payload = _read_component(
            source / "gaps" / f"gap_{item['index']:02d}.bin",
            item,
            require_original,
        )
        parts.append((item["start"], item["end"], payload,
                      f"gap {item['index']}"))

    parts.sort(key=lambda part: part[0])
    cursor = start
    result = bytearray()
    for part_start, part_end, payload, name in parts:
        if part_start != cursor:
            relation = "overlap" if part_start < cursor else "uncovered range"
            raise AudioPackError(f"{name} creates an {relation} at 0x{cursor:X}")
        if part_end - part_start != len(payload):
            raise AudioPackError(f"{name} has inconsistent manifest bounds")
        result.extend(payload)
        cursor = part_end
    if cursor != end:
        raise AudioPackError(f"store ends at 0x{cursor:X}, expected 0x{end:X}")

    try:
        validate_payloads(payloads)
    except AudioInventoryError as exc:
        raise AudioPackError(f"rebuilt libmus structures are invalid: {exc}") from exc
    return bytes(result)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--source", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--require-original", action="store_true")
    args = parser.parse_args()
    manifest = json.loads(args.manifest.read_text())
    rebuilt = rebuild(manifest, args.source, args.require_original)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(rebuilt)
    print(f"Rebuilt {len(manifest['files'])} libmus files, {len(rebuilt)} bytes")


if __name__ == "__main__":
    main()
