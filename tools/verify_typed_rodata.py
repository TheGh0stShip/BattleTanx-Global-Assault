#!/usr/bin/env python3
"""Compile and ROM-compare all transitional typed rodata units in parallel."""

from __future__ import annotations

import argparse
import concurrent.futures
import csv
import os
from pathlib import Path
import subprocess
import sys


VRAM_ROM_DELTA = 0x80070000


def load_manifest(path: Path) -> list[tuple[str, int, int]]:
    with path.open(newline="") as stream:
        rows = csv.DictReader(stream, delimiter="\t")
        return [
            (row["unit"], int(row["address"], 0), int(row["size"], 0))
            for row in rows
            if row.get("unit") and not row["unit"].startswith("#")
        ]


def compare_unit(repo: Path, out: Path, unit: str, address: int, size: int) -> str:
    source = repo / "src/code" / f"{unit}.c"
    if not source.is_file():
        raise RuntimeError(f"{unit}: missing {source.relative_to(repo)}")
    raw = out / f"{unit}.raw.s"
    normalized = out / f"{unit}.s"
    obj = out / f"{unit}.o"
    linked = out / f"{unit}.elf"
    blob = out / f"{unit}.bin"
    commands = (
        [str(repo / ".toolchain/kmc-gcc-2.7.2/gcc"),
         "-B" + str(repo / ".toolchain/kmc-gcc-2.7.2") + "/", "-S",
         "-O2", "-G0", "-mips3", "-mgp32", "-mfp32", "-Iinclude",
         "-o", str(raw), str(source)],
        [sys.executable, "tools/normalize_kmc_gcc_asm.py", str(raw), str(normalized)],
        [str(repo / ".toolchain/kmc-gcc-2.7.2/as"), "-mips3", "-G0",
         "-o", str(obj), str(normalized)],
    )
    for command in commands:
        subprocess.run(command, cwd=repo, check=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    env = os.environ.copy()
    library = repo / ".toolchain/mips-binutils/usr/lib/x86_64-linux-gnu"
    env["LD_LIBRARY_PATH"] = str(library) + (":" + env["LD_LIBRARY_PATH"] if env.get("LD_LIBRARY_PATH") else "")
    linker_script = out / f"{unit}.ld"
    linker_script.write_text(
        f"INCLUDE {repo / 'build/us/symbols.ld'}\n"
        f"SECTIONS {{ . = 0x{address:X}; .rodata : {{ *(.rodata) }} }}\n"
    )
    subprocess.run(
        [str(repo / ".toolchain/mips-binutils/usr/bin/mips-linux-gnu-ld"),
         "-EB", "-m", "elf32btsmip", "-T", str(linker_script),
         "-o", str(linked), str(obj)],
        cwd=repo, env=env, check=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    subprocess.run(
        [str(repo / ".toolchain/mips-binutils/usr/bin/mips-linux-gnu-objcopy"),
         "-O", "binary", "--only-section=.rodata", str(linked), str(blob)],
        cwd=repo, env=env, check=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    emitted = blob.read_bytes()
    expected = (repo / "baseroms/us/baserom.z64").read_bytes()[address - VRAM_ROM_DELTA:address - VRAM_ROM_DELTA + size]
    if len(expected) != size:
        raise RuntimeError(f"{unit}: ROM range is short")
    if emitted[:size] != expected:
        first = next(i for i, pair in enumerate(zip(emitted[:size], expected)) if pair[0] != pair[1])
        raise RuntimeError(f"{unit}: byte mismatch at +0x{first:X}")
    if len(emitted) < size:
        raise RuntimeError(f"{unit}: emitted 0x{len(emitted):X}, expected 0x{size:X}")
    if any(emitted[size:]):
        raise RuntimeError(f"{unit}: nonzero bytes emitted beyond owned range")
    return f"{unit}\t0x{address:08X}\t0x{size:X}\tMATCH"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", type=Path, default=Path("config/us/typed_rodata_units.tsv"))
    parser.add_argument("--jobs", type=int, default=min(os.cpu_count() or 1, 8))
    parser.add_argument("--unit", action="append", default=[])
    args = parser.parse_args()
    repo = Path(__file__).resolve().parent.parent
    manifest = args.manifest if args.manifest.is_absolute() else repo / args.manifest
    entries = load_manifest(manifest)
    if args.unit:
        wanted = set(args.unit)
        entries = [entry for entry in entries if entry[0] in wanted]
        missing = wanted - {entry[0] for entry in entries}
        if missing:
            parser.error("unknown unit(s): " + ", ".join(sorted(missing)))
    out = repo / "build/typed-rodata-gate"
    out.mkdir(parents=True, exist_ok=True)
    failures: list[str] = []
    results: list[str] = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, args.jobs)) as pool:
        futures = {pool.submit(compare_unit, repo, out, *entry): entry[0] for entry in entries}
        for future in concurrent.futures.as_completed(futures):
            try:
                results.append(future.result())
            except Exception as exc:
                failures.append(str(exc))
    print("unit\taddress\tsize\tstatus")
    print("\n".join(sorted(results)))
    for failure in sorted(failures):
        print(f"ERROR\t{failure}", file=sys.stderr)
    print(f"verified={len(results)} failed={len(failures)}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
