# Research provenance

The initial ROM map and symbol seeds were adapted from the GPL-3.0 project `wootbeer/btgarecomp`, commit `775085c31e76e30022d9018ef37d6eefdf5f0499`. That project established the nonstandard ROM-to-VRAM mapping (`ROM 0x1000 -> VRAM 0x80071000`) and refined function boundaries while building a static recompilation. Its boundary table is retained as `config/us/recomp_function_boundaries.toml`; `tools/import_recomp_symbols.py` deterministically combines it with the seed symbol list so splat can distinguish code from data.

The predecessor-game project `bdragoncore/battle-tanx-recomp`, commit `52c92319766e24b0c7244f76952325bece0f59ab`, is a secondary reference for shared engine, libultra, audio, and runtime behavior. BattleTanx and Global Assault are distinct games; symbols or conclusions are never transferred between them without binary evidence.

Those symbols are hypotheses and navigation aids, not matching-decomp proof. Each function boundary, name, type, and implementation must be independently verified against the supported ROM and recorded through this repository's diff workflow.

The source/tooling-only distribution and eventual Vita workflow follow the project structure and legal/release boundaries established in `TheGh0stShip/VitaKart64`.
