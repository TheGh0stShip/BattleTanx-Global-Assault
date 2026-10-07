#include "types.h"

extern s32 D_8021945C;

typedef struct {
    u16 id;
    u16 pad_2;
    s32 resource;
    u16 flags;
    u16 pad_A;
    f32 value;
    f32 previous;
    f32 target;
} ObjectDefaults;

void func_80081FD8(ObjectDefaults* object) {
    s32 resource = D_8021945C;
    f32 zero;

    object->value = 0.0f;
    zero = object->value;
    object->id = 0xFFFF;
    object->flags = 0;
    object->resource = resource;
    object->target = zero;
    object->previous = zero;
}
