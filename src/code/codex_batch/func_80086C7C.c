#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
s32 func_80086C7C(void *arg0) {
    s32 var_a1;
    u32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, u32 *, 0x18);
    var_a1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 4:
        var_a1 = M2C_FIELD(arg0, s32 *, 0x68) + 0x10;
        break;
    case 5:
        var_a1 = M2C_FIELD(arg0, s32 *, 0x68) + 0x24;
        break;
    case 6:
    case 3:
        var_a1 = M2C_FIELD(arg0, s32 *, 0x68) + 8;
        break;
    }
    return var_a1;
}
