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
boundary currently ends at `0x8011449C`.

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

## PI access and scheduler boundary corrections

The contiguous unit at `0x8010D680`-`0x8010D73F` is `io/piacs.o`, not a
mixture of PI and SI access helpers. Its three routines are
`__osPiCreateAccessQueue`, `__osPiGetAccess`, and `__osPiRelAccess`, beginning
at object offsets `0`, `0x50`, and `0x94`. The latter two operate on the same
PI access-queue state initialized by the first routine. The independent
`io/siacs.o` copy is at `0x8010FB60`-`0x8010FC1F`.

The 2.0I `sched/sched.o` text occupies `0x8010ECB0`-`0x8010F5EF`. Both the
reference object and ROM contain four consecutive, independently emitted
eight-byte no-op static routines at object offsets `0x364`, `0x36C`, `0x374`,
and `0x37C` (`0x8010F014`, `0x8010F01C`, `0x8010F024`, and `0x8010F02C`). Each
is exactly `jr $ra; nop`. The imported `ptstart` label incorrectly aggregated
all four; `ptstart` belongs to the SDK initialization unit, not the scheduler.
Until the stripped static names are proven, keep address-derived names for
these four boundaries.

## Late SDK object and static-function corrections

The `libc/sprintf.o` unit spans `0x80110020`-`0x8011009F`; its static output
callback is `proutSprintf` at offset `0`, followed by `sprintf` at offset
`0x24`. The `io/sptask.o` unit spans `0x801100E0`-`0x801103CF`; its static
`_VirtualToPhysicalTask` helper occupies the first `0x11C` bytes. Likewise,
the second routine in `io/vimgr.o` at `0x801116E8` is its static `viMgrMain`,
and the standalone `io/vigetcurrcontext.o` routine at `0x801118C0` is
`__osViGetCurrentContext`.

Two imported audio boundaries previously crossed independently linked or
emitted routines. `audio/syndelete.o` is exactly
`0x80110750`-`0x8011075F`; `audio/synthesizer.o` begins at `0x80110760` with
an independent eight-byte no-op static routine before `_timeToSamples`.
Within `synthesizer.o`, `__allocParam` occupies object offsets
`0x110`-`0x13F`, followed by another independent eight-byte no-op static
routine at offset `0x140`; `alAudioFrame` begins at offset `0x148`. Thus the
ROM boundaries are `0x80110870`-`0x8011089F`, `0x801108A0`-`0x801108A7`, and
`0x801108A8` onward respectively. The stripped static names of the two no-op
routines remain unproven, so they retain address-derived names.

## Formatted I/O and compiler-runtime tail

The `libc/xprintf.o` static routine at `0x80112110` is `_Putfld`, followed by
`_Printf` at object offset `0x670`. In `libc/xldtob.o`, `_Genld` occupies
`0x80112DD0`-`0x80113337`; the independently emitted eight-byte static no-op
at `0x80113338` remains address-named, and `_Ldtob` begins at object offset
`0x570`. These layouts match the rebuilt 2.0I objects. The libc units use the
SDK I IDO `-O3 -mips2 -o32 -non_shared -G 0` configuration.

The executable tail after `osYieldThread` consists of GNU libgcc ABI helpers:
`__cmpdi2`, `__floatdisf`, `__udivdi3`, `__udivmoddi4`, and `__umoddi3`.
Their exact compiler-runtime release is not yet proven, so the names identify
their stable ABI behavior rather than a claimed GCC version. Two four-byte
data fragments interrupt the code: `0x80113E0C` lies between `__floatdisf`
and `__udivdi3`, while `0x8011446C` lies between `__udivmoddi4` and
`__umoddi3`. Neither is part of a function.

The final `__umoddi3` return is at `0x80114494`, with its architectural `nop`
delay slot at `0x80114498`. Consequently its size is `0x2C` and the exclusive
verified executable endpoint is `0x8011449C`; treating `0x80114498` as the
start of data would omit an executed instruction.
