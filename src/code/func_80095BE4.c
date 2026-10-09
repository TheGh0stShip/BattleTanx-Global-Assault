#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK func_80097BA4(s32, M2C_UNK);                /* static */

void func_80095BE4(void *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x288);
    if (temp_a0 != 0) {
        func_80097BA4(temp_a0, 0);
        M2C_FIELD(arg0, s32 *, 0x288) = 0;
    }
    temp_a0_2 = M2C_FIELD(arg0, s32 *, 0x280);
    if (temp_a0_2 != 0) {
        func_80097BA4(temp_a0_2, 0);
        M2C_FIELD(arg0, s32 *, 0x280) = 0;
    }
    temp_a0_3 = M2C_FIELD(arg0, s32 *, 0x284);
    if (temp_a0_3 != 0) {
        func_80097BA4(temp_a0_3, 0);
        M2C_FIELD(arg0, s32 *, 0x284) = 0;
    }
}
