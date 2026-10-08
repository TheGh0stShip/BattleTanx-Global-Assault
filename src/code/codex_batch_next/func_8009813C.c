#include "types.h"

extern s32 D_801B4490[];

s32 func_8009813C(s32 value) {
    s32 index = D_801B4490[0];

    if ((u32)index < 42) {
        D_801B4490[0] = index + 1;
        D_801B4490[index + 1] = value;
        return 1;
    }
    return 0;
}
