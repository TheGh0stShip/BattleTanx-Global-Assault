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

The libmus asset inventory is independently derived from the matching
`MusPtrBankInitialize`, effect-player, and song-relocation routines plus the
retail 26-entry resource directory. Pointer-bank, wave-bank, effect, and song
references are range-checked directly against the verified ROM. The unlicensed
`nviewer` project was used only to cross-check format facts; none of its parser
or renderer source is incorporated.

The inspection-only N64 ADPCM decoder in `tools/decode_libmus_wave.py` follows
the ABI1 recurrence and adversarial oracle vectors from the MIT-licensed
[`slfx77/neversoft-multitool`](https://github.com/slfx77/neversoft-multitool/tree/3c9028e0178deb9060190362d1c47039a32846e2/src/NeversoftMultitool/Core/Formats/Audio)
implementation. Global Assault's 307 PTR/WBK waves are parsed and validated
independently against this project's verified ROM; the decoder never infers a
playback rate absent from the serialized bank. The required upstream license
text is retained in [`THIRD_PARTY_LICENSES.md`](THIRD_PARTY_LICENSES.md).

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

The libultra `__osRcpImTable` reconstruction follows `setintmask.s` from
`decompals/ultralib` revision `e24c8367`. Rather than copying an extracted byte
array, `src/libultra/os_rcp_im_table.c` expresses the documented conversion
from six MI interrupt-enable bits to the RCP's paired clear/set mask fields.
IDO 5.3 evaluates the table to the exact 0x80 retail bytes at `0x80077A00`.

The initialized RSP DMEM layout for F3DEX FIFO 2.07 follows the matching,
documented CC0 disassembly in
[`Mr-Wiseguy/f3dex2`](https://github.com/Mr-Wiseguy/f3dex2/tree/bd31393fd02b89e024b043c8c50c9a5a10143fb6).
The pinned upstream 2.07 configuration independently assembles to the same
`0x1390`-byte instruction image found at `0x800F8E80` and the same `0x420`-byte
data image found at `0x80125EC0`; `src/code/f3dex2_fifo_data.c` preserves its
named matrices, render state, light buffers, dispatch tables, clipping state,
and overlay metadata without embedding an extracted binary.

The standard `0xCC`-byte RSP boot program at `0x800F8DB0`, including its
four-byte link pad, is reconstructed from the CC0
[`n64decomp/sm64` RSP source](https://github.com/n64decomp/sm64/blob/9921382a68bb0c865e5e45eb594d9c64db59b1af/rsp/rspboot.s).
The build downloads only checksum-pinned source archives and regenerates both
RSP instruction images; no extracted microcode binary is stored in Git.

The adjacent `0x801262E0–0x80126590` initialized DMEM image is the N_audio
RSP ABI data layout. Its vector constants and 256-entry resampling table were
cross-checked against the CC0
[`n64decomp/sm64` audio microcode source](https://github.com/n64decomp/sm64/blob/9921382a68bb0c865e5e45eb594d9c64db59b1af/rsp/audio.s).
Global Assault's 16-entry command dispatch table is title-specific and was
independently verified against the retail ROM rather than copied from that
reference.

The title-specific N_audio RSP instruction program at
`0x800FA210–0x800FAE70` is a new symbolic reverse-engineering in
`rsp/n_aspMain.s`. All 789 instructions, 80 control-flow labels, and the
12-byte unreachable object pad assemble with the repository's pinned armips
to the exact `0xC60` retail bytes. No extracted binary or word-array
transcription is used. Handler names were inferred from the opcode order in
[`mupen64plus-rsp-hle`](https://github.com/mupen64plus/mupen64plus-rsp-hle/tree/8a7a472)
and checked against the dispatcher conventions in the CC0 SM64 audio source;
neither reference supplied this microcode's instruction source.

The initialized libultra objects at `0x80126B40–0x80126B60` and
`0x80126E30–0x80126F80` follow the 2.0I definitions in
[`decompals/ultralib`](https://github.com/decompals/ultralib/tree/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd).
They include the hardware-interrupt table, PI/SI access flags, thread sentinel
and queues, timer list, VI contexts, and small audio/controller globals. The
definitions were retyped into this repository's independently matching units;
all owning text remains byte-identical and every initialized byte and pointer
relocation is compared at its retail address. The adjacent N_audio globals at
`0x80126B20–0x80126B40` use the MIT-licensed Mario Golf 64 reconstruction at
revision `5014056b8ac5c26178e299bbdaede70c0d318910` as a declaration reference.

The libmus command dispatch, effect presets, scheduler callbacks, and initial
audio state at `0x80126590–0x80126890` follow the MIT-licensed N64 Sound Tools
3.14 sources retained by the
[`AngheloAlf/drmario64` decompilation](https://github.com/AngheloAlf/drmario64/tree/b5526094c4b699c1718ebec510acc31ccafd4b47/lib/libmus/src).
The typed reconstruction is linked at its retail address and all relocated
pointers and scalar values are compared byte-for-byte with this title.

The source/tooling-only distribution and eventual Vita workflow follow the project structure and legal/release boundaries established in `TheGh0stShip/VitaKart64`.
