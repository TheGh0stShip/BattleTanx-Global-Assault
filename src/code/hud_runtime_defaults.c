#include "types.h"

/* Mutable HUD state and two packed color defaults. */
u32 gHudRuntimeDefaultWords[16] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xFFFFFF00, 0xFFFFFF00,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
};
