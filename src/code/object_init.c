#include "types.h"

typedef struct {
    s16 field_0;
    s16 field_2;
    s16 field_4;
    u8 pad_6[2];
    s32 field_8;
    s32 field_C;
    u8 pad_10[8];
    u8 field_18;
    u8 field_19;
    u8 field_1A;
    u8 field_1B;
} ObjectInit;

void func_800A7290(ObjectInit* object, u8 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5) {
    object->field_0 = 0;
    object->field_2 = 0;
    object->field_4 = 0;
    object->field_8 = 0;
    object->field_18 = arg1;
    object->field_19 = arg2;
    object->field_1A = arg3;
    object->field_C = arg5;
    object->field_1B = arg4;
}
