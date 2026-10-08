#include "types.h"
typedef struct { u8 pad[12]; f32 x, y; u8 pad14[4]; f32 z; u8 pad1c[8]; u16 ang; u8 b26; } Tgt;
typedef struct Seg {
    u8 pad[16]; Tgt *t; struct Seg *next; f32 r; u8 pad1c[2]; u16 w;
    u8 pad20[2]; u16 h22; u8 pad24[16]; s32 w34;
} Seg;
typedef struct {
    u8 pad[28]; f32 lo, hi; u16 ang; u8 pad26[6]; f32 cur; Seg *list; u16 lim;
    u8 pad36[8]; s16 d;
} Head;
f32 func_8009D4B0(u16);
f32 func_8009D510(u16);
void func_800EE688(Seg *);
void func_800B14A8(u16, s16, s16);
void func_800A2B9C(u16);
void func_800B22F8(u16);
void func_800EE8C0(Head *h, f32 v) {
    f32 vel[3];
    f32 acc = 0.0f;
    Seg *s;
    s16 d;
    d = v - h->cur;
    h->d = d;
    if (d > h->lim) h->d = h->lim;
    else if (d < -h->lim) h->d = -h->lim;
    vel[0] = h->d * func_8009D4B0(h->ang);
    vel[1] = h->d * func_8009D510(h->ang);
    vel[2] = 0.0f;
    h->cur = v;
    for (s = h->list; s != 0; s = s->next) {
        f32 x = v + acc;
        s->r = x;
        if (s->w34 > 0) {
            if (h->lo < x + (s16)s->w && x < h->hi) {
                if (s->h22 == 0xFFFF) {
                    func_800EE688(s);
                } else {
                    Tgt *t = s->t;
                    f32 half = x + (s16)s->w / 2;
                    s32 px = (s16)(t->x + func_8009D4B0(t->ang) * half);
                    s32 py = (s16)(t->y + func_8009D510(t->ang) * half);
                    func_800B14A8(s->h22, px, py);
                    func_800A2B9C(s->h22);
                }
            } else if (s->h22 != 0xFFFF) {
                func_800B22F8(s->h22);
                s->h22 = 0xFFFF;
            }
        }
        acc += (s16)s->w;
    }
}
