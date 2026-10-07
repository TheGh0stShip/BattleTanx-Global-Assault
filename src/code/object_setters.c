#include "types.h"

typedef struct {
    u8 pad_0[0xC];
    s32 field_C;
    s32 field_10;
    u8 field_14;
    u8 pad_15[3];
    s32 field_18;
    s32 field_1C;
} ObjectState;

extern void func_800A6588(ObjectState* object);

void func_800A6ABC(ObjectState* object, s32 value) {
    object->field_18 = value;
    func_800A6588(object);
}

void func_800A6ADC(ObjectState* object, s32 value) {
    s32 previous = object->field_C;

    object->field_C = value;
    object->field_14 = 0;
    object->field_10 = previous;
    func_800A6588(object);
}

void func_800A6B08(ObjectState* object, s32 value) {
    s32 previous = object->field_C;

    object->field_C = value;
    object->field_14 = 0x40;
    object->field_10 = previous;
    func_800A6588(object);
}

void func_800A6B38(ObjectState* object, s32 value) {
    object->field_10 = value;
    object->field_14 = 0;
    func_800A6588(object);
}

void func_800A6B5C(ObjectState* object, s32 value) {
    object->field_1C = value;
    func_800A6588(object);
}
