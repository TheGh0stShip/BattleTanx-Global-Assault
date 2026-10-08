#include "types.h"
#include "m2c_macros.h"

extern s16 D_80397650;
extern s32 func_800B49E0(void *, s32, M2C_UNK, u8, s32, s32, M2C_UNK *);

s32 func_800841D4(void *arg0, s32 arg1) {
    u8 sp20[40];
    u8 arg7;
    s32 arg5;
    void *arg4;
    s32 arg6;
    void *arg8;

    arg7 = M2C_FIELD(arg0, u8 *, 0x94);
    arg5 = 0x241009;
    arg8 = sp20;
    arg4 = arg0 + 8;
    arg1 += 0x10;
    arg6 = M2C_FIELD(arg0, s32 *, 0);
    D_80397650 = 0;
    return (func_800B49E0(arg4, arg1, arg5, arg7, 0, arg6, arg8) & 0xFFFF) == 0;
}
