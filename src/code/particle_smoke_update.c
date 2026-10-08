typedef unsigned char u8; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad0[0x18]; f32 f18; f32 f1c; f32 f20; u8 pad24[0x32-0x24]; u16 h32; u16 h34; u8 pad36[0x39-0x36]; u8 b39; u8 b3a; } A;
typedef struct { u8 pad0[0xC]; f32 x; f32 y; f32 z; u16 h18; u16 h1a; f32 f1c; u8 b20; u8 b21; u8 pad22[2]; f32 f24; u8 pad28[0]; u16 h28; } B;
typedef struct { f32 x, y, z; } Vec3;
extern u32 func_8009D914(void);
extern f32 func_8009D4B0(u16);
extern f32 func_8009D510(u16);
extern void func_800A5BD8(Vec3 *, s32, s32, f32, void *, s32);
extern void func_800F0B08(B *);
extern f32 D_80219488;
extern u8 D_80116484[];

s32 func_800F0658(A *a, B *b) {
    f32 k, s, c, m, s0, d, s2, c2;
    u16 r;
    s32 t;
    u16 h1, h2;
    Vec3 v;

    b->b21--;
    b->b20 += 9;
    if (b->b21 == 0) {
        if (func_8009D914() % 6 != 0) {
            v = *(Vec3 *)&b->x;
            v.z = 0;
            func_800A5BD8(&v, 0, a->b3a, 1.0f, D_80116484, 0);
        }
        return 1;
    }
    k = 1.0f;
    b->f1c += k;
    s0 = func_8009D4B0((u16)(((26 - b->b21) << 14) / 26));
    d = k - s0;
    b->f24 = s0 + 0.1f;
    k = d + 0.2f;
    t = 26 - b->b21;
    if (t < 7) {
        if (t < 6) {
            b->h18 = a->h32;
            b->h1a = a->h34;
        } else {
            r = (func_8009D914() & 0x1556) + 0xF555;
            b->h18 += r;
            r = (func_8009D914() & 0x1000) + 0xF800;
            b->h1a += r;
        }
    }
    h1 = b->h18;
    h2 = b->h1a;
    if (b->b21 > a->b39) {
        s = func_8009D4B0(h1);
        c = func_8009D510(h2);
        m = k * 19.0f;
        b->x += m * s * c;
        b->z += m * func_8009D4B0(h2);
        s2 = func_8009D510(h1);
        c2 = func_8009D510(h2);
        b->y += m * s2 * c2;
        b->x += a->f18 * (a->f20 * D_80219488);
        b->y += a->f1c * (a->f20 * D_80219488);
    }
    b->z -= 1.0f;
    if ((b->h28 & 7) == 4) {
        a->b39 = 0;
        func_800F0B08(b);
    }
    return 0;
}
