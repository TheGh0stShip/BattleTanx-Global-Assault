# N64 Tooling Notes

This is a project-specific review of tools listed by [n64.dev](https://n64.dev/).
It is not a dependency list. New tools must preserve the repository's exact-ROM
comparison gate and must not turn equivalent-but-different code into a match.

## Local trial results

The following results were measured on 2026-10-08 without adding repository
dependencies:

- `m2c` generated a structurally accurate 51-line first pass for
  `func_800F6344` in 0.57 seconds. It recovered the branches, repeated player
  lookup, field offsets, and calls, but not safe types or project-ready struct
  declarations. Use it to remove transcription work, then rewrite and verify
  with `cmpfunc_strict.sh`.
- The hand-cleaned version of that candidate reached the correct size with only
  five differing words on its first strict comparison. This supports using
  `m2c` for initial control flow, but does not support treating its output as a
  match.
- `asm-differ` starts successfully in an isolated environment, but needs a
  project adapter for the KMC/IDO selection, real ROM addresses, linker symbols,
  and normalizer. The existing strict comparator remains the better unattended
  gate; `asm-differ` would mainly improve interactive diagnosis.
- `decomp-permuter` starts successfully, but its stock import path assumes a
  conventional Make-produced object. It needs a compile adapter that invokes
  this repository's compiler and normalizer and preserves relocation/rodata
  context. Trial it on two-to-twenty-word near misses after that adapter exists,
  not on structurally wrong functions.
- The current `m3c` repository is a Pokemon Snap-specific prototype with
  hard-coded developer paths, a fixed Ninja build, and a TODO where permuter
  integration should be. Do not adopt it; its useful idea is already covered by
  explicitly chaining `m2c`, a project-specific compiler wrapper, and the
  permuter.

## Use now

- [asm-differ](https://github.com/simonlindholm/asm-differ): add or adapt a
  local configuration for rapid, symbol-aware assembly diffs while editing a
  function. This should sit in front of the narrow object/ROM comparator; it
  does not replace the comparator or `make verify-code`.
- [decomp-permuter](https://github.com/simonlindholm/decomp-permuter): useful
  for the remaining small and medium near-misses involving expression order,
  temporary lifetimes, register allocation, and statement placement. Its
  compile script must invoke this project's exact KMC GCC or IDO path and the
  appropriate production normalizer mode. Accept a result only after an
  unmasked object/ROM comparison.
- [m2c](https://github.com/matt-kempster/m2c): continue using it to produce a
  readable first-pass C control-flow model from MIPS assembly. Treat output as
  a candidate, never as matching evidence.
- [splat](https://github.com/ethteck/splat) and
  [spimdisasm](https://github.com/Decompollaborate/spimdisasm): retain as the
  ROM segmentation and disassembly foundation. Prefer improving the existing
  configuration over introducing a parallel extraction pipeline.
- [ultralib](https://github.com/decompals/ultralib) and
  [libreultra](https://github.com/n64decomp/libreultra): use as provenance and
  source-shape references for the remaining libultra functions. Compiler,
  version, flags, section ownership, and bytes still have to be verified in
  this repository.

## Evaluate after the current integration batch

- [bdiff](https://github.com/encounter/bdiff): useful as a fast first-pass binary
  triage tool for locating changed runs before using the project's
  symbol-aware and instruction-aware comparators. It is not sufficient match
  evidence by itself.
- [n64sym](https://github.com/shygoo/n64sym): useful when identifying remaining
  SDK/library functions from RAM or code fingerprints. It is evidence for a
  symbol name, not proof of source or byte identity.
- [rabbitizer](https://github.com/Decompollaborate/rabbitizer): useful as a
  maintained MIPS instruction decoder if the local comparison, boundary-audit,
  or normalizer tests need richer opcode analysis. Prefer it over adding
  hand-maintained instruction masks.
- [romjudge](https://github.com/jkbenaim/romjudge): potentially useful as an
  additional ROM header/byte-order/checksum sanity check around releases. It
  should supplement, not replace, the known-base-ROM hash and reconstruction
  comparison.
- [y64_linker](https://github.com/ethteck/y64_linker): potentially useful for
  inspecting MIPS64 ELF relocations and extracting sections while diagnosing
  the remaining jump-table and rodata-placement problems. Validate its output
  against the repository's real linker map and ROM addresses.

## Difficult static-analysis cases

- [N64LoaderWV](https://github.com/zeroKilo/N64LoaderWV): consider for a
  Ghidra workspace when assembly plus relocation evidence is insufficient to
  establish function boundaries, jump-table ownership, or shared data. Keep
  Ghidra-derived names and types provisional until corroborated by the ROM and
  build.
- [n64ops](https://github.com/PeterLemon/N64): a compact R4300i/RCP/RSP opcode
  reference for checking unusual instructions, delay-slot behavior, and
  coprocessor operations. The VR4300 and official SDK references remain the
  authority when sources disagree.

## Later runtime validation

- [Mupen64+ Reverser Edition](https://www.retroreversing.com/mupen64RE): useful
  for breakpoints, memory inspection, and tracing when static analysis cannot
  establish behavior or data ownership.
- [ares](https://ares-emu.net/) and
  [cen64](https://github.com/n64dev/cen64): useful as higher-accuracy emulator
  checks after reconstruction remains exact and runtime tests are needed.
  Emulator success is not a substitute for byte matching or supervised tests
  on physical N64 hardware.

## Not priorities for matching decompilation

Asset exporters, modern N64 homebrew libraries/toolchains, ROM packers, and
flash-cart uploaders solve different problems. Reconsider them only for asset
documentation, an N64-native test harness, or explicitly supervised hardware
testing. They should not be mixed into the matching pipeline, and none of them
changes the decision to defer PS Vita-specific work until the portable source
base is substantially complete.
