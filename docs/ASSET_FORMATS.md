# ROM asset formats

This document records only layout properties established by matching game code
and cross-stream validation. Semantic names will be added after the consuming
code establishes them; extracted bytes alone are not counted as decompiled
Data.

## Progress accounting scope

decomp.dev's Data category covers initialized non-function bytes in the loaded
main executable image, ROM `0x001000–0x0B7E30`. It excludes the cartridge asset
store, the raw RSP payload beginning at `0x0B7E30`, stale-build material, BSS,
and terminal ROM padding. Those excluded regions still need documented formats
and exact reconstruction for the project, but adding their bytes to the Data
denominator would conflate asset extraction with linked program-data matching.

The current denominator is therefore 118,560 bytes: the 749,104-byte loaded
image minus 630,544 non-overlapping catalogued function bytes. Source-owned
`.data` and `.rodata` currently account for 20,040 bytes, or 16.903%.

The initialized-data reconstruction includes the complete cheat-code unit at
`0x80121A00`: 33 input strings, their pointer table, and the associated result
labels. It is represented as one typed C structure so the KMC compiler and
linker reproduce the original padding and relocated pointers.

## LZARI bundles

The matching decoder at `0x800A0750` is used by the level, common-world, and
image loaders. The audited ROM tables identify 271 streams: 74 level worlds,
one common world, 195 images, and one unreferenced world left over from the
first BattleTanx. The inventory independently decodes and canonically
re-encodes their 1,174,975 stored bytes into 2,886,765 decoded bytes. Image
records store an inclusive cart end and their
loader rounds the transfer down to an even byte count; where that adds a zero
DMA-alignment byte, the repacker restores it after the canonical LZARI payload.

Every decoded GA world stream begins with eight big-endian 32-bit offsets. The first is
always `0x20`, offsets never decrease, and the eighth equals the decoded size.
They delimit seven components:

| Component | Proven layout |
| --- | --- |
| 0 | One big-endian `u32` group count |
| 1 | 16-byte groups: placement count, first placement, and six signed bounds |
| 2 | 12-byte placements: XYZ, yaw, and an object-definition offset |
| 3 | Variable-size object definitions, dispatched by kind byte |
| 4 | 16-byte models: part count, first part, and six signed bounds |
| 5 | 4-byte parts: pool-reference count and first pool reference |
| 6 | 24-byte pool references: offsets and sizes in GEO, PB, and TEX pools |

The widths and relationships are established by the matching loader and are
checked against every decoded world. Each group range must stay within the
placement array, each model's part range within the part array, and each
part's pool-reference range within the pool-reference array. Object
definitions remain variable length and are therefore recorded as bytes rather
than misreported as fixed-width records.

The complete ROM organization and structure names were cross-checked against
[`nviewer` revision
`700432e9bf368caebbe3150e86e987e246c851a8`](https://github.com/DSLL32/nviewer/tree/700432e9bf368caebbe3150e86e987e246c851a8).
That repository does not publish a license, so no source code was copied; the
format facts were independently validated against this ROM and the matching
game loader.

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
declared ROM range, validates world-bundle invariants, and records decoded
SHA-256 values. Its encoder uses the original 1989 LZARI match parser and
arithmetic coder with the game's big-endian size header. Re-encoding all 271
known streams, including explicit image DMA padding, reproduces every stored
byte exactly; this is also a test gate. ROM-derived output stays untracked.

After editing the seven extracted components, rebuild their offset table and
compressed stream with:

```sh
python3 tools/pack_lzari_bundle.py \
  bundle.part0.bin bundle.part1.bin bundle.part2.bin bundle.part3.bin \
  bundle.part4.bin bundle.part5.bin bundle.part6.bin bundle.lzari
```

The packer rejects component sets that violate the loader-derived record and
alignment rules.
