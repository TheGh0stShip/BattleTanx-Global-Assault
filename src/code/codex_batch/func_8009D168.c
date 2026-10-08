#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK func_80097D14(s32, M2C_UNK);                /* extern */
f32 func_8009D8A0(f32);                             /* extern */
extern f32 D_80072558;

void func_8009D168(void) {
    s32 temp_f2;
    s32 var_a0;

    temp_f2 = (s32) func_8009D8A0(D_80072558);
    var_a0 = temp_f2 + 6;
    if ((((var_a0 == 0x18) | (var_a0 == 9)) != 0) || (var_a0 == 0x16)) {
        var_a0 = temp_f2 + 7;
    }
    func_80097D14(var_a0, 0xA);
}
