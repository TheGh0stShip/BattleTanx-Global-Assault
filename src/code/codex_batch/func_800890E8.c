#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK func_8009DA44(M2C_UNK *, void *, s32);      /* extern */
u16 func_8009DFAC(M2C_UNK *);                       /* extern */
s32 func_80089154();                                /* static */

s32 func_800890E8(void *arg0, s32 *arg1) {
    M2C_UNK sp10;
    s32 temp_v0;
    u16 var_a0;

    temp_v0 = func_80089154();
    if (temp_v0 == 0) {
        var_a0 = M2C_FIELD(arg0, u16 *, 0x20);
        *arg1 = 0;
    } else {
        func_8009DA44(&sp10, arg0 + 8, temp_v0);
        var_a0 = func_8009DFAC(&sp10);
        *arg1 = 1;
    }
    return var_a0 & 0xFFFF;
}
