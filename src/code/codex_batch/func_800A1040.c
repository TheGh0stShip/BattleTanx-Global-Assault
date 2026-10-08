#include "types.h"

extern u32 D_8021DF28[];

s32 func_800A1040(u32 arg0) {
    s32 temp_v1;
    s32 var_a1 = 1;
    s32 var_a2 = 0x13A;

    do {
        temp_v1 = (s32) (var_a1 + var_a2) / 2;
        if (arg0 < D_8021DF28[temp_v1]) {
            var_a1 = temp_v1 + 1;
        } else {
            var_a2 = temp_v1;
        }
    } while (var_a1 < var_a2);
    return var_a1;
}
