/* ---- 0x800ED800/tu/segment_place_800EEADC.c ---- */
#include "types.h"
typedef struct { u32 flags; s32 w4; u8 pad[24]; u16 h20; u8 pad2[6]; } Slot;
extern Slot D_803978E0[];
extern u8 D_80115BC8[];
typedef struct { u8 pad[12]; f32 x, y; u8 pad14[4]; f32 z; u8 pad1c[8]; u16 ang; u8 b26; } Tgt;
typedef struct {
    u8 pad[12]; s32 state; Tgt *t; u8 pad14[4]; f32 r; u8 pad1c[2]; u16 w;
    u16 h20, h22; s32 w24; s32 p28; s32 w2C; s32 p30; s32 w34; u8 pad38[4]; u16 h3C;
} Ent;
void func_800B22F8(u16);
f32 func_8009D4B0(u16);
f32 func_8009D510(u16);
void func_800DA7D0(s32, f32 *, u16, u8, s32, s32, s32, s32, s32);
void func_800A5BD8(f32 *, u16, u8, f32, void *, s32);
void func_800EEADC(Ent *e, s32 flag) {
    f32 pos[3];
    Tgt *t = e->t;
    if (e->h22 != 0xFFFF) {
        func_800B22F8(e->h22);
        e->h22 = 0xFFFF;
    }
    pos[0] = t->x + (e->r + (s16)e->w / 2) * func_8009D4B0(t->ang);
    pos[1] = t->y + (e->r + (s16)e->w / 2) * func_8009D510(t->ang);
    pos[2] = t->z;
    if (e->p30 != 0) func_800DA7D0(e->p30, pos, t->ang, t->b26, 0, 0, 0, 0, 0);
    func_800A5BD8(pos, t->ang, t->b26, 1.0f, D_80115BC8, 0);
    e->w34 = flag ? -100 : -1;
}
void func_800EEC2C(Ent *e) {
    f32 pos[3];
    Tgt *t = e->t;
    if (e->h22 != 0xFFFF) D_803978E0[e->h22].h20 = e->h3C;
    e->h20 = e->h3C;
    pos[0] = t->x + (e->r + (s16)e->w / 2) * func_8009D4B0(t->ang);
    pos[1] = t->y + (e->r + (s16)e->w / 2) * func_8009D510(t->ang);
    pos[2] = t->z;
    if (e->p28 != 0) func_800DA7D0(e->p28, pos, t->ang, t->b26, 0, 0, 0, 0, 0);
    func_800A5BD8(pos, t->ang, t->b26, 1.0f, D_80115BC8, 0);
    e->state = 5;
    e->w34 = 200000;
    e->w24 = e->w2C;
}

