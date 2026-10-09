/* SPAN 0x800A2FBC */
#include "types.h"

typedef struct Vec3Bits800A2F0C {
    s32 x;
    s32 y;
    s32 z;
} Vec3Bits800A2F0C;

typedef struct Record800A2F0C {
    u8 pad00[0x0C];
    s32 owner;
    s32 unk10;
    s32 unk14;
    s32 kind;
    Vec3Bits800A2F0C position;
    s16 angle;
    u8 subtype;
    u8 pad2B;
    s32 unk2C;
    f32 scale;
} Record800A2F0C;

extern Record800A2F0C *func_800A18D0(s32 size, s32 capacity);

void func_800A2F0C(Vec3Bits800A2F0C *position_arg, s32 angle_arg,
                   s32 subtype_arg, s32 owner_arg, s32 kind_arg,
                   f32 scale_arg) {
    register Vec3Bits800A2F0C *position asm("$19");
    register s32 angle asm("$17");
    register s32 subtype asm("$18");
    register s32 owner asm("$16");
    register s32 kind asm("$20");
    register f32 scale asm("$f20");
    Record800A2F0C *record;

    kind = kind_arg;
    scale = scale_arg;
    position = position_arg;
    owner = owner_arg;
    angle = angle_arg;
    subtype = subtype_arg;
    record = func_800A18D0(0x34, 0x34);
    if (record != 0) {
        record->owner = owner;
        record->unk10 = 0;
        record->unk14 = 0;
        record->kind = kind;
        record->position = *position;
        record->angle = angle;
        record->subtype = subtype;
        record->unk2C = 0;
        record->scale = scale;
    }
}
