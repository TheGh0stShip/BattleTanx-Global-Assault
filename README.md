# BattleTanx: Global Assault decompilation

This is a work-in-progress matching decompilation of **BattleTanx: Global Assault** for Nintendo 64 (USA Rev 0). Its immediate goal is a byte-identical ROM reconstruction from readable C and locally extracted assets. That recovered source will then become the foundation for the **VitaTanxGA** PS Vita port, following the source/tooling and hardware-validation workflow used by [VitaKart64](https://github.com/TheGh0stShip/VitaKart64).

No ROM or ROM-derived assets are included. Supply your own legally obtained dump.

## Supported ROM

- Title: `BATTLETANXGA`
- Region/revision: USA, Rev 0 (`NBQE`)
- Format: big-endian `.z64`
- Size: 8 MiB
- SHA-1: `805248fb0a0ee694cad8d7dc927b631d860dd8cf`

## Bootstrap

```sh
cp /path/to/BattleTanxGA.z64 "BattleTanx - Global Assault (USA).z64"
make setup
make split
make verify-code
make check
```

`make setup` creates a local Python environment, verifies the dump, and copies it to the ignored canonical path. `make split` disassembles the known first-MiB code region with splat. `make verify-code` assembles and links that region and requires it to match ROM offsets `0x000000-0x101000` byte-for-byte. If GNU MIPS binutils are not installed system-wide, the build downloads the Ubuntu package into the ignored `.toolchain/` directory without requiring root. Generated assembly is intentionally ignored; reviewed C and project metadata are the source of truth.

## Current status

- ROM identity and the unusual `0x80071000` load address are verified.
- A GPL-compatible catalogue contains 1,684 supported function boundaries after correcting false splits and aggregations, with provenance.
- Reproducible splat extraction and repository safety gates are in place.
- The header, IPL3, and known first-MiB code region reconstruct byte-for-byte.
- 1,171 of 1,684 functions (69.5%) are reconstructed in production C and pass the full byte-exact region gate. By function-body bytes, 284,924 of 632,176 bytes (45.1%) are reconstructed.
- Four reconstructed units require narrowly gated, documented normalizer rules; they are identified separately from pure source matches in [the normalizer-assisted record](docs/NORMALIZER_ASSISTED.md).
- CI publishes an objdiff v2 `us_report` artifact for decomp.dev progress tracking.
- Matching C reconstruction, compiler identification, linker layout, asset format mapping, and ROM rebuild are not complete.
- The Vita port has not begun; N64 source recovery comes first.

See the [architecture and portability contract](docs/ARCHITECTURE.md), [library fingerprints](docs/LIBRARIES.md), and [research provenance](docs/PROVENANCE.md).

## License

Project-authored code and imported GPL research are licensed under GPL-3.0; see `COPYING`. BattleTanx: Global Assault and its game data remain the property of their respective rights holders.
