# N64 Tooling Notes

This is a project-specific review of tools listed by [n64.dev](https://n64.dev/).
It is not a dependency list. New tools must preserve the repository's exact-ROM
comparison gate and must not turn equivalent-but-different code into a match.

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

- [m3c](https://github.com/ethteck/m3c): potentially combines m2c and
  decomp-permuter into a higher-throughput candidate pipeline. Before using it
  broadly, build an adapter for the repository's compiler selection, real
  function address, rodata placement, production normalizer, and strict
  comparison. It must distinguish direct C matches from
  normalizer-assisted matches.
- [n64sym](https://github.com/shygoo/n64sym): useful when identifying remaining
  SDK/library functions from RAM or code fingerprints. It is evidence for a
  symbol name, not proof of source or byte identity.
- [romjudge](https://github.com/jkbenaim/romjudge): potentially useful as an
  additional ROM header/byte-order/checksum sanity check around releases. It
  should supplement, not replace, the known-base-ROM hash and reconstruction
  comparison.

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
