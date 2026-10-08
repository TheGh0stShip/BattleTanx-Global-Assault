#include "types.h"
#include "m2c_macros.h"

f32 func_8009D4B0(s32);
f32 func_8009D510(s32);
extern f32 D_80072648;

void func_8009EFD4(void *arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    void *object = arg0;
    s32 angle;
    f32 sine;
    f32 cosine;
    f32 default_value;

    M2C_FIELD(object, s32 *, 0x30) = arg1;
    M2C_FIELD(object, s32 *, 0x34) = arg2;
    angle = arg4 & 0xFFFF;
    M2C_FIELD(object, s32 *, 0x38) = arg3;
    if (angle != 0) {
        sine = func_8009D510(angle);
        cosine = func_8009D4B0(angle);
        M2C_FIELD(object, f32 *, 0x20) = cosine;
        M2C_FIELD(object, f32 *, 0) = sine;
        M2C_FIELD(object, f32 *, 0x28) = sine;
        M2C_FIELD(object, f32 *, 8) = -cosine;
        return;
    }
    default_value = D_80072648;
    M2C_FIELD(object, f32 *, 8) = 0.0f;
    M2C_FIELD(object, f32 *, 0x20) = 0.0f;
    M2C_FIELD(object, f32 *, 0) = default_value;
    M2C_FIELD(object, f32 *, 0x28) = default_value;
}
