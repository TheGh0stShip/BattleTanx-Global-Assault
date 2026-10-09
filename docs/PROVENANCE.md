# Research provenance

The initial ROM map and symbol seeds were adapted from the GPL-3.0 project `wootbeer/btgarecomp`, commit `775085c31e76e30022d9018ef37d6eefdf5f0499`. That project established the nonstandard ROM-to-VRAM mapping (`ROM 0x1000 -> VRAM 0x80071000`) and refined function boundaries while building a static recompilation. Its boundary table is retained as `config/us/recomp_function_boundaries.toml`; `tools/import_recomp_symbols.py` deterministically combines it with the seed symbol list so splat can distinguish code from data.

The source shapes for libmus `__MusIntRemapPtrBank` (`func_800FD2A0`), `__MusIntStartSong` (`player_text_329C`), `Fenvelope` (`func_800FAE70`, with `Fdefa` inlined), and `MusInitialize` (`func_800FAFBC`) were adapted from the MIT-licensed [Dr. Mario 64 decompilation](https://github.com/AngheloAlf/drmario64/tree/b552609482122681480f1798713f97625044a6a7/lib/libmus/src). The implementations are compiled with this project's identified KMC toolchain and independently compared byte-for-byte with the BattleTanx: Global Assault ROM.

The libultra 2.0I `osCreatePiManager`, `__osDevMgrMain`, scheduler, audio resampler, reverb, synthesizer, FX allocation, `guRotate`, `sinf`, SP task, `osStartThread`, `_Printf`, `_Putfld`, `_Ldtob`, `_Ldunscale`, and `_Genld` source shapes were likewise adapted from that MIT-licensed project. Their text, owned read-only and initialized data, and local BSS layout are independently placed and checked against this title.

The N_audio `n_alAdpcmPull`, `n_alLoadParam`, and `_decodeChunk` translation unit was reconstructed from the MIT-licensed [Mario Golf 64 `n_load.c`](https://github.com/monde-lointain/mariogolf64/blob/5014056b8ac5c26178e299bbdaede70c0d318910/src/libnaudio/n_load.c). Its declaration order and original `inp` local uses are required for the retail KMC allocation. The complete unit is independently compiled and compared byte-for-byte against this ROM.

The N_audio reverb presets and equal-power pan curve were recovered from the
SDK source retained in the MIT-licensed [Dr. Mario 64
decompilation](https://github.com/AngheloAlf/drmario64/tree/b5526094c4b699c1718ebec510acc31ccafd4b47/lib/ultralib/src/audio).
The two tables are independently compiled with this project's KMC toolchain,
placed at `0x80126890` and `0x80126A20`, and checked against the retail ROM.

The asset decompressor at `0x800A0750` is Haruhiko Okumura's LZARI algorithm
(`N=4096`, `F=60`, `THRESHOLD=2`) adapted to the game's global decoder state
and big-endian stream header. The public reference source permits free use,
distribution, and modification. All nine functions in the recovered unit and
the repository decoder are independently checked against this ROM; the N64
unit requires two documented retail-assembler load-delay nops.

The matching asset encoder in `tools/lzari.py` is a Python port of Okumura's
freely redistributable 1989 [`LZARI.C`](https://github.com/e-n-f/lzss/blob/f370a64a7ce5e4c54cfe122ca441671c3faccc24/LZARI.C).
The retail variant changes the native `unsigned long` size field to a
four-byte big-endian value; its match parser and arithmetic coder are
otherwise exact. All known retail streams are re-encoded and compared
byte-for-byte in the asset inventory tests.

The predecessor-game project `bdragoncore/battle-tanx-recomp`, commit `52c92319766e24b0c7244f76952325bece0f59ab`, is a secondary reference for shared engine, libultra, audio, and runtime behavior. BattleTanx and Global Assault are distinct games; symbols or conclusions are never transferred between them without binary evidence.

Those symbols are hypotheses and navigation aids, not matching-decomp proof. Each function boundary, name, type, and implementation must be independently verified against the supported ROM and recorded through this repository's diff workflow.

The imported boundary table has been audited through its highest addresses. The last supported executable boundary currently ends at `0x80114498`. Later candidates were rejected because the ROM words are pointer tables, zero-filled records, named libultra data, strings, or repeating asset data rather than coherent control flow. Names retained in the raw seed beyond that point are navigation hints only and must not receive `type:func` annotations without new direct evidence.

The libultra 2.0I video-mode table in `src/libultra/vitbl.c` comes from the
MIT-licensed Dr. Mario 64 repository at revision
`b5526094c4b699c1718ebec510acc31ccafd4b47`. Its 42 `OSViMode` records compile
with IDO 5.3 to the exact 0xD20 retail bytes at `0x80127090`. The local
compatibility header retains only the fixed-width record layout and constants
needed to build that table.

The source/tooling-only distribution and eventual Vita workflow follow the project structure and legal/release boundaries established in `TheGh0stShip/VitaKart64`.
