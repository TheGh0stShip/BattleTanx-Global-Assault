# Project instructions

This is a matching decompilation of BattleTanx: Global Assault (N64, USA Rev 0) and the source foundation for a later PS Vita port. Keep matching-decomp work distinct from host/Vita portability changes. Never commit ROMs, ROM-extracted assets, generated assembly, binaries, crash dumps, or private hardware evidence.

## Architectures

- Original game: big-endian MIPS III/R4300, 32-bit o32 ABI. Preserve N64 integer widths, alignment, delay-slot behavior, and big-endian disk/ROM layouts while matching.
- PS Vita: little-endian ARMv7-A Cortex-A9, 32-bit ARM/Thumb, ILP32 (`int`, `long`, pointers, and `size_t` are 32-bit). It is not AArch64.
- Linux/WSL x86-64 hosts are normally LP64; Windows x64 is LLP64. Use explicit-width integers and endian-safe reads at ROM, disk, wire, checksum, and pointer-token boundaries. Do not globally pack structs or truncate live host pointers.
- Before accepting Vita output, inspect compiler identity and ELF attributes, especially hard-float versus softfp ABI consistency. CPU floating-point capability does not establish the ABI. Check ARM alignment and calling conventions.

Host or emulator success is not evidence of physical Vita correctness. Label host-only defects as host-only. A Vita release gate requires a clean cross-build plus supervised testing on physical hardware.

## Workflow gates

1. Verify the base ROM before extraction or comparison.
2. Keep generated `asm/`, extracted assets, and build products untracked.
3. Every matching claim must be supported by an object/ROM diff, not just equivalent behavior.
4. Every portability change must retain original 32-bit semantics and receive host compile-time layout assertions where relevant.
5. Do not deploy to or launch hardware without an explicitly supervised hardware-test request.

`make verify-code` is the current matching gate. It must remain byte-identical while assembly functions are replaced with C. Do not describe a function as decompiled or matching unless its compiled bytes pass this gate (or a narrower object diff that is subsequently covered by it).
