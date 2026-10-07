# BattleTanx: Global Assault parallel decompilation assignment

You are assisting the matching N64 decompilation in the parent repository.

Before doing anything, read:

1. `../AGENTS.md`
2. `AGENTS.md`

Your exclusive assignment is the game-code interval **0x8007B020 through
0x8007C000** (start inclusive, end exclusive). Other workers own all ranges
outside it. Do not analyze or change their implementations except for read-only
context needed to understand callers, callees, types, and conventions.

Work autonomously toward exact C reconstruction of as many complete functions in
that interval as possible. Determine boundaries first, then use the verified KMC
GCC 2.7.2 and KMC GNU assembler 2.6 pipeline recorded in
`output/reports/harness_baseline.md`. Do not return to the provisional IDO 5.3
assumption for this interval unless byte evidence disproves KMC GCC. Match compiler
output, not merely behavior. Infer types and structure layouts from assembly and
caller / callee evidence. Preserve original big-endian MIPS o32 semantics. Do not
introduce Vita portability changes into matching source.

## Current recovery point

The comparison harness now has two trustworthy properties that must not regress:

- it measures one ELF symbol through the next text symbol rather than the rest of
  the combined `.text` section;
- it resolves relocations to their final addresses and compares every emitted word
  without masking relocated fields.

`func_8007B030` is the baseline exact match (0xC emitted and target bytes).
`func_8007B03C` compiles to the correct instructions but raw KMC assembler reorder
mode emits 0xA0 instead of the retail 0xA8. The seven reported word differences
are one scheduling issue, not seven independent C problems: KMC `as` moves the
`sb` into the final `jal` delay slot and moves `addiu sp,sp,24` into the `jr` delay
slot. Retail keeps both instructions before their transfers and has a NOP in each
delay slot.

Next, make a separate, deterministic assembly-normalization stage and test only
these two functions:

1. Preserve GCC's existing explicit `.set noreorder` / `.set reorder` regions;
   those contain intentionally filled delay slots and already match.
2. For code GCC emits in reorder mode, prevent assembler scheduling and add an
   explicit `nop` after an otherwise unfilled `jal`, `j`, or `jr`.
3. Assemble the normalized file with KMC GNU `as` 2.6, never the repository's
   modern GNU assembler.
4. Require `func_8007B030` to remain a 0xC exact match and
   `func_8007B03C` to become a 0xA8 exact match before iterating on any later C.
5. Record both raw GCC assembly and normalized assembly in the report. The
   transformation must be syntax-aware/stateful; do not blindly insert NOPs after
   transfers already inside GCC's noreorder blocks.

Do not delegate or fan out more functions until that two-function regression gate
passes. Once it passes, use the same fixed pipeline for the remaining functions.
Do not count a function merely because its structure looks right.

Strict isolation requirements:

- The parent repository is read-only. Write only under `claude-work/`.
- Do not use any Git command that changes state, and do not commit anything.
- Do not edit production source, build scripts, configuration, generated assembly,
  extracted assets, ROMs, or toolchains.
- Do not copy the base ROM, generated assembly, extracted assets, objects, or other
  binaries into your workspace.
- You may compile scratch source to a temporary system directory for comparison,
  but leave no binary deliverables in `claude-work/`.

Required output:

1. Candidate C files under `claude-work/output/src/`, arranged by proposed
   production relative path.
2. A proposed unified integration diff under `claude-work/output/patches/` that is
   **not applied** to the parent repository.
3. `claude-work/output/reports/B020_C000.md` containing function boundaries,
   meanings, callers/callees, inferred types/layouts, compiler version/flags, and
   byte-for-byte comparison evidence for every attempted function.
4. `claude-work/output/HANDOFF.md` with explicit totals: functions discovered,
   functions exactly matched, functions partial, functions untouched, and bytes
   exactly matched. List all generated files and any assumptions or blockers.

Do not claim a match unless the emitted text bytes were compared against the ROM
or canonical extracted text and are identical. Group results into a useful chunk;
do not make one-function commits (and in fact do not commit at all).
