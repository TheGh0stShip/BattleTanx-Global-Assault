/* ---- trick_sweep/800EDC00.c ---- */
#include "types.h"
typedef struct { u8 pad0[20]; s32 h; u8 pad18[4]; f32 x; f32 y; u8 pad24[6]; u16 a; u8 id; u8 b; u8 pad2e; u8 done; } Obj;
extern void func_800DA4F0(s32, f32 *, u16, u8, u16, u8);
extern void func_800DA7D0(s32, f32 *, u16, u8, s32, s32, s32, s32, s32);
extern s32 func_8009D914(void);
extern void func_80097FB4(s32, f32, f32, f32, u8);
void func_800EDC00(Obj *o, u16 r, s32 flag) {
    s32 b;
    if (o->b == 0xFE) b = 0; else b = o->b;
    if (o->done == 0) {
        if (flag) {
            func_800DA4F0(o->h, &o->x, o->a, o->id, r, b);
        } else {
            func_800DA7D0(o->h, &o->x, o->a, o->id, b, 0, 0, 0, 0);
        }
        if (!(func_8009D914() & 1)) {
            func_80097FB4(4, o->x, o->y, 1.0f, o->id);
        } else {
            func_80097FB4(40, o->x, o->y, 1.0f, o->id);
        }
        o->done = 1;
    }
}

/* ---- 0x800ED800/tu/slot_release_800EDCEC.c ---- */
#include "types.h"
typedef struct { u32 flags; s32 w4; u8 pad[24]; u16 h20; u8 pad2[6]; } Slot;
extern Slot D_803978E0[];
typedef struct { u8 pad[72]; s32 w48; u8 b4C; u8 b4D; } Tgt;
typedef struct { u8 pad[12]; Tgt *t; u8 pad10[8]; s32 w18; u8 pad1c[12]; u16 idx; u8 pad2a[5]; u8 on; u8 pad30[2]; u16 h32; } Ent;
void func_800EDCEC(Ent *e, s32 *out) {
    u8 buf[16];
    if (e->on) {
        e->t->b4C = 0;
        e->t->w48 = e->w18;
        D_803978E0[e->idx].h20 = e->h32;
        D_803978E0[e->idx].w4 = 0;
        { Slot *b = D_803978E0; b[e->idx].flags |= 0x40; b[e->idx].flags &= ~0x8000; }
        *out = 1;
        e->t->b4D = 0xF0;
    } else {
        e->t->b4D |= 0xF0;
    }
}

