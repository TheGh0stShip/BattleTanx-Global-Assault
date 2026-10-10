#include "types.h"

extern f32 D_803A5948;

s32 func_800BFDA4(void *unused, u8 *wrapper) {
    u8 *object = *(u8 **)(wrapper + 8);
    u32 converted;
    f32 amount;

    if (object[7] == 0xFF) {
        return 1;
    }
    amount = D_803A5948 * 15.0f;
    if (255.0f < (f32)object[7] + amount) {
        object[7] = 0xFF;
        return 0;
    }
    if (2.1474836e+09f <= amount) {
        goto high;
    }
    converted = (s32)amount;
    goto converted_done;
high:
    converted = (s32)(amount - 2.1474836e+09f);
    converted |= 0x80000000;
converted_done:
    object[7] += converted;
    return 0;
}
