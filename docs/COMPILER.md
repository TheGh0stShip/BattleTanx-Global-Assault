# Compiler identification

The game compiler has not yet been conclusively identified. IDO is strongly indicated by the N64-era code generation and library layout, but the exact release and per-file optimization flags require broader evidence.

The current matching build pins the decompals static recompilation of IDO 5.3 (`v1.2`, archive SHA-1 `976b115acb973c3828a7215b531b203537135e38`). IDO 5.3 and IDO 7.1 both generate the retail instruction sequence for the initial leaf-function probes, so those probes do not distinguish the releases. IDO 5.3 is a provisional build choice, not a claim that compiler identification is complete.

Every compiled function must still pass the byte-exact ROM comparison. If later nontrivial functions consistently require another IDO release or different flags, update the pinned toolchain and document the evidence here.
