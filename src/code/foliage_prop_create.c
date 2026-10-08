/* ---- 0x800E0000/f1540.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[0xC]; short kind; char pE[2]; } Elem;
typedef struct { char pad[0x14]; Elem *elems; char p18[0x38 - 0x18]; } Group;
typedef struct { int pad; Group *groups; } World;
typedef struct { short p0; unsigned short h2; unsigned short h4; unsigned short h6; } Desc;
typedef struct {
    char pad[0xC]; void *p0C; void *p10; void *p14; char p18[0x18 - 0x18];
    unsigned short h18; unsigned short h1A; unsigned char b1C; unsigned char b1D; char p1E[2];
    Vec3 pos;
} NewObj;
extern NewObj *func_800A18D0(int, int);
extern void *func_800DF758(World *, int, unsigned short);
extern void *func_800DF558(World *, void *, Vec3 *, unsigned short, unsigned char, int, int, int);
extern unsigned short func_800DF89C(World *, Vec3 *, unsigned short, unsigned char, int, unsigned short, int, NewObj *);

void func_800E1540(Desc *d, World *w, Vec3 *pos, unsigned short a3, unsigned char a4, int a5) {
    NewObj *o;
    void *r;
    Elem *el;

    o = func_800A18D0(14, 44);
    if (o != 0) {
        r = func_800DF758(w, a5, d->h2);
        o->p10 = func_800DF758(w, a5, d->h4);
        o->p14 = func_800DF758(w, a5, d->h6);
        el = &w->groups[a5].elems[d->h2];
        o->p0C = func_800DF558(w, r, pos, a3, a4, 0, 240, (el->kind > 80) ? 129 : 1);
        o->h18 = func_800DF89C(w, pos, a3, a4, a5, d->h2, (el->kind == 36) ? 0x800000 : 1024, o);
        o->h1A = a3;
        o->b1C = a4;
        o->b1D = 0;
        o->pos = *pos;
    }
}

