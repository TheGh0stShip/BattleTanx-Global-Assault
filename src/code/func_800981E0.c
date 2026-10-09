#include "types.h"

s32 func_800FB888(s32, s32 *, u32);
extern u32 D_801B4490;
extern s32 D_801B4494;

s32 func_800981E0(s32 arg0, s32 *arg1, u32 arg2) {
    volatile s32 frame_pad[2];
    u32 index = 0;
    s32 result;
    u32 count = D_801B4490;

    if (count != 0) {
        arg2 = count;
        arg1 = &D_801B4494;
loop:
        index++;
        if (*arg1 == arg0) {
            goto found;
        }
        arg1++;
        if (index < arg2) {
            goto loop;
        }
    }
    result = 0;
check:
    if (result == 0) {
        goto fallback;
    }
    result = 1;
    goto done;
found:
    result = 1;
    goto check;
fallback:
    result = func_800FB888(arg0, arg1, arg2);
done:
    return result;
}
