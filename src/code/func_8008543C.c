#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK func_80087EF0(void *, void *);              /* extern */
M2C_UNK func_800909CC(void *, f32, s32, s32);       /* extern */
s32 func_8009D6DC(u16, M2C_UNK);                    /* extern */

void func_8008543C(void *arg0, void *arg1) {
    s32 temp_a2;

    if (M2C_FIELD(arg0, s32 *, 0x90) != 0) {
        if (M2C_FIELD(arg0, s32 *, 0x184) != 0) {
            M2C_FIELD(arg1, f32 *, 4) = (f32) M2C_FIELD(arg0, f32 *, 0x188);
        } else {
            M2C_FIELD(arg1, f32 *, 4) = (f32) -M2C_FIELD(arg0, f32 *, 0x188);
        }
        func_80087EF0(arg0, arg1);
        temp_a2 = func_8009D6DC(M2C_FIELD(arg0, u16 *, 0x20), 0x4000) & 0xFFFF;
        func_800909CC(arg0, M2C_FIELD(arg1, f32 *, 4), temp_a2, temp_a2);
    }
}
