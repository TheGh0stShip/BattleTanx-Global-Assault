#include "types.h"

extern f64 D_800725B0;
extern f32 D_800725B8;
extern s32 player_bss_0048;

f32 func_8009D8A0(f32 arg0) {
    f64 value;
    f32 result;
    s32 seed;

    seed = (player_bss_0048 * 0x10DCD) + 1;
    value = (f64)seed;
    player_bss_0048 = seed;
    if (seed < 0) {
        value += D_800725B0;
    }
    result = ((f32)value / D_800725B8) * arg0;
    __asm__ volatile ("" : : "f"(result));
    return result;
}
