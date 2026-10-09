#include "types.h"

extern u8 D_80122EE0[];
extern u8 D_80122EE8[];
extern u8 D_80122EEC[];

void func_80082A90(u8 *state, f32 *values) {
    u8 *owner = *(u8 **)(state + 0x1D0);
    f32 first = values[0] / *(f32 *)(owner + 0x1DC);
    f32 multiplier = (f32)*(u16 *)(
        D_80122EE8 + *(s32 *)(state + 0x98) * 208);

    *(f32 *)(state + 0x1B8) = first;
    *(f32 *)(state + 0x1BC) = values[1] * multiplier;
    *(f32 *)(state + 0x1C0) = values[2] * multiplier;
    *(f32 *)(state + 0x1C4) = values[3];
    *(s32 *)(state + 0x1C8) =
        (s32)((f32)*(s32 *)(state + 0x1D4) *
              *(f32 *)(D_80122EEC +
                       *(volatile s32 *)(state + 0x98) * 208) * values[4]);
    *(f32 *)(state + 0x1CC) = values[5];
}
