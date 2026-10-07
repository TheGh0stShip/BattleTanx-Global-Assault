#include "types.h"

typedef struct {
    u8 pad_0[8];
    s16 value;
} LayoutChild;

typedef struct {
    u8 pad_0[8];
    LayoutChild* child;
    u8 pad_C[6];
    s16 field_12;
    u8 pad_14[0xE];
    s16 field_22;
} LayoutObject;

void func_800C0C18(LayoutObject* object, s32 value) {
    object->child->value = value + 11;
    object->field_12 = value + 28;
    object->field_22 = value + 32;
}
