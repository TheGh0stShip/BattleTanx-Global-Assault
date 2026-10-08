typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct { f32 x, y, z; } Vec3;
typedef struct { u8 pad0[8]; f32 x; f32 y; f32 z; u8 pad14[0x20-0x14]; u16 h20; u8 pad22[0x1DC-0x22]; s32 i1dc; } Src;
typedef struct { s32 i0; u8 pad4[8]; Src *src; } Obj;
typedef struct { u8 pad0[0xC]; Obj *owner; s32 i10; f32 v[2]; f32 dz; s16 h20; u16 h22; f32 f24; f32 f28; u8 b2c; u8 b2d; u8 b2e; } Part;
extern Part *func_800A18D0(s32, s32);
extern void func_800B2364(f32 *, u16, f32 *);
extern s16 func_8009DDF0(f32 *);

static inline u32 f2u(f32 x) { return x; }

s32 func_800F0ED0(Obj *o, Vec3 *pos, s32 n, s16 *out) {
    Src *s = o->src;
    Part *p;
    f32 d[2];
    f32 dz;
    f32 t;

    if (s != 0) {
        p = func_800A18D0(57, 48);
        if (p == 0) {
            return -1;
        }
        d[0] = pos->x - s->x;
        d[1] = pos->y - s->y;
        func_800B2364(d, s->h20, p->v);
        dz = pos->z - s->z;
        p->dz = dz;
        if (dz >= 70.0f) {
            p->h22 = 10923;
        } else if (dz < 20.0f) {
            p->h22 = 0;
        } else {
            p->h22 = (dz - 20.0f) / 50.0f * 10923.0f + 0.0f;
        }
        p->h20 = func_8009DDF0(p->v);
        p->owner = o;
        p->f24 = 10.0f;
        p->i10 = o->i0;
        t = (f32)s->i1dc / 100.0f;
        p->b2c = f2u(t * -127.0f + 255.0f);
        p->b2d = f2u(t * 128.0f + 0.0f);
        p->b2e = f2u(t * 255.0f + 0.0f);
        if (n >= 50) {
            p->f28 = 2.0f;
        } else if (n < 6) {
            p->f28 = 0.5f;
        } else {
            p->f28 = (f32)(n - 5) / 45.0f * 1.5f + 0.5f;
        }
        if (out != 0) {
            *out = func_8009DDF0(d);
        }
    }
    return 0;
}
