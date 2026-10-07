# Claude isolation rules

This directory is an isolated scratch area for a second decompilation assistant.

## Hard boundaries

- Treat the repository root outside `claude-work/` as read-only.
- Create, edit, or delete files only below `claude-work/`.
- Do not run `git add`, `git commit`, `git checkout`, `git switch`, `git reset`,
  `git clean`, `git stash`, `git rebase`, or any command that changes Git state.
- Do not modify the active build, configuration, source tree, generated files, or
  toolchains. Never alter or replace another worker's files.
- Do not copy ROMs, extracted assets, generated assembly, object files, binaries,
  or proprietary SDK source into this directory. Small original source candidates,
  notes, scripts, command transcripts, and textual diff reports are allowed.
- Read the repository-root `AGENTS.md` and obey its matching and architecture
  requirements. A behavioral reconstruction is not a match without byte evidence.

## Deliverables

- Put proposed original C in `output/src/`, preserving the intended production
  relative path below that directory.
- Put any proposed production-file patch in `output/patches/` as a plain unified
  diff. The primary worker will review and apply it; do not apply it yourself.
- Put analysis and matching evidence in `output/reports/`.
- Record every function address, symbol/name, byte size, compiler flags, and exact
  comparison result. Clearly label candidates that do not match.
- Keep one cumulative `output/HANDOFF.md` with completed, matching, partial, and
  untouched function counts for the assigned range.
