/* ---- 0x800ED800/r2/f_800EFE10.c ---- */
#include "types.h"
typedef struct E { struct E *next; u8 pad[40]; } E;
typedef struct { E *head; s32 init; } Pool;
extern E D_803AB550[200];
void func_800EFE10(Pool *p) {
    s32 off;
    E *n;
    E *last;
    if (p->init == 0) {
        n = &D_803AB550[199];
        for (off = 198 * 44; off >= 0; off -= 44) { *(E **)((u8 *)D_803AB550 + off) = n; n--; }
        last = &D_803AB550[199];
        last->next = 0;
        p->head = last - 199;
        p->init = 1;
    }
}

/* ---- 0x800ED800/tu/effect_desc_800EFE60.c ---- */
#include "types.h"
typedef struct { s32 a; f32 x, y, z; u16 b, c; u8 d; u8 pad[3]; f32 f; } In;
typedef struct { u8 pad[8]; s32 a; f32 x, y, z; u16 b, c; s32 z0; u8 z1, d; u8 pad2[2]; f32 f; u16 id; } Out;
extern u16 D_801255EC;
void func_800EFE60(s32 unused, Out *o, In *in) {
    o->a = in->a; o->x = in->x; o->z = in->z; o->y = in->y;
    o->b = in->b; o->c = in->c; o->z0 = 0; o->z1 = 0; o->d = in->d;
    o->f = in->f; o->id = D_801255EC++;
}

