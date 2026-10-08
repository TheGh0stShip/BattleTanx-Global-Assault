/* ---- 0x800E0000/f/f21a0.c ---- */
typedef struct { float m[4][4]; int flag; int pad; } MtxEx;
typedef struct { char pad[0x4D]; unsigned char b4D; } Def;
typedef struct {
    char pad0[0xC]; Def *def; void *obj10; char pad14[0x1C-0x14]; void *obj1C;
    char pad20[0x24-0x20]; float x; float y; float z; char pad30[0x30-0x30];
    unsigned short h30; unsigned short h32; unsigned short h34; unsigned char id; char p37;
    unsigned char mode;
} Ent;
extern unsigned char D_8021957C[];
extern void func_8009EEE0(void *);
extern void func_8009EFD4(void *, float, float, float, unsigned short);
extern void func_8009FB68(void *, void *, unsigned short);
extern void func_8009FA04(void *, void *, unsigned short);
extern void *func_800AA058(unsigned char, float *);
extern void func_800AE4D0(void *, void *, void *, int, int, int, int);

void func_800E21A0(Ent *e) {
    float a[4][4];
    float b[4][4];
    float zero;
    MtxEx c;
    unsigned char mode;
    int mask;
    int m8;
    int d;

    mode = e->mode;
    if (mode == 0) return;
    mask = D_8021957C[e->id] & (e->def->b4D >> 4);
    m8 = mask & 0xff;
    if (m8 == 0) return;
    if (mode == 1 && e->obj1C != 0) {
        c.flag = 0;
        func_8009EEE0(&c);
        func_8009EFD4(&c, e->x, e->z, e->y, e->h30);
        func_800AE4D0(e->obj1C, 0, &c, 0, mode, 0, m8);
    } else {
        c.flag = 0;
        func_8009EEE0(a);
        func_8009EFD4(a, e->x, 0.0f, e->y, (e->h30 - e->h32 > 0) ? e->h30 - e->h32 : e->h32 - e->h30);
        func_8009FB68(a, b, e->h34);
        func_8009FA04(b, &c, e->h32);
        c.m[3][1] = e->z;
        func_800AE4D0(e->obj10, func_800AA058(e->id, &e->x), &c, 0, 0, 0, mask);
    }
}

/* ---- 0x800E0000/f2308.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[0xC]; short kind; char pE[2]; } Elem;
typedef struct { char pad[0x14]; Elem *elems; char p18[0x38 - 0x18]; } Group;
typedef struct { int pad; Group *groups; } World;
typedef struct { short p0; unsigned short h2; unsigned short h4; unsigned short h6; unsigned short h8; unsigned short hA; unsigned char bC; } Desc;
typedef struct {
    char pad[0xA]; unsigned short hA; void *p0C; void *p10; void *p14; void *p18; void *p1C;
    float f20; Vec3 pos; unsigned short h30; char p32[2]; unsigned short h34; unsigned char b36;
    unsigned char b37; unsigned char b38; unsigned char b39;
} Turret;
extern Turret *func_800A18D0(int, int);
extern void *func_800DF758(World *, int, unsigned short);
extern void *func_800DF558(World *, void *, Vec3 *, unsigned short, unsigned char, int, int, int);
extern unsigned short func_800B1898(Turret *, short, short, int, short, short, short, short, short, short, unsigned short, int, unsigned char);
extern unsigned char func_800B9C68(int);

void func_800E2308(Desc *d, World *w, Vec3 *pos, unsigned short a3, unsigned char a4, int a5) {
    Turret *o;
    int ***r;
    Elem *el;

    o = func_800A18D0(15, 60);
    if (o != 0) {
        r = func_800DF758(w, a5, d->h2);
        el = &w->groups[a5].elems[d->h2];
        o->p14 = func_800DF758(w, a5, d->h6);
        o->p10 = func_800DF758(w, a5, d->h4);
        o->p18 = func_800DF758(w, a5, d->h8);
        o->p1C = func_800DF758(w, a5, d->hA);
        o->p0C = func_800DF558(w, r, pos, a3, a4, 0, 240, 2);
        o->hA = func_800B1898(o, pos->x, pos->y, 0, -10, 10, -10, 10, 0, el->kind, a3, 2048, a4);
        o->h30 = a3;
        o->b36 = a4;
        o->b38 = 0;
        o->f20 = el->kind;
        o->h34 = 0;
        o->pos = *pos;
        o->b37 = 0;
        if (d->bC != 0) {
            o->b39 = func_800B9C68(***r);
        } else {
            o->b39 = 0;
        }
    }
}

