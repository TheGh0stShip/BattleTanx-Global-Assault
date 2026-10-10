/* ---- 0x800F2000/f32ec.c ---- */
typedef struct { char p[0x2c]; int hp; char p1[4]; int a; int c; int b; unsigned short d; unsigned char e; } O;
typedef struct { int t; int v; } M;
typedef struct { char p[0x10]; int dmg; int who; } D;
typedef struct { int f; char p[0x3c]; unsigned char e; } R;
extern void func_8009EEE0(R *);
extern void func_8009EFD4(R *, int, int, int, int);
extern void func_800A9A98(int, int);
void func_800F32EC(O *o, M *m, int ev, D *d, R *r) {
    switch (ev) {
    case 1:
        func_8009EEE0(r);
        func_8009EFD4(r, o->a, o->b, o->c, o->d);
        r->e = o->e;
        break;
    case 0:
        switch (m->v) {
        case 11: case 37: case 38: case 50:
            if (o->hp > 0) {
                o->hp -= d->dmg;
                if (o->hp <= 0) func_800A9A98(d->who, 2000);
            }
            r->f = 1;
            break;
        }
        break;
    }
}

/* ---- 0x800F2000/b/f33c0.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[12];
    Vec3 pos;
    unsigned char b;
    char pad25;
    short c;
    int h;
    int z32;
    int z36;
    float f40;
    short r44;
    short r46;
    short t48;
} Obj33;
extern Obj33 *func_800A18D0(int, int);
extern unsigned int func_8009D914(void);
extern float func_8009D8A0(float);
extern int func_800DF758(void *, int, int);
void func_800F33C0(unsigned short *a0, void *a1, Vec3 *a2, short a3, unsigned char a4, int a5) {
    float zz;
    Obj33 *o = func_800A18D0(48, 52);
    if (o != 0) {
        o->h = func_800DF758(a1, a5, a0[1]);
        o->f40 = (func_8009D8A0(0.2f) + 0.9f) * -1e+01f;
        zz = 6e+02f;
        o->z32 = 0;
        o->c = a3;
        o->pos = *a2;
        o->pos.z = zz;
        o->t48 = 30;
        o->b = a4;
        o->z36 = 0;
        o->r44 = func_8009D914() % 182 + 182;
        o->r46 = func_8009D914() % 182 + 182;
    }
}

