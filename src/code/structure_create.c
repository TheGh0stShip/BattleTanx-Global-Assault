/* ---- 0x800DC000/b/func_800DD3A0.c ---- */
typedef float f32; typedef unsigned char u8; typedef unsigned short u16; typedef short s16;
typedef struct { f32 x, y, z; } Vec3;
typedef struct { s16 a0, a2, x4, x6, x8, x10, x12, x14; } R;
typedef struct { char p0[20]; R *recs; char p1[56 - 24]; } E;
typedef struct { int p0; E *tab; } W;
typedef struct { u16 h0, h2, h4, h6, h8, h10, h12, h14, h16; u8 b18, b19; } D;
typedef struct { char p0[72]; int i72; char p1[2]; u8 b78; } S;
typedef struct { s16 h; char p[38]; } T;
typedef struct {
    char p0[10]; u8 b10, b11; Vec3 pos; u16 rz; u8 c, b27;
    int p28, p32, p36, p40, p44, p48, p52; S *p56; u8 b60, b61, b62, b63; u16 h64; s16 h66;
} O;
extern T D_80397900[];
extern O *func_800A18D0(int, int);
extern int func_800DF758(W *, int, u16);
extern u8 func_800DD290(int);
extern u8 func_800DF63C(W *, int, u16, u8, f32, u8);
extern S *func_800DF558(W *, int, Vec3 *, u16, u8, int, int, u8);
extern u16 func_800DF89C(W *, Vec3 *, u16, u8, int, u16, int, O *);
O *func_800DD3A0(D *d, W *w, Vec3 *pos, u16 rz, u8 c, int k) {
    O *o;
    int t;
    R *r;
    int dx;
    u8 v;
    o = func_800A18D0(12, 68);
    if (o == 0) return 0;
    t = func_800DF758(w, k, d->h2);
    o->p28 = func_800DF758(w, k, d->h4);
    o->p32 = func_800DF758(w, k, d->h6);
    o->p36 = func_800DF758(w, k, d->h8);
    o->p40 = func_800DF758(w, k, d->h10);
    o->p44 = func_800DF758(w, k, d->h12);
    o->p48 = func_800DF758(w, k, d->h14);
    o->p52 = func_800DF758(w, k, d->h16);
    o->b11 = func_800DD290(t);
    o->b61 = 0;
    o->b60 = 0;
    o->pos = *pos;
    o->rz = rz;
    o->c = c;
    o->b27 = 15;
    v = func_800DF63C(w, k, d->h2, d->b18, pos->z, c);
    o->p56 = func_800DF558(w, t, pos, rz, c, 2, 240, v);
    o->h64 = func_800DF89C(w, pos, rz, c, k, d->h2, 2, o);
    r = &w->tab[k].recs[d->h12];
    o->b63 = r->x12;
    r = &w->tab[k].recs[d->h6];
    o->b62 = r->x12;
    o->h66 = -1;
    if (d->b19) {
        o->b61 = 5;
        o->p56->i72 = o->p32;
        D_80397900[o->h64].h = o->b62;
        o->p56->b78 &= 0x7F;
    }
    r = &w->tab[k].recs[d->h2];
    dx = r->x10 - r->x4;
    if (dx == 288 && r->x14 - r->x8 == dx && (r->x12 - r->x6 == 210 | r->x12 - r->x6 == 315)) {
        o->b10 = 2;
    } else if (r->x4 == r->x10 - 576 && r->x8 == r->x14 - 288 && (r->x12 - r->x6 == 210 | r->x12 - r->x6 == 315)) {
        o->b10 = 4;
    }
    return o;
}

