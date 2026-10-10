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

The current denominator is therefore 118,820 bytes: the 749,104-byte loaded
image minus 630,284 non-overlapping catalogued function bytes. Source-owned
`.data` and `.rodata` currently account for 74,112 bytes, or 62.373%.

The effect-definition bank at `0x80114F10–0x80116580` accounts for 5,744 of
those bytes. It is kept as heterogeneous 32-bit record words until the matched
effect interpreter establishes every variant's field layout. Words containing
addresses are N64 ILP32 pointer tokens, not native host pointers.

Adjacent reconstructed units preserve the effect runtime palettes, scaling and
dispatch tables, gameplay RNG state and dispatch table, and initialized
frontend/HUD layout state. Unresolved code-address fields use the same explicit
32-bit token convention.

Campaign text at `0x80123E40–0x80124B30` is represented as a fixed-layout
structure containing 23 location labels, 26 objective strings, and 19 mission
briefings. Field widths retain the retail padding between independently
addressed strings.

The adjacent campaign configuration at `0x80124B30–0x80125580` contains 19
fixed-size `0x70`-byte mission records, a mission lookup table, 15 cartridge
asset-range records, a 34-entry mission-selection table, and the mission-code
alphabet. Address fields remain explicit 32-bit N64 ROM or RAM tokens rather
than native host pointers.

Projectile and particle data at `0x80125580–0x80125978` now preserves the
model and effect identifiers, motion parameters, interpolation curve, rotated
texture coordinates, four billboard vertex sets, and their F3DEX draw and
texture-setup display lists. Display-list and asset addresses are retained as
32-bit N64 tokens.

The following `0x80125978–0x80125C30` block contains a second billboard's
vertices and render state plus three mutable hazard-animation display lists.
The lists retain the retail F3DEX command words while exposing their role and
unit boundaries in source.

Model setup data at `0x80125C30–0x80125EC0` contains eleven fixed 24-byte
motion presets, four selectable F3DEX material lists, their pointer table, and
the initialized model-cache state. Nintendo's source-backed F3DEX FIFO 2.07
DMEM image occupies `0x80125EC0–0x801262E0`; its matrices, render state, light
buffers, dispatch tables, clipping state, and overlay metadata are kept in a
separate unit from the game-owned model data.

The source-built RSP boot and F3DEX FIFO 2.07 instruction images occupy
`0x800F8DB0–0x800FA210`. Although these bytes execute on the RSP, decomp.dev
accounts for them as Data because they are not R4300 CPU functions. Their
assembly sources and instruction-set architecture remain distinct from the
game's big-endian MIPS III/o32 CPU code.

Audio runtime data at `0x801262E0–0x80126890` contains the N_audio RSP command
dispatch addresses, vector constants and resampling coefficients, followed by
the libmus command table, six effect-parameter presets, scheduler callbacks,
and initialized audio-thread state. The arrays and pointers are represented as
typed source data; the unit does not embed a ROM-extracted binary.

The initialized-data reconstruction includes the complete cheat-code unit at
`0x80121A00`: 33 input strings, their pointer table, and the associated result
labels. It is represented as one typed C structure so the KMC compiler and
linker reproduce the original padding and relocated pointers.

The 996-byte Controller Pak message pool at `0x8011F298` is likewise expressed
as fixed-size fields in a typed C structure. This preserves the original text,
newlines, field boundaries, and alignment used by the save-game menu.

Other reconstructed text units cover the title and legal notices, PAL-system
warning, results-screen labels, and ending/rating labels. Each remains mutable
initialized data, matching how the original game updates some labels in place.
The ending results scene is represented as its score-source tokens, 23 typed
front-end element records, and root-node links rather than an opaque byte dump.
Menu-help, controls-help, controller-binding, code-entry, campaign-map,
team-setup, and missing-controller messages are also reconstructed as
fixed-layout text structures.
Additional fixed-layout units cover the main and options menus, code-entry
keys, gang and team selection, mission results, pause, Rumble Pak prompts,
and tank selection.

The main-menu scene graph at `0x80118054–0x8011854C` is reconstructed as 318
big-endian 32-bit tokens. It combines packed front-end commands, coordinates,
floating-point fields, callback addresses, and links among graph records. Its
address-valued fields remain N64 ILP32 tokens rather than native host pointers.

Gameplay and camera defaults at `0x80121CC0–0x80122384` preserve the initialized
HUD/audio/loader state, fourteen front-end record pointers, script-particle
tables, and two fifteen-record camera-preset banks. Float fields retain their
retail bit patterns and address fields remain explicit N64 tokens.

The adjacent `0x80122384–0x80123E40` definition bank contains the twelve
effect-code records, four rotating decode records, the code alphabet and lookup
tables, and the heterogeneous definitions consumed by weapon, vehicle, world
object, effect, and pickup code. Major consumer-proven boundaries are explicit
in source; variant fields remain word-exact until their individual interpreters
establish stronger types.

The results-screen data at `0x801201E0–0x801217BC` contains shared scene-graph
roots, three mutable player-result slots, single-player and split-screen panel
graphs, four menu layouts, summary widgets, and seven results-page layouts.
Consumer-known boundaries are preserved, and callback and graph-link fields
remain explicit N64 address tokens.

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

The structured extractor validates and emits the fixed records for all 75 GA
worlds: 90 groups, 43,939 placements, 4,635 models, 4,718 parts, and 39,103
pool references. It also identifies 5,372 reachable object-definition records
by their file-relative offsets and kind bytes. Of those, 495 are reachable only
through kind-39 conditional indirections rather than directly from placements;
the parser follows and validates that graph. Kind-specific payloads remain
binary until the corresponding loader switch establishes each layout.

```sh
python3 tools/extract_lzari_worlds.py assets/extracted/us/worlds
```

Each ignored output directory contains `world.json` plus the seven original
decoded components. Keeping those components unchanged means they can already
be passed to `pack_lzari_bundle.py` for an exact rebuild while the structured
formats are progressively named.

The world loader's two 17-command render-state replacement tables are also
reconstructed as typed initialized data at `0x80116710`. These are the exact
original/replacement command pairs used when state chunks are relocated.

Four baseline renderer display lists at `0x80114520` are reconstructed as
typed command arrays. They establish the opaque, textured, translucent, and
modulated RDP/RSP states selected by the renderer.

The 26-entry compressed resource-bank directory at `0x80114710` is represented
as address/size records. Its addresses form a contiguous chain through the
cartridge resource region, providing an independent boundary check for every
listed bank.

The complete ROM organization and structure names were cross-checked against
[`nviewer` revision
`700432e9bf368caebbe3150e86e987e246c851a8`](https://github.com/DSLL32/nviewer/tree/700432e9bf368caebbe3150e86e987e246c851a8).
That repository does not publish a license, so no source code was copied; the
format facts were independently validated against this ROM and the matching
game loader.

### Image textures

The image descriptor table contains 195 populated entries. Its format and size
fields use the standard N64 `G_IM_FMT_*` and `G_IM_SIZ_*` values. All decoded
payloads fall into six layouts, and their byte lengths agree with the table's
dimensions:

The complete 196-entry table (including one descriptor with no ROM payload) is
reconstructed as typed initialized data at `0x80116860`. Its 5,488 bytes include
the two runtime pointers initially set to null and the exact cartridge start/end
tokens consumed by the matching loader.

| Format | Size | Decoded layout |
| --- | --- | --- |
| RGBA | 16-bit | Big-endian RGBA5551 pixels |
| RGBA | 32-bit | RGBA8888 pixels |
| CI | 4-bit | 16-entry RGBA5551 palette followed by packed indices |
| CI | 8-bit | 256-entry RGBA5551 palette followed by byte indices |
| IA | 8-bit | Four-bit intensity and four-bit alpha |
| IA | 16-bit | Eight-bit intensity and eight-bit alpha |

One CI4 record stores one additional complete texture row beyond its visible
descriptor height. The extractor validates the stored row stride and crops the
preview to the descriptor dimensions; it does not discard or rewrite the raw
decoded payload.

Generate PNG previews and a provenance manifest in the ignored extraction tree
without installing an external texture converter:

```sh
python3 tools/extract_lzari_images.py assets/extracted/us/images
```

The PNGs are inspection artifacts. The compressed stream and its decoded N64
payload remain the byte-exact reconstruction inputs, preserving palette order,
unused entries, and storage padding that a generic PNG round trip could lose.

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

## libmus audio store

The 26-entry directory at `0x80114710` covers the cartridge range
`0x58FAE0–0x7BE9AE`, excluding at most six alignment bytes between entries.
The 2,289,292 indexed bytes contain two `N64 PtrTablesV2` pointer banks, two
`N64 WaveTables` sample banks, one effect bank, and 21 version-`0x215` song
files. This title uses Software Creations' libmus format rather than MIDI or
libultra `.ctl`/`.tbl` banks.

The SFX pointer bank describes 76 waves and the effect bank contains 93 effect
programs. The music pointer bank describes 231 waves; the 21 songs reference
171 of them, leaving 60 music-bank waves unused by the songs. Every wave record
is checked against its sample bank, including its ADPCM predictor book, loop
record, base note, and detune field. Every song's channel, volume, pitch-bend,
envelope, drum, wave, and master-track offsets are checked against its bounds.

Generate a metadata-only inventory without writing audio payloads:

```sh
python3 tools/inventory_libmus_assets.py \
  --output /tmp/btga-libmus-assets.json
```

The inventory records retail offsets, sizes, SHA-256 values, and structural
metadata. Lossless source repacking remains future work; decoded WAV files are
inspection artifacts and must stay untracked.
