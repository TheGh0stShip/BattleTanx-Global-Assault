#include "types.h"

extern u16 D_803A5988;
extern s16 D_803A5970;

s32 func_800C247C(void) {
    if (D_803A5988 != 0) {
        return 0;
    }
    D_803A5970 = 1;
    return 1;
}
