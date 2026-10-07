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
make check
```

`make setup` creates a local Python environment, verifies the dump, and copies it to the ignored canonical path. `make split` disassembles the known first-MiB code region with splat. Generated assembly is intentionally ignored; reviewed C and project metadata are the source of truth.

## Current status

- ROM identity and the unusual `0x80071000` load address are verified.
- A GPL-compatible seed map of roughly 1,400 function symbols has been imported with provenance.
- Reproducible splat extraction and repository safety gates are in place.
- Matching C reconstruction, compiler identification, linker layout, asset format mapping, and ROM rebuild are not complete.
- The Vita port has not begun; N64 source recovery comes first.

See [architecture and portability contract](docs/ARCHITECTURE.md) and [research provenance](docs/PROVENANCE.md).

## License

Project-authored code and imported GPL research are licensed under GPL-3.0; see `COPYING`. BattleTanx: Global Assault and its game data remain the property of their respective rights holders.
