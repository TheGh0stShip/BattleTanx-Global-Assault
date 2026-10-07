# BattleTanx: Global Assault parallel decompilation assignment

You are assisting the matching N64 decompilation in the parent repository.

Before doing anything, read:

1. `../AGENTS.md`
2. `AGENTS.md`

Your exclusive assignment is the game-code interval **0x8007B65C through
0x8007C364** (start inclusive, end exclusive). Other workers own all ranges
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

The compiler and assembler problem is solved in production. Do not spend more
time redesigning or debugging the old `cmpfunc.py` harness. The parent repository
now contains the authoritative pipeline:

- `../tools/normalize_kmc_gcc_asm.py` preserves GCC-owned delay slots and inserts
  NOPs only for transfers emitted in reorder mode;
- `../tools/trim_elf32_section.py` removes translation-unit tail padding;
- `../tools/build_code.sh` compiles and links each accepted unit; and
- `make clean-generated && make verify-code && make check` is the acceptance gate.

The parent has independently integrated and byte-verified `func_8007B030`,
`func_8007B03C`, `func_8007B1F0`, and `func_8007B498`. Do not revisit them.
Begin at `func_8007B65C` and proceed in address order through `0x8007C364`.

For scratch iteration, copy the three relevant parent tools into a temporary
directory outside the repository or invoke them read-only. Compile one proposed
translation unit at a time, trim it to the exact target interval, link it at its
retail address with symbols from `../config/us/symbol_addrs.txt`, and compare the
entire interval byte-for-byte. A result is invalid if emitted and target sizes
differ; in particular, never report `MATCH` alongside different sizes. If a
candidate repeatedly fails, preserve the best C and mismatch evidence and move
to the next function instead of altering the established compiler pipeline.

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
