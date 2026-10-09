# Research provenance

The initial ROM map and symbol seeds were adapted from the GPL-3.0 project `wootbeer/btgarecomp`, commit `775085c31e76e30022d9018ef37d6eefdf5f0499`. That project established the nonstandard ROM-to-VRAM mapping (`ROM 0x1000 -> VRAM 0x80071000`) and refined function boundaries while building a static recompilation. Its boundary table is retained as `config/us/recomp_function_boundaries.toml`; `tools/import_recomp_symbols.py` deterministically combines it with the seed symbol list so splat can distinguish code from data.

The source shapes for libmus `__MusIntRemapPtrBank` (`func_800FD2A0`) and `__MusIntStartSong` (`player_text_329C`) were adapted from the MIT-licensed [Dr. Mario 64 decompilation](https://github.com/AngheloAlf/drmario64/blob/master/lib/libmus/src/player.c). The implementations are compiled with this project's identified KMC toolchain and independently compared byte-for-byte with the BattleTanx: Global Assault ROM.

The libultra 2.0I `osCreatePiManager`, `__osDevMgrMain`, scheduler, audio resampler, reverb, synthesizer, FX allocation, `guRotate`, `sinf`, SP task, `osStartThread`, `_Printf`, `_Putfld`, `_Ldtob`, `_Ldunscale`, and `_Genld` source shapes were likewise adapted from that MIT-licensed project. Their text, owned read-only and initialized data, and local BSS layout are independently placed and checked against this title.

The N_audio `n_alAdpcmPull`, `n_alLoadParam`, and `_decodeChunk` translation unit was reconstructed from the MIT-licensed [Mario Golf 64 `n_load.c`](https://github.com/monde-lointain/mariogolf64/blob/5014056b8ac5c26178e299bbdaede70c0d318910/src/libnaudio/n_load.c). Its declaration order and original `inp` local uses are required for the retail KMC allocation. The complete unit is independently compiled and compared byte-for-byte against this ROM.

The predecessor-game project `bdragoncore/battle-tanx-recomp`, commit `52c92319766e24b0c7244f76952325bece0f59ab`, is a secondary reference for shared engine, libultra, audio, and runtime behavior. BattleTanx and Global Assault are distinct games; symbols or conclusions are never transferred between them without binary evidence.

Those symbols are hypotheses and navigation aids, not matching-decomp proof. Each function boundary, name, type, and implementation must be independently verified against the supported ROM and recorded through this repository's diff workflow.

The imported boundary table has been audited through its highest addresses. The last supported executable boundary currently ends at `0x80114498`. Later candidates were rejected because the ROM words are pointer tables, zero-filled records, named libultra data, strings, or repeating asset data rather than coherent control flow. Names retained in the raw seed beyond that point are navigation hints only and must not receive `type:func` annotations without new direct evidence.

The source/tooling-only distribution and eventual Vita workflow follow the project structure and legal/release boundaries established in `TheGh0stShip/VitaKart64`.
