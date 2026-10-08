#include "types.h"
#define NULL ((void *)0)
typedef struct N { struct N *next, *prev; } N;
typedef struct { s32 a; f32 x, y, z; u16 b, c; u8 d; u8 pad[3]; f32 f; } In;
typedef struct { u8 pad[8]; s32 a; f32 x, y, z; u16 b, c; s32 z0; u8 z1, d; u8 pad2[2]; f32 f; u16 id; } Out;
extern u16 D_801255EC;
extern N *D_801255E4;
extern f32 D_801255F0;
static inline void func_800EFE60(s32 unused, Out *o, In *in) {
    o->a = in->a; o->x = in->x; o->z = in->z; o->y = in->y;
    o->b = in->b; o->c = in->c; o->z0 = 0; o->z1 = 0; o->d = in->d;
    o->f = in->f; o->id = D_801255EC++;
}
typedef struct { u8 pad[12]; s32 *p0C; s32 p10; N *list; f32 x, y, z; u8 pad24[12]; u16 h30; u8 pad32[4]; u16 h36; u8 b38, b39, b3A; s8 count; u8 b3C; } Ent;
typedef struct { s32 *p; f32 x, y, z; u16 h10, h12; u8 b14; u8 pad15; u16 h16; f32 x2, y2, z2; s32 w24; u8 b28; } Par;
N *func_800F0618(N **);
void func_800F0930(N **, N *, N *);
void func_800EFEC4(Ent *e, Par *p) {
    N *m;
    In in;
    e->p0C = p->p;
    e->p10 = *p->p;
    e->count = 0;
    e->b3C = p->h16;
    e->h30 = 0;
    e->list = NULL;
    e->x = p->x2; e->y = p->y2; e->z = p->z2;
    e->b3A = p->b14;
    e->h36 = p->w24;
    e->b38 = p->b28;
    e->b39 = 0;
    m = func_800F0618(&D_801255E4);
    if (m != NULL) {
        in.a = (s32)e; in.x = p->x; in.z = p->z; in.y = p->y;
        in.b = p->h10; in.c = p->h12; in.d = e->b3C; in.f = (u8)D_801255F0;
        e->count++;
        func_800EFE60(0, (Out *)m, &in);
        func_800F0930(&e->list, m, NULL);
    }
}
