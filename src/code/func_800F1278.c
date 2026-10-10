#include "types.h"

typedef struct P { s32 f0; u8 pad[8]; struct Q *q; } P;
typedef struct Q { u8 pad[8]; f32 x; f32 y; f32 z; u8 pad2[12]; u16 rot; u8 pad3[0x72]; u8 f94; } Q;
typedef struct A {
    u8 pad[12]; P *p; s32 f10; f32 v[2]; f32 z; u16 r20; u16 r22; f32 t; s32 f28; u8 c0; u8 c1; u8 c2;
} A;

extern u32 D_803A57A4[];
extern void func_800B2364(f32 *, u16, f32 *);
extern u8 func_800AD14C(f32, f32, f32, u8);
extern void func_8009EEE0(f32 *);
extern void func_8009EF30(f32 *, f32 *);
extern void func_8009FB68(f32 *, f32 *, u16);
extern void func_8009FA04(f32 *, f32 *, u16);
extern void func_8009F444(f32 *, f32 *, s32);
extern void func_800AD9A8(u32, s32, f32 *, s32, s32, u8, u32 *, s32);

static __inline__ f32 lerp(f32 t, f32 c) {
    return (t - 3.0f) / 7.0f * (255.0f - c) + c;
}

void func_800F1278(A *a) {
    Q *q = a->p->q;
    u32 g[8];
    f32 pos[3];
    f32 m[17] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    f32 n[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    u8 vis;
    f32 f;
    u8 alpha, r, gg, b, pr, pg, pb;

    if (a->p->f0 != a->f10 || q == 0) {
        return;
    }
    func_800B2364(a->v, 0xFFFF - q->rot, pos);
    pos[0] += q->x;
    pos[1] += q->y;
    pos[2] = a->z + q->z;
    vis = func_800AD14C(pos[0], pos[1], 30.0f, q->f94);
    if (vis == 0) {
        return;
    }
    alpha = 90;
    func_8009EEE0(n);
    func_8009EF30(n, pos);
    func_8009FB68(n, m, 0xFFFF - a->r22);
    func_8009FA04(m, n, q->rot + a->r20);
    func_8009F444(n, m, a->f28);
    if (!(a->t >= 5.0f)) {
        alpha = a->t / 5.0f * 90.0f + 0.0f;
    }
    if (a->t >= 7.0f) {
        r = lerp(a->t, a->c0);
        gg = lerp(a->t, a->c1);
        b = lerp(a->t, a->c2);
        pb = pg = pr = lerp(a->t, 0.0f);
    } else {
        r = a->c0;
        gg = a->c1;
        b = a->c2;
        pb = pg = pr = 0;
    }
    g[0] = 0xFC6164A0;
    g[1] = 0xF3FCF2FD;
    g[2] = 0xE200001C;
    g[3] = 0xC8104A50;
    g[4] = 0xFA000000;
    g[5] = (pr << 24) | (pg << 16) | (pb << 8) | 0xFF;
    g[6] = 0xFB000000;
    g[7] = (r << 24) | (gg << 16) | (b << 8) | alpha;
    func_800AD9A8(D_803A57A4[0], 0, m, 1, 0, vis, g, 4);
}
