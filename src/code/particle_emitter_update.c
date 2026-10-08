/* ---- 0x800ED800/tu/effect_update_800F0064.c ---- */
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
typedef struct { u8 pad[12]; s32 *p0C; s32 p10; N *list; u8 pad18[12]; f32 x, y, z; u16 h30, h32, h34; u8 pad36[5]; s8 count; u8 b3C; } Ent;
s32 func_800F0658(Ent *, N *);
void func_800F0978(N **, N *);
void func_800F0644(N **, N *);
N *func_800F0618(N **);
void func_800F0930(N **, N *, N *);
void func_800967F0(s32 *);
s32 func_800F0064(Ent *e) {
    N *n = e->list;
    In in;
    while (n != NULL) {
        if (func_800F0658(e, n)) {
            func_800F0978(&e->list, n);
            func_800F0644(&D_801255E4, n);
            n = NULL;
            if (--e->count == 0) {
                if (e->p10 == *e->p0C) func_800967F0(e->p0C);
                return 0;
            }
        } else {
            n = n->next;
        }
    }
    if (e->h30 != 0) {
        N *m = func_800F0618(&D_801255E4);
        e->h30 = 0;
        if (m == NULL) return 1;
        {
            in.a = (s32)e; in.x = e->x; in.z = e->z; in.y = e->y;
            in.b = e->h32; in.c = e->h34; in.d = e->b3C; in.f = D_801255F0;
            func_800EFE60(0, (Out *)m, &in);
            func_800F0930(&e->list, m, e->list);
            e->count++;
        }
    }
    e->h30 = 0;
    return 1;
}

