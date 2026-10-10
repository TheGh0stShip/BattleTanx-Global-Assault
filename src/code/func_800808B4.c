#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
void *Steps_InitStep_Free(u16);                           /* extern */

u16 func_800808B4(void *arg0, void **arg1) {
    u16 temp_s0;
    void *temp_v0;

    temp_s0 = M2C_FIELD(arg0, u16 *, 0x104);
    if (temp_s0 == 0) {
        return 0U;
    }
    temp_v0 = Steps_InitStep_Free(temp_s0);
    M2C_FIELD(arg0, u16 *, 0x104) = (u16) M2C_FIELD(temp_v0, u16 *, 0x10);
    M2C_FIELD(temp_v0, u16 *, 0x10) = 0U;
    if (arg1 != NULL) {
        *arg1 = temp_v0;
    }
    return temp_s0;
}
