#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} GridOverlapPoint;

typedef struct {
    s32 unk0;
    s32 unk4;
    u16 link[4];
    s16 x;
    s16 z;
    s16 y;
    s16 dx0;
    s16 dx1;
    s16 dz0;
    s16 dz1;
    u16 unk1E;
    u16 unk20;
    s16 radius;
    u16 unk24;
    u16 grid;
} GridOverlapNode;

extern GridOverlapNode D_803978E0[];
extern u16 D_80397650;
extern u16 func_800B3018(u16 ia, u16 ib, GridOverlapPoint *out, s16 *side);

u16 func_800B33FC(s32 ia, s32 ib, GridOverlapPoint *out, s16 *side) {
    GridOverlapNode *a;
    GridOverlapNode *b;
    s16 dx;
    s16 dz;
    s32 radius;

    a = &D_803978E0[ia];
    ia = (u16)ia;
    ib = (u16)ib;
    b = &D_803978E0[ib];
    {
        s16 bx = b->x;
        dx = a->x - bx;
    }
    dz = a->z - b->z;
    {
        s16 aradius = a->radius;
        radius = aradius + b->radius;
    }
    if ((u32)(radius * radius) < (u32)((dx * dx) + (dz * dz))) {
        return 0;
    }
    if (D_80397650 != 0) {
        if ((a->y + (s16)a->unk1E) > (b->y + (s16)b->unk20)) {
            return 0;
        }
        if ((a->y + (s16)a->unk20) >= (b->y + (s16)b->unk1E)) {
            return func_800B3018(ia, ib, out, side);
        } else {
            return 0;
        }
    } else {
        return func_800B3018(ia, ib, out, side);
    }
}
