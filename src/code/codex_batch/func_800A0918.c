#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
s32 func_800A10E0();                                /* static */
extern s32 D_8021C500;

void func_800A0918(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        var_s0 += 1;
        D_8021C500 = (D_8021C500 * 2) + func_800A10E0();
    } while (var_s0 < 0x11);
}
