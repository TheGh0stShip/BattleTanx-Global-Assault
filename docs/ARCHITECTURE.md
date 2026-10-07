# Architecture and portability contract

The project has three deliberately separate execution environments:

| Environment | CPU/ABI | Endian | Purpose |
|---|---|---|---|
| Original N64 | MIPS R4300, o32 | Big | Matching ROM reconstruction |
| Development host | Usually x86-64 LP64/LLP64 | Little | Tools, tests, eventual native runtime |
| PS Vita | ARMv7-A Cortex-A9, ILP32 | Little | Final handheld port |

The original ROM remains the behavioral and binary-match reference. Host-facing code must not reinterpret N64 structs through native C layouts. Decode serialized fields with explicit widths and byte order, assert layouts that genuinely cross boundaries, and represent emulated 32-bit addresses as integer tokens rather than truncated host pointers.

The Vita cross-build must verify `arm-vita-eabi-gcc` targets 32-bit ARM and must inspect every linked dependency with `arm-vita-eabi-readelf -A`. All objects must agree on the floating-point calling convention. Hardware validation is required because host tests and Vita3K cannot establish Cortex-A9 alignment, timing, driver, or calling-convention correctness.
