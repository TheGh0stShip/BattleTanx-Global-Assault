#include "types.h"

extern u32 D_801B4490[];

s32 func_80098190(s32 value) {
    u32 index;

    for (index = 0; index < D_801B4490[0]; index++) {
        if (D_801B4490[index + 1] == value) {
            return 1;
        }
    }
    return 0;
}
