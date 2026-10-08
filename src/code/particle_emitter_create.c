/* ---- 0x800ED800/tu/effect_spawn_800EFC70.c ---- */
#include "types.h"
#define NULL ((void *)0)
typedef struct {
    s32 a;
    f32 pos[3];
    s16 b;
    s16 pad;
    u8 c;
    s16 type;
    f32 v[2];
    f32 f;
    s32 d;
    u8 e;
} Desc;
extern u8 D_801255E4[];
void *func_800A18D0(s32, s32);
void func_800F09C0(void);
void func_800EFE10(void *);
void func_800EFEC4(void *, Desc *);
void *func_800EFC70(f32 *pos, s32 c, s32 unused, s16 b, f32 *v, f32 f, s32 a, s32 d, u8 e) {
    Desc desc;
    void *obj = func_800A18D0(35, 64);
    if (obj == 0) return 0;
    func_800F09C0();
    desc.a = a;
    desc.pos[0] = pos[0];
    desc.pos[2] = pos[2];
    desc.pos[1] = pos[1];
    desc.c = c;
    desc.b = b;
    desc.type = 26;
    desc.v[0] = v[0];
    desc.v[1] = v[1];
    desc.f = f;
    desc.d = d;
    desc.e = e;
    func_800EFE10(D_801255E4);
    func_800EFEC4(obj, &desc);
    return obj;
}
s32 func_800F0064(void *);
void func_800EFD74(void *a, s32 *out) { if (func_800F0064(a) == 0) *out = 1; }
void func_800F01F0(void);
void func_800EFDA8(void) { func_800F01F0(); }
typedef struct { u8 pad[0x18]; f32 r0, r1, s; f32 px, py, pz; s16 flag, a, b; } S;
void func_800EFDC8(S *o, f32 *pos, s32 a, s32 b, f32 *rot, f32 s) {
    o->flag = 1;
    o->px = pos[0]; o->pz = pos[2]; o->py = pos[1];
    o->a = a; o->b = b;
    o->r0 = rot[0]; o->r1 = rot[1]; o->s = s;
}

