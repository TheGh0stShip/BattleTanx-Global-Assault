#include "types.h"

extern f32 D_803A5948;
extern f32 D_800732A0;
extern f32 D_800732A4;
extern f32 D_800732A8;

s32 func_800BFDA4(void *unused, u8 *wrapper) {
    register u8 *object asm("$5") = *(u8 **)(wrapper + 8);
    u32 converted;
    register s32 highValue asm("$4");
    f32 amount;

    if (object[7] == 0xFF) {
        return 1;
    }
    amount = D_803A5948 * D_800732A0;
    if (D_800732A4 < (f32)object[7] + amount) {
        object[7] = 0xFF;
        return 0;
    }
    if (D_800732A8 <= amount) {
        goto high;
    }
    converted = (s32)amount;
    goto converted_done;
high:
    highValue = (s32)(amount - D_800732A8);
    converted = highValue | 0x80000000;
converted_done:
    object[7] += converted;
    return 0;
}
