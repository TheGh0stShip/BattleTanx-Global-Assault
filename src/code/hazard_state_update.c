#include "types.h"
typedef struct { f32 x, y, z; } V3;
typedef struct {
    u8 pad[12]; V3 pos; u8 b; u8 pad19; u16 r; s32 t; s32 state; f32 vel; s32 timer;
    union { s32 c; struct { u8 c0, c1, c2, c3; } cb; } u; u16 h; u8 k;
} Obj;
extern s32 D_8021945C;
extern f32 D_80219488;
extern u8 D_80115CD4[], D_801155EC[], D_80115868[];
u32 func_8009D914(void);
f32 func_8009D4B0(u16);
f32 func_8009D510(u16);
void func_800B22F8(u16);
void func_800A2C6C(u8, V3 *, s32, s32, s32, s32);
void func_800A5BD8(V3 *, s32, u8, f32, void *, s32);
void func_800DC968(V3 *, u8, f32 *, u16, u8, void *, s32, s32, s32, s32);
void func_800F1D94(Obj *o, s32 *done) {
    f32 v[3];
    u16 s;
    switch (o->state) {
    case 0:
        if (D_8021945C - o->timer >= 46) o->state = 1;
        break;
    case 5:
        if (o->h != 0xFFFF) func_800B22F8(o->h);
        *done = 1;
        break;
    case 2:
        if (D_8021945C - o->timer >= 9) {
            *done = 1;
            func_800A2C6C(o->b, &o->pos, 108, 20, o->u.c, 0xE49D0A);
        }
        break;
    case 3:
        o->pos.z -= 30.0f;
        func_800A5BD8(&o->pos, 0, o->b, 1.0f, D_80115CD4, 0);
        o->pos.z += 30.0f;
        o->vel += -0.5f;
        if (o->vel < 0.0f) {
            o->state = 4;
            o->k = 17;
        } else {
            o->pos.z += o->vel * D_80219488;
        }
        break;
    case 4:
        s = func_8009D914() % 10468 + 1820;
        v[0] = func_8009D510(s) * func_8009D4B0(o->r);
        v[2] = -func_8009D4B0(s);
        v[1] = func_8009D510(s) * func_8009D510(o->r);
        func_800DC968(&o->pos, o->b, v, o->r, o->u.cb.c3, D_801155EC, 15, 0, 0, 7);
        o->r += 11565;
        if (--o->k == 0) {
            *done = 1;
            func_800A5BD8(&o->pos, 0, o->b, 1.0f, D_80115868, 0);
        }
        break;
    }
}
