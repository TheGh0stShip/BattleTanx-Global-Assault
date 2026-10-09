#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK func_80086208(void *, s32);                 /* static */
s32 func_8008F3FC(u16, M2C_UNK, M2C_UNK, M2C_UNK);  /* static */

void func_80085D58(void *arg0) {
    if (func_8008F3FC(M2C_FIELD(arg0, u16 *, 0x1F4), 0x2C7007, 0, 0) == 0) {
        func_80086208(arg0, M2C_FIELD(arg0, s32 *, 0x18C));
    }
}
