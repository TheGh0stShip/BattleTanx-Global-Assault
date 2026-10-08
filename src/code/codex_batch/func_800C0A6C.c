#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
extern u32 D_80117EB8;
extern M2C_UNK D_80118EC4;
extern M2C_UNK D_80118ECC;
extern M2C_UNK D_80118ED4;

void func_800C0A6C(void *arg0) {
    M2C_UNK *var_v0;

    switch (D_80117EB8) {                           /* irregular */
    case 1:
        var_v0 = &D_80118EC4;
        break;
    case 3:
        var_v0 = &D_80118ED4;
        break;
    default:
    case 2:
        var_v0 = &D_80118ECC;
        break;
    }
    M2C_FIELD(arg0, M2C_UNK **, 8) = var_v0;
}
