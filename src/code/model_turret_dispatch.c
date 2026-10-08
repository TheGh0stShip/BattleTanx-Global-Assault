typedef struct { char pad[12]; float x, y; char p2[4]; unsigned short h24; unsigned char b26, b27; char p3[12]; void *p40; char p4[4]; void *p48; unsigned char b52, b53, p54, b55; } Obj;
typedef struct { unsigned char b0; char p[3]; int w4; int w8; int w12; int w16; } Arg;
typedef struct { unsigned char b0; char p[3]; float x, y; } Out;
typedef struct { int w0; int type; } Ent;
unsigned short func_8009E9C8(Arg *, float *);
void func_800DEEDC(void *);
void func_800DA4F0(void *, float *, int, int, int, int);
static inline void h8f4(Obj *o, Ent *e, Arg *a, int *out) {
    switch (e->type) {
    case 38:
        *out = 5;
        break;
    case 35:
        o->b27 -= a->w16;
        *out = 1;
        break;
    case 11: case 37: case 50:
        o->b27 -= a->w16;
        *out = 1;
        break;
    case 4: case 28: case 66:
        *out = 1;
        break;
    }
}
static inline void h964(Obj *a0, Ent *a1, Arg *a2, Out *a3) {
    int t;
    if (a2->w4 == 0 && a2->b0 == a0->b26 && a0->b55 != 1) { t = a0->b53; if (t < 4) if (t >= 0) {
        a3->b0 = 1;
        a3->x = a0->x;
        a3->y = a0->y;
    }}
}
static inline void h9c0(Obj *a0, Ent *a1, Arg *a2) {
    unsigned short r = func_8009E9C8(a2, &a0->x);
    int t = a0->b53;
    if (t < 4) if (t >= 0) {
        if (a0->p48 != 0) { func_800DEEDC(a0->p48); a0->p48 = 0; }
        func_800DA4F0(a0->p40, &a0->x, a0->h24, a0->b26, r, 0);
        a0->b53 = 4;
        a0->b52 = 255;
    }
}
static inline void ha68(Obj *a0, Ent *a1, Arg *a2) { a0->b27 -= a2->w12; }
void func_800EAA7C(Obj *arg0, Ent *e, unsigned int cmd, Arg *a, void *out) {
    Obj *o = arg0;
    switch (cmd) {
    case 0: h8f4(o, e, a, out); break;
    case 4: h964(o, e, a, out); break;
    case 5: h9c0(o, e, a); break;
    case 3: ha68(o, e, a); break;
    }
}
