# Claude parallel decompilation workspace

This directory lets Claude analyze and reconstruct a non-overlapping code range
without modifying the primary decompilation worktree. Claude may inspect files in
the parent repository, but all of its writes must remain here.

The current assignment is in `PROMPT.md`. Reviewable results belong under
`output/`; the primary worker will independently verify and integrate them in a
larger batch.

