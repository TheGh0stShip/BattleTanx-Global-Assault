# Compiler identification

The game compiler has not yet been conclusively identified. IDO is strongly indicated by the N64-era code generation and library layout, but the exact release and per-file optimization flags require broader evidence.

The matching build uses more than one original compiler family. SDK/archive
objects pin the decompals static recompilation of IDO 5.3 (`v1.2`, archive
SHA-1 `976b115acb973c3828a7215b531b203537135e38`). Game code beginning at
`0x8007B030` has a KMC GCC 2.7.2 instruction shape instead. The build pins the
decompals GCC 2.7.2 and GNU binutils 2.6 releases for those translation units;
their archive SHA-256 values are enforced by `tools/bootstrap_kmc_gcc.sh`.

KMC GCC emits deliberate filled delay slots inside explicit `.set noreorder`
regions, but this game's remaining transfers retain empty delay slots instead
of accepting GNU assembler reorder scheduling. The deterministic
`tools/normalize_kmc_gcc_asm.py` pass preserves compiler-owned regions,
disables scheduling elsewhere, and inserts only those unfilled delay-slot
NOPs. `func_8007B030`, `func_8007B03C`, `func_8007B1F0`, and
`func_8007B498` prove the pipeline byte-for-byte. The larger
`func_8007B65C` renderer submit loop, asset loader `func_8007BCF0`, video-mode
selector `func_8007BDFC`, and texture-tile builder `func_8007BEF0` independently
confirm the same pipeline across control flow, DMA/decompression calls, and
runtime display-list construction.

Current C objects use `-O2 -mips2 -non_shared -G 0`, with `-g3` selected per
translation unit when the retail scheduling requires it. In particular,
`func_8007B020` requires `-g3`, while `func_8007ADB0` only matches without it.
This is evidence that original per-file flags may differ and must be recovered
rather than imposed globally.

Every compiled function must still pass the byte-exact ROM comparison. Compiler
selection is per translation unit; neither the SDK's IDO match nor the first
GCC game-code object establishes one compiler for the entire executable.
