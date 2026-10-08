#include "types.h"
typedef struct { u8 pad[12]; f32 x; f32 y; u8 p2[16]; u16 ang; } Par;
typedef struct { u8 pad[16]; Par *par; u8 p2[4]; f32 r; u8 p3[2]; s16 w; u8 p4[2]; u16 id; } Obj;
extern f32 func_8009D4B0(u16);
extern f32 func_8009D510(u16);
extern void func_800B14A8(u16, s16, s16);
extern void func_800A2B9C(u16);
void func_800EE7F8(Obj *o) {
    Par *p = o->par;
    f32 r = o->r + (f32)(o->w / 2);
    s32 x = (s16)(p->x + func_8009D4B0(p->ang) * r);
    s32 y = (s16)(p->y + func_8009D510(p->ang) * r);
    func_800B14A8(o->id, x, y);
    func_800A2B9C(o->id);
}
