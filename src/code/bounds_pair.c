#include "types.h"

extern u8 *D_80219498;

s32 func_800B02D4(f32 x, f32 y, s32 index) {
    s32 offset = ((index & 0xFF) << 5) + 4;
    s16 *bounds = (s16 *)(D_80219498 + offset);

    if ((f32)bounds[0] <= x && x <= (f32)bounds[2] &&
        (f32)bounds[1] <= y && y <= (f32)bounds[3]) {
        return 1;
    }
    return 0;
}

s32 func_800B0364(f32 x, f32 y, s32 index) {
    s32 offset = ((index & 0xFF) << 5) + 4;
    s16 *bounds = (s16 *)(D_80219498 + offset);

    if ((f32)bounds[4] <= x && x <= (f32)bounds[6] &&
        (f32)bounds[5] <= y && y <= (f32)bounds[7]) {
        return 1;
    }
    return 0;
}
