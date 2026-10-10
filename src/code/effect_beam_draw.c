typedef struct { float x, y, z; } V3;
typedef struct { char p[0xa]; unsigned short ti; float x, y, z; unsigned short a; unsigned char b; unsigned char idx; short hp; short maxhp; int m0, m1, m2, m3, m4, m5; char p3[0x42 - 0x38]; unsigned short col; } O;
typedef struct { char p[0x1e4]; unsigned char r, g, b; char p2[0x250 - 0x1e7]; } P;
typedef struct { float v[17]; } Mtx;
typedef struct { unsigned int w0, w1; } Gfx;
extern P D_80235F00[];
static const Mtx D_8007722C = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } };
extern int D_80117EB4;
extern unsigned char func_800ACF20(int, int);
extern int func_800AA058(int, V3 *);
extern void func_8009EFD4(Mtx *, float, float, float, int);
extern float func_8009D4B0(int);
extern float func_8009D510(int);
extern void func_800D0858(int, unsigned char *, unsigned char *, unsigned char *);
extern void func_800AE4D0(int, int, Mtx *, int, int, int, int);
extern void func_800AD9A8(int, int, Mtx *, int, int, int, Gfx *, int);
static inline P *getp(int i) {
    P *r;
    if (i == 127) r = 0; else r = &D_80235F00[i];
    return r;
}
void func_800F5B3C(O *o) {
    Mtx m;
    Gfx tile, col;
    unsigned char rgb[3];
    unsigned char lod;
    P *p;
    int h;
    float s;
    Gfx *tp;
    int t;
    m = D_8007722C;
    lod = func_800ACF20(o->ti, o->b);
    if (lod == 0) return;
    p = getp(o->idx);
    h = func_800AA058(o->b, (V3 *)&o->x);
    func_8009EFD4(&m, o->x, o->z, o->y, o->a);
    s = func_8009D4B0(o->a);
    s = (o->x * s + o->y * func_8009D510(o->a)) * -3.0f;
    if (D_80117EB4 == 10) {
        func_800D0858(o->col, &rgb[0], &rgb[1], &rgb[2]);
        col.w0 = 0xFB000000;
        col.w1 = (rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 0xFF;
    } else {
        col.w0 = 0xFB000000;
        col.w1 = (p->r << 24) | (p->g << 16) | (p->b << 8) | 0xFF;
    }
    t = (int)s;
    tp = &tile;
    tile.w0 = 0xF2000000 | ((t % 64) & 0xFFF);
    tile.w1 = 0x7C0FC;
    func_800AE4D0(o->m5, 0, &m, 0, 5, 0, lod);
    if (o->maxhp / 2 < o->hp) {
        func_800AD9A8(o->m0, h, &m, 0, 0, lod, &col, 1);
        func_800AD9A8(o->m3, h, &m, 0, 0, lod, tp, 1);
    } else {
        func_800AD9A8(o->m2, h, &m, 0, 0, lod, &col, 1);
        func_800AD9A8(o->m4, h, &m, 0, 0, lod, tp, 1);
    }
}
