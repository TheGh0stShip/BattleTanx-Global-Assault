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
that interval as possible. Determine boundaries first, then use the repository's
IDO toolchain and existing comparison workflow to iterate. Match compiler output,
not merely behavior. Infer types and structure layouts from assembly and caller /
callee evidence. Preserve original big-endian MIPS o32 semantics. Do not introduce
Vita portability changes into matching source.

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
