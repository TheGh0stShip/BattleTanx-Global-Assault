#include "types.h"
#include "m2c_macros.h"

f32 func_8009D4B0(s32);
f32 func_8009D510(s32);
static f32 D_80072644 __attribute__((section(".rodata"))) = 1.0f;

void func_8009EF4C(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *object = arg0;
    s32 angle = arg3 & 0xFFFF;
    f32 sine;
    f32 cosine;
    f32 default_value;

    M2C_FIELD(object, s32 *, 0x30) = arg1;
    M2C_FIELD(object, s32 *, 0x38) = arg2;
    if (angle != 0) {
        sine = func_8009D510(angle);
        cosine = func_8009D4B0(angle);
        M2C_FIELD(object, f32 *, 0x20) = cosine;
        M2C_FIELD(object, f32 *, 0) = sine;
        M2C_FIELD(object, f32 *, 0x28) = sine;
        M2C_FIELD(object, f32 *, 8) = -cosine;
        return;
    }
    default_value = D_80072644;
    M2C_FIELD(object, f32 *, 8) = 0.0f;
    M2C_FIELD(object, f32 *, 0x20) = 0.0f;
    M2C_FIELD(object, f32 *, 0) = default_value;
    M2C_FIELD(object, f32 *, 0x28) = default_value;
}
