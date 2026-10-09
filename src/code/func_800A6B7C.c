#include "types.h"

typedef struct Func800A6B7CObject {
    u8 pad0[0xA];
    u8 type;
    u8 padB;
    s16 value;
    u8 padE[2];
    f32 x;
    f32 y;
    u8 field18;
    u8 field19;
    u8 pad1A[6];
    s32 field20;
} Func800A6B7CObject;

extern Func800A6B7CObject *func_800A18D0(s32 category, s32 size);

Func800A6B7CObject *func_800A6B7C(
        f32 x, f32 y, s16 value, u8 field19, u32 type, u8 field18) {
    Func800A6B7CObject *object = func_800A18D0(3, 0x24);

    if (object == 0) return 0;
    object->type = type;
    if (type < 3) {
        object->value = value;
        object->x = x;
        object->y = y;
        object->field18 = field18;
        object->field19 = field19;
        object->field20 = 0;
    }
    return object;
}
