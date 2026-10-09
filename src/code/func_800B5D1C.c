#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
f32 func_8009E948();                                /* extern */

void func_800B5D1C(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f4;

    temp_f4 = M2C_FIELD(arg0, f32 *, 8) * (f32) M2C_FIELD(arg0, u16 *, 0x18);
    M2C_FIELD(arg0, f32 *, 0) = (f32) (M2C_FIELD(arg0, f32 *, 0) * temp_f4);
    M2C_FIELD(arg0, f32 *, 4) = (f32) (M2C_FIELD(arg0, f32 *, 4) * temp_f4);
    M2C_FIELD(arg0, f32 *, 0) = (f32) (M2C_FIELD(arg0, f32 *, 0) + M2C_FIELD(arg1, f32 *, 0));
    M2C_FIELD(arg0, f32 *, 4) = (f32) (M2C_FIELD(arg0, f32 *, 4) + M2C_FIELD(arg1, f32 *, 4));
    temp_f0 = func_8009E948();
    M2C_FIELD(arg0, f32 *, 8) = temp_f0;
    M2C_FIELD(arg0, f32 *, 8) = (f32) (temp_f0 / (f32) M2C_FIELD(arg0, u16 *, 0x18));
}
