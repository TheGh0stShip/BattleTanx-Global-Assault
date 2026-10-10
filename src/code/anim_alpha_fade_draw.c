typedef struct { float x, y, z; } V3f;
typedef struct Obj { char pad0[12]; V3f p; unsigned char k; char p19; unsigned short ang; int w1c; float f20; unsigned char c24; } Obj;
extern int D_803A53B0;
extern unsigned char func_800AD14C(float, float, int, int);
extern void func_800AD6A8(int, V3f *, int, int, int, int, int, int, int, unsigned int *, int, int);
void func_800F7870(Obj *o) {
    unsigned char vis;
    unsigned int a;
    unsigned int g[4];
    float f;
    vis = func_800AD14C(o->p.x, o->p.y, o->w1c, o->k);
    if (vis == 0) return;
    f = o->f20 * 255.0f;
    if (!(f >= 2.1474836e+09f)) a = (int)f;
    else { a = (int)(f - 2.1474836e+09f); a |= 0x80000000; }
    g[0] = 0xFA000000;
    g[1] = (o->c24 << 24) | (o->c24 << 16) | (o->c24 << 8) | (a & 0xff);
    g[2] = 0xFB000000;
    g[3] = (o->c24 << 24) | (o->c24 << 16) | (o->c24 << 8);
    func_800AD6A8(D_803A53B0, &o->p, o->w1c, o->w1c, 0, o->ang, 0, 0, vis, g, 2, 0);
}
