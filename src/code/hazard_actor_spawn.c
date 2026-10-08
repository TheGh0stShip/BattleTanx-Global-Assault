/* ---- 0x800ED800/d/f_800F1B30.c ---- */
#include "types.h"
typedef struct { f32 x, y, z; } V3;
typedef struct {
    u8 pad[12]; V3 pos; u8 b; u8 pad19; u16 r; s32 t; s32 zero; u8 pad24[4]; s32 g; s32 c; s16 h; u8 pad32[2]; s32 rnd;
} Obj;
extern s32 D_8021945C;
extern u16 D_80125AA0[];
void *func_800A18D0(s32, s32);
u32 func_8009D914(void);
s32 func_800B02D4(f32, f32, s32);
s16 func_800B1898(void *, s16, s16, s16, s16, s16, s16, s16, s32, s32, s32, s32, s32);
void func_800F1B30(V3 *pos, u8 b, s32 t, s32 c) {
    Obj *o;
    if (func_800B02D4(pos->x, pos->y, b) == 0) return;
    o = func_800A18D0(34, 56);
    if (o == 0) return;
    o->zero = 0;
    o->t = t;
    o->pos = *pos;
    o->b = b;
    o->c = c;
    o->g = D_8021945C;
    o->r = func_8009D914() % 0xFFFF;
    o->h = func_800B1898(o, pos->x, pos->y, pos->z, -D_80125AA0[t], D_80125AA0[t], -D_80125AA0[t], D_80125AA0[t],
                         0, 10, 0, 256, b);
    o->rnd = func_8009D914();
}

