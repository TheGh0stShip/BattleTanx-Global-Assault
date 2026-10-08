/* ---- 0x800E9000/r2/eac30.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char p0[2]; unsigned short h2, h4, h6; unsigned char b8, b9; unsigned short h10; } Desc;
typedef struct { char p0[12]; int w12; int w16; int w20; int w24; Vec3 pos; short h40, h42; unsigned char b44, b45, b46, b47; int w48; unsigned char b52; } Obj;
extern Obj *func_800A18D0(int, int);
extern int ***func_800DF758(int, int, int);
extern int func_800DF63C(int, int, int, int, float, unsigned char);
extern int func_800DF558(int, int ***, Vec3 *, unsigned short, unsigned char, int, int, unsigned char);
extern int func_800DF89C(int, Vec3 *, unsigned short, unsigned char, int, int, int, Obj *);
extern unsigned char func_800B9C68(int);

void func_800EAC30(Desc *d, int a1, Vec3 *pos, unsigned short a3, unsigned char a4, int a5) {
    Obj *o;
    int size;
    int ***t;
    int flag;

    size = 22;
    if (d->b8 == 4) size = 32;
    o = func_800A18D0(size, 56);
    if (o == 0) return;
    t = func_800DF758(a1, a5, d->h2);
    o->w16 = (int)func_800DF758(a1, a5, d->h4);
    o->w20 = (int)func_800DF758(a1, a5, d->h6);
    o->w24 = (int)func_800DF758(a1, a5, d->h10);
    {
    unsigned char b = a4;
    unsigned short id;
    int r = func_800DF63C(a1, a5, d->h2, 0, pos->z, b);
    id = a3;
    o->w12 = func_800DF558(a1, t, pos, id, b, 0, 240, r);
    o->h40 = func_800DF89C(a1, pos, id, b, a5, d->h2, (d->b8 == 2 || d->b8 == 3 || d->b8 == 6) ? 2 : 0x8000, o);
    }
    o->h42 = a3;
    o->b44 = a4;
    switch (d->b8) { case 1: case 6: o->b45 = 1; break; default: o->b45 = 0; break; }
    o->b46 = d->b9;
    o->b47 = d->b8;
    o->pos = *pos;
    o->w48 = 150;
    if (o->b47 == 4) {
        o->b52 = func_800B9C68(***t);
        if (o->b52 != 0) return;
    }
    o->b52 = 1;
}

