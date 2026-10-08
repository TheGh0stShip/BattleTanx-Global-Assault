/* ---- 0x800DE000/b.c ---- */
typedef struct N { char pad[0x2C]; struct N *next; } N;
void func_800DEAC0(N *a) { N *p = a->next; if (p != a) { while (p->next != a) p = p->next; p->next = a->next; } }

/* ---- 0x800DE000/c.c ---- */
typedef struct A { char pad[0x28]; unsigned char t; } A;
typedef struct B { char pad[0xA]; unsigned char f; } B;
int func_800DEAF8(A *a, B *b) { if (a->t == 4) { if (!(b->f & 4)) return 1; } return 0; }

/* ---- 0x800DE000/d/deb1c.c ---- */
typedef struct O {
    char pad[0xC]; float x, y; unsigned short s20; unsigned char b22, b23; unsigned short h24;
    unsigned char b26, b27; unsigned short h28; int i32; int i36;
    unsigned char b40, b41, b42, b43; struct O *link; int i48, i52;
} O;
typedef struct { char p0[10]; unsigned char b10, b11; char pc[500 - 12]; int i500, i504; unsigned char b508; char p[0x250 - 509]; } P;
typedef struct { float x, y; int i40; short s44; unsigned char b46, b47; int i48, i52, i56, i60, i64, i68; O *o; } L;
extern P D_80235F00[];
extern int D_80117EB4;
extern int D_8021945C;
extern int func_8009D914(void);
extern int func_800A974C(P *);
extern int func_8008F4AC(int, short, short, int, int, int, int, int);
extern int func_8008AE04(L *);
static __inline__ P *getpl(int i) {
    if (i == 127) return 0;
    return &D_80235F00[i];
}
void func_800DEB1C(O *o, int *out) {
    P *pl;
    int n, i, skip;
    L l;
    O *p;
    pl = getpl(o->b23);
    if (o->h24 == 0 || D_80117EB4 == 10) {
        *out = 1;
        pl->b508--;
        return;
    }
    if (o->b43 >= o->b42) return;
    if ((func_8009D914() & 0xF) != 0 && (unsigned)(o->b40 - 2) >= 2) return;
    if (D_8021945C < o->i32 + o->i36) return;
    if (!func_800A974C(pl)) return;
    skip = 0;
    if (o->b40 == 4) if (!(pl->b10 & 4)) skip = 1;
    if (skip) return;
    do {
        if (++o->b26 == 15) o->b26 = 0;
    } while (!((o->h28 >> (n = o->b26)) & 1));
    if (!func_8008F4AC(n, o->x, o->y, o->s20, o->b22, 1, 0, o->i52)) return;
    l.i52 = (pl->b10 & 2) ? 2 : 1;
    l.i48 = n;
    l.i56 = o->b41;
    l.s44 = o->s20;
    l.x = o->x;
    l.i40 = 0;
    l.y = o->y;
    l.b46 = o->b22;
    l.i60 = (o->b40 == 3) << 6;
    l.b47 = pl->b11;
    l.i64 = pl->i500;
    l.i68 = pl->i504;
    l.o = o;
    if (!func_8008AE04(&l)) return;
    o->i32 = D_8021945C;
    o->b43++;
    if (o->h24 != 0xFFFF) {
        o->h24--;
        p = o->link;
        if (p != 0) {
            while (p != o) {
                p->h24--;
                p = p->link;
            }
        }
    }
}

/* ---- 0x800DE000/d/dedac.c ---- */
typedef struct E { int f0; int type; short next; char pa[0x18-0xA]; unsigned short h24; char p1a[0x2C-0x1A]; void *link; char p30[0x44-0x30]; } E;
extern E D_80224EF0[];
extern short D_80235EF0;
extern int func_800A2B74(void *);
void func_800DEDAC(void) {
    short i;
    int j;
    E *e, *f, *last, *cur;
    for (i = D_80235EF0; i != -1; i = e->next) {
        e = &D_80224EF0[i];
        if (e->type == 13) {
            cur = e;
            if (e->link != 0 && func_800A2B74(e->link) == 0 && cur->h24 >= 2) {
                {
                    last = e;
                    for (j = e->next; j != -1; j = f->next) {
                        f = &D_80224EF0[j];
                        if (f->type == 13 && f->link == cur->link) { last->link = f; last = f; }
                    }
                    last->link = cur;
                }
            } else if (cur->h24 < 2) {
                cur->link = 0;
            }
        }
    }
}

/* ---- 0x800DE000/b/f.c ---- */
typedef struct NodeF { int active; int type; char pad[0x18-8]; short s; char p2[0x2C-0x1A]; struct NodeF *next; } NodeF;
void func_800DEEDC(NodeF *n) {
    NodeF *p;
    if (n->active != 0 && n->type == 13) {
        if (n->next != 0) {
            p = n->next;
            if (p != n) {
                while (p->next != n) p = p->next;
                p->next = n->next;
            }
        }
        n->s = 0;
    }
}

/* ---- 0x800DE000/d/def38.c ---- */
typedef struct { int w[17]; } Mtx17;
typedef struct { unsigned int w0, w1; } G;
typedef struct {
    char pad[0xC]; float x, y; unsigned short s20; unsigned char b22, b23; unsigned short h24;
    unsigned char b26, b27; char p28[0x30-0x1C]; void *model;
} Obj;
extern Mtx17 D_80075840;
extern int D_8021945C;
extern int func_800AD14C(float, float, float, int);
extern void func_8009EFD4(Mtx17 *, float, float, float, int);
extern void func_800AD9A8(void *, int, Mtx17 *, int, int, int, G *, int);
void func_800DEF38(Obj *o) {
    Mtx17 m;
    G g[3];
    G *gp;
    int r;
    if (o->model == 0) return;
    r = func_800AD14C(o->x, o->y, 200.0f, o->b22);
    if ((unsigned char)r == 0) return;
    m = D_80075840;
    if (o->b27) {
        g[0].w0 = 0xFA000000; g[0].w1 = 0x00F000FF;
        g[1].w0 = 0xFB000000; g[1].w1 = 0x00F0F000;
    } else {
        g[0].w0 = 0xFA000000; g[0].w1 = 0xF00000FF;
        g[1].w0 = 0xFB000000; g[1].w1 = 0xF0F00000;
    }
    gp = g;
    g[2].w0 = 0xF2000000 | (((D_8021945C % 32) * 4) & 0xFFF);
    g[2].w1 = 0x0007C07C;
    func_8009EFD4(&m, o->x, 0.0f, o->y, o->s20);
    func_800AD9A8(o->model, 0, &m, 1, 0, (unsigned char)r, gp, 3);
}

