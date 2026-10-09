# ROM asset formats

This document records only layout properties established by matching game code
and cross-stream validation. Semantic names will be added after the consuming
code establishes them; extracted bytes alone are not counted as decompiled
Data.

## LZARI bundles

`func_800E8380` selects the compressed ROM ranges and `func_800BA6C0` DMA-loads
and expands them with the matching LZARI decoder at `0x800A0750`. The currently
known boundary table identifies 74 streams. They occupy 481,785 ROM bytes and
expand to 1,570,628 bytes.

Every decoded stream begins with eight big-endian 32-bit offsets. The first is
always `0x20`, offsets never decrease, and the eighth equals the decoded size.
They delimit seven components:

| Component | Proven layout |
| --- | --- |
| 0 | One big-endian 32-bit count |
| 1 | Exactly that many 16-byte records |
| 2 | 12-byte records |
| 3 | 16-byte records followed by 0, 4, 8, or 12 tail bytes |
| 4 | 4-byte records |
| 5 | An unknown 4-byte-aligned region |
| 6 | 24-byte records |

The record widths are not guesses. At `0x800BA78C–0x800BA848`, the loader
relocates the eight offsets and derives counts from the differences using
division by 12, 16, 4, and 24. Component 0 agrees with the size of component 1
in every stream. Component 5 is relocated but its internal semantics remain
open.

Run the inventory without writing extracted data:

```sh
python3 tools/inventory_lzari_assets.py --output /tmp/btga-assets.json
```

To inspect locally, write into the ignored extraction tree:

```sh
python3 tools/inventory_lzari_assets.py \
  --output /tmp/btga-assets.json \
  --extract-dir assets/extracted/us/lzari \
  --split-bundles
```

The tool verifies the base-ROM SHA-1, requires each stream to consume its exact
declared ROM range, validates the bundle invariants above, and records decoded
SHA-256 values. Its encoder uses the original 1989 LZARI match parser and
arithmetic coder with the game's big-endian size header. Re-encoding all 74
decoded bundles reproduces all 481,785 retail compressed bytes exactly; this is
also a test gate. ROM-derived output stays untracked.

After editing the seven extracted components, rebuild their offset table and
compressed stream with:

```sh
python3 tools/pack_lzari_bundle.py \
  bundle.part0.bin bundle.part1.bin bundle.part2.bin bundle.part3.bin \
  bundle.part4.bin bundle.part5.bin bundle.part6.bin bundle.lzari
```

The packer rejects component sets that violate the loader-derived record and
alignment rules.
