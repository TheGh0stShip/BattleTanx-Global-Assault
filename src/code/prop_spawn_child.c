#include "types.h"
typedef struct { u8 pad[12]; f32 x; f32 y; u8 p2[4]; f32 z; u8 p3[8]; u16 ang; u8 c; } Par;
typedef struct { u8 pad[16]; Par *par; u8 p2[4]; f32 r; s16 a; s16 w; s16 m; u16 id; } Obj;
extern f32 func_8009D4B0(u16);
extern f32 func_8009D510(u16);
extern u16 func_800B1898(Obj *, s16, s16, s16, s16, s16, s16, s16, s32, s32, u16, s32, u8);
extern void func_800A2B9C(u16);
void func_800EE688(Obj *o) {
    Par *p = o->par;
    f32 r = o->r + (f32)(o->w / 2);
    s32 x = (s16)(p->x + func_8009D4B0(p->ang) * r);
    s32 y = (s16)(p->y + func_8009D510(p->ang) * r);
    o->id = func_800B1898(o, x, y, p->z, -o->a / 2, o->a / 2, -o->w / 2, o->w / 2, 0, o->m, p->ang, 0x200000, p->c);
    func_800A2B9C(o->id);
}
