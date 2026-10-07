# Library fingerprints

The USA Rev 0 ROM contains an **N64 OS 2.0I `libultra_rom`** runtime and the
**F3DEX FIFO 2.07** graphics microcode. These identifications are based on ROM
bytes, not solely on imported symbol names.

## Evidence

Reference objects were rebuilt from the primary-source
[`decompals/ultralib`](https://github.com/decompals/ultralib) project for SDK
versions I, J, K, and L with its IDO 5.3 toolchain and documented per-object
flags. Comparisons mask only linker-owned `R_MIPS_26`, `R_MIPS_HI16`, and
`R_MIPS_LO16` fields; all remaining instruction words and object padding must
agree.

| ROM function | VRAM | Size | Result |
| --- | ---: | ---: | --- |
| `osInitialize` | `0x80105B10` | `0x290` | Exact normalized match to 2.0I; J first differs at `+0x188`, K/L differ at the entry. |
| `osSetTimer` | `0x8010FA80` | `0xE0` | Exact I/J match; K/L differ at the entry. |
| `alSynAllocFX` | `0x80110E40` | `0xA0` | Exact 2.0I match; J/K/L differ at `+0x4`. |
| `__osTimerInterrupt` | `0x80110FAC` | `0x178` | Exact I/J/K/L match; corroborates the object family but does not discriminate the release. |
| `__osSetTimerIntr` | `0x80111124` | `0x74` | Exact I/J match; K/L differ at `+0x4`. |
| `__osInsertTimer` | `0x80111198` | `0x188` | Exact I/J match; K/L differ at the entry. |
| `alCopy` | `0x801037B0` | `0x80` | Exact I/J/K/L match. |
| `alFilterNew` | `0x801059B0` | `0x20` | Exact I/J/K/L match. |
| `alHeapInit` | `0x80105AD0` | `0x40` | Exact I/J/K/L match. |

The unique `osInitialize` and `alSynAllocFX` results identify 2.0I; the other
matches independently confirm that the compared ROM region is the expected
libultra/libaudio family. At ROM offset `0x000B5FF8`, the embedded microcode
identification string is:

```text
RSP Gfx ucode F3DEX       fifo 2.07  Yoshitaka Yasumoto 1998 Nintendo.
```

## Compiler scope

The exact SDK object matches prove that the linked library code is compatible
with the IDO 5.3 build and flags used by ultralib. They do **not** prove that
3DO compiled every game-owned translation unit with IDO 5.3. IDO 5.3 and 7.1
generate the same code for the current small game-function probes, so the game
compiler remains provisional until a non-library function discriminates the
versions. Library functions should use their SDK-specific optimization flags;
the global game-code flags must not be inferred from one archive member.

## Proposed regions

These are research and future split boundaries, not yet linker-layout claims:

| VRAM range | Proposed ownership | Notes |
| --- | --- | --- |
| `0x800FB244`-`0x800FFB2F` | N64 Sound Tools player (`libmus`) | `Mus*` APIs, player internals, and command handlers such as `Ftempo` and `Fstartfx`. The precise Sound Tools release is not yet proven. |
| `0x800FFB30`-`0x80101D0F` | Nintendo N_audio driver core | Starts at `n_alFxNew`; contains the N_audio mixer, ADPCM, load, resample, and buffer helpers. |
| `0x80101D10`-`0x80114497` | SDK/archive tail | N_audio lifecycle, libaudio, libultra OS/I/O, libc, and compiler-runtime objects in link order. Split this range at verified object boundaries rather than treating it as one source file. |

The earlier exception and OS fragments around `0x80078CE4` are a separate
linked group and should not be merged into the late SDK/archive region merely
because their symbols are also libultra-derived. The final verified executable
boundary currently ends at `0x80114498`.

## Controller object layout caveats

The `contquery.o` layout agrees exactly with the 2.0I reference: its
`osContStartQuery` body occupies `0x801030B0`-`0x80103133`, and
`osContGetQuery` begins at object offset `0x84`. The imported
`__osCleanupThread` name at `0x801030D4` is therefore an interior signature
false-positive, not a callable boundary. It remains an untyped seed symbol for
navigation only.

Do not force the adjacent controller objects to stock offsets. The ROM's
`contreaddata` unit spans `0x80103190`-`0x801033EF`; its read-start, read-result,
and command-packing routines begin at local offsets `0`, `0xC4`, and `0x16C`.
The imported `contreaddata_text_017C` label at `0x801032FC` marks the last of
those routines but does not carry a trustworthy SDK name. Likewise, the ROM's
`controller` unit places `osContInit` at `0x801033F0` and
`__osContGetInitData` at `0x801035E8`, which differs from both stock IDO and
GCC 2.0I layouts. Preserve these ROM-derived boundaries until their local
compiler or source variation is established.

The imported `MusPtrBankGetCurrent` boundary at `0x8010CF88` is another
interior signature false-positive. The branch at `0x8010CF78` reaches that
address when the PI device manager is active, where the code loads its command
queue and returns. Consequently `osPiGetCmdQueue` occupies
`0x8010CF70`-`0x8010CF97` (`0x28` bytes), matching its SDK behavior of returning
the command queue only for an active manager. The old music-library name is
retained as an untyped seed alias for navigation, not as a function.
