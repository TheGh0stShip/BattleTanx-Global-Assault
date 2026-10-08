#include "types.h"

typedef struct { f32 x, y, z; } V3F;
typedef struct Obj4F3C {
    u8 pad00[0x0A];
    u16 id;
    V3F pos;
    u16 h18;
    u8 b1A;
    u8 b1B;
    s32 time;
    u8 b20;
} Obj4F3C;
typedef struct Type4F3C {
    u8 pad00[0x10];
    f32 zoff;
    union { s32 w; struct { s16 hi; s16 lo; } h; } u;
    u8 pad18[0x48];
} Type4F3C;

extern Type4F3C D_80123BC8[];
extern s32 D_8021945C;
extern s32 func_800E29A4(s32, V3F*, s32, s32);
extern s32 func_800E2AEC(s32, V3F*, s32, s32, s32, s32, s32);
extern void *func_800A18D0(s32, s32);
extern u16 func_800B1898(void*, s16, s16, s32, s16, s16, s16, s16, s16, s16, u16, s32, u8);

s32 func_800E4F3C(u8 type, V3F* pos, u16 h, u8 b, u8 c) {
    Obj4F3C* o;
    f32 z;

    if (func_800E29A4(type, pos, h, b) == 0) {
        return 0;
    }
    if (type == 1) {
        return func_800E2AEC(1, pos, h, 0, b, c, 0);
    }
    o = func_800A18D0(41, 36);
    if (o == 0) {
        return 0;
    }
    o->id = func_800B1898(o, pos->x, pos->y, 0,
                          -D_80123BC8[type].u.w, D_80123BC8[type].u.h.lo,
                          -D_80123BC8[type].u.w, D_80123BC8[type].u.h.lo,
                          pos->z - 50.0f, (70.0f < (z = pos->z + D_80123BC8[type].zoff)) ? (s16)z : 70, h, 0x1000, b);
    o->pos = *pos;
    o->b1A = type;
    o->b1B = b;
    o->time = D_8021945C;
    o->h18 = h;
    o->b20 = c;
    return 1;
}
