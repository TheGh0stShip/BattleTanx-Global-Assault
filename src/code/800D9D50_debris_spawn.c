/* Unit 0x800D9D50..0x800DA1D8 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * func_800D9D50 (0x488): debris piece spawn with 17 parameters. Written in this lane from the ROM listing (m2c draft
 * as a guide); no prior candidate existed.
 * Rodata: this unit emits the seven float literals 2.0 0.2 0.9 -2.0 2.5 8.0 30.0 at 0x8007536C..0x80075388 (0x1C).
 * The bounds-init vectors (D_80075310/D_8007531C) and the identity matrix (D_8007532C) are the pool constants that
 * production's model_bounds_center.c already references as externs (same constants shared by the original
 * translation unit, which also held func_800D9B50/func_800D9C38 and func_800DA1D8).
 * Shapes that mattered (strategy/CAUSES.md R28-R31): struct copies before the gfx pointer load, indexed loop over the
 * display list, per-branch velocity stores, call-first products in the else branch, field update order x,z,y then
 * centre x,z,y.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800DA1D8 */
/* RODATA_VRAM 0x8007536C */
typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef unsigned int u32;
typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 m[4][4]; } Mtx;
typedef union { struct { u8 cmd; u8 b1; u8 b2; u8 b3; } b; u32 w; } GfxW0;
typedef struct { GfxW0 w0; u32 w1; } Gfx;
typedef struct { u8 pad[8]; Gfx *gfx; } Elem12;
typedef struct {
    u8 pad0[0xC];
    Vec3f pos;      /* 0x0C */
    Vec3f center;   /* 0x18 */
    f32 vx;         /* 0x24 */
    f32 vz;         /* 0x28 */
    f32 vy;         /* 0x2C */
    u16 unk30;
    u16 unk32;
    u16 unk34;
    u16 unk36;
    u16 unk38;
    u8 unk3A;
    u8 flags;       /* 0x3B */
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 unk3F;
    Elem12 *model;  /* 0x40 */
} Debris;

extern const Vec3f D_80075310;   /* { 1e6, 1e6, 1e6 } */
extern const Vec3f D_8007531C;   /* { -1e6, -1e6, -1e6 } */
extern const Mtx D_8007532C;     /* identity */
/* float literals are emitted by this unit: 2.0 0.2 0.9 -2.0 2.5 8.0 30.0 at 0x8007536C.. */

extern Debris *func_800A18D0(s32, s32);
extern void func_800D9B50(Vec3f *, Vec3f *, u32, u32);
extern u32 func_8009D914(void);
extern f32 func_8009D4B0(u16);
extern f32 func_8009D510(u16);
extern f32 func_8009D8A0(f32);
extern void func_8009F090(Mtx *, u16, u16, u16);
extern void func_8009F288(Mtx *, Vec3f *, Vec3f *);

static inline void model_center(Vec3f *center, Elem12 *model) {
    Vec3f mn;
    Vec3f mx;
    Gfx *g;
    s32 i;

    mn = D_80075310;
    mx = D_8007531C;
    g = model->gfx;
    for (i = 0; g[i].w0.w != 0xDF000000; i++) {
        if (g[i].w0.b.cmd == 1) {
            func_800D9B50(&mx, &mn, g[i].w1, (g[i].w0.w >> 12) & 0xFF);
        }
    }
    center->x = (mn.x + mx.x) / 2.0f;
    center->z = (mn.z + mx.z) / 2.0f;
    center->y = (mn.y + mx.y) / 2.0f;
}

s32 func_800D9D50(Vec3f *pos, u8 a1, u16 a2, u16 a3, u16 a4, u16 a5, u16 a6, u16 a7, u16 a8, u16 a9, f32 scale,
                  Elem12 *model, s32 a12, u8 flags, u8 a14, u8 a15, u8 a16) {
    Vec3f out;
    Mtx m;
    Debris *o;
    s32 ang;
    u16 ang2;
    f32 c;

    m = D_8007532C;
    o = func_800A18D0(0x40, 0x44);
    if (o == 0) {
        return -1;
    }
    o->pos = *pos;
    o->unk3C = a1;
    o->flags = flags;
    o->unk36 = a5;
    o->unk38 = a6;
    o->model = model;
    o->unk3A = a12;
    if (model != 0) {
        model_center(&o->center, model);
    } else {
        o->center.x = 0.0f;
        o->center.z = 0.0f;
        o->center.y = 0.0f;
    }
    ang = (a7 + (func_8009D914() & 0x1FFF)) - 0x1000;
    ang2 = a9 + func_8009D914() % (a8 - a9);
    c = func_8009D4B0(ang2);
    if (flags & 8) {
        o->vy = (func_8009D8A0(0.2f) + 0.9f) * -2.0f;
        o->vx = func_8009D4B0(ang) * 2.5f * (func_8009D8A0(0.2f) + 0.9f);
        o->vz = func_8009D510(ang) * 2.5f * (func_8009D8A0(0.2f) + 0.9f);
    } else {
        f32 t;
        t = func_8009D4B0(ang);
        o->vx = c * 8.0f * t * scale;
        o->vy = func_8009D510(ang2) * 8.0f;
        t = func_8009D510(ang);
        o->vz = c * 8.0f * t * scale;
    }
    func_8009F090(&m, a4, a3, a2);
    func_8009F288(&m, &o->center, &out);
    o->pos.x += out.x;
    o->pos.z += out.z;
    o->pos.y += out.y;
    o->center.x = -o->center.x;
    o->center.z = -o->center.z;
    o->center.y = -o->center.y;
    o->unk30 = a2;
    o->unk32 = a3;
    o->unk34 = a4;
    if (a14 || a15 || a16) {
        o->unk3D = a14;
        o->unk3E = a15;
        o->unk3F = a16;
        o->flags |= 0x20;
    }
    if (flags & 8) {
        o->pos.z -= func_8009D8A0(30.0f);
    }
    return 0;
}
