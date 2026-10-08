/* ---- 0x800F2000/g/f5f10.c ---- */
typedef struct { float x, y, z; } V3;
typedef struct { char p[0xc]; V3 pos; unsigned short a; unsigned char b; unsigned char idx; short hp; char p2[6]; int h; } O;
typedef struct { char p0[0x10]; int id; char p[0x1e4 - 0x14]; unsigned char r, g, b; char p2[0x250 - 0x1e7]; } P;
typedef struct { int a; int type; } M;
typedef struct { char p[0x12]; unsigned short dmg; P *who; } X;
extern P D_80235F00[];
extern char D_80115BC8[];
extern void func_800DA7D0(int, V3 *, int, int, int, int, int, int, int);
extern void func_800A5BD8(V3 *, int, int, float, void *, int);
extern void func_800A9A98(P *, int);
extern void func_800A9B64(P *, int);
static inline P *getp(int i) {
    P *r;
    if (i == 127) r = 0; else r = &D_80235F00[i];
    return r;
}
int func_800F5F10(O *o, M *m, X *x, int *out) {
    P *p;
    P *who;
    int t = m->type;
    switch (t) {
    case 11: case 37: case 38: case 50:
        if (x->who != 0 && x->who->id == getp(o->idx)->id) {
            func_800A9B64(x->who, 7);
        } else {
            func_800A9B64(getp(o->idx), 10);
        }
        {
        short hp = o->hp; unsigned short dmg = x->dmg;
        who = x->who;
        if (hp > 0) {
            o->hp = hp - dmg;
            if (o->hp <= 0) {
                p = getp(o->idx);
                func_800DA7D0(o->h, &o->pos, o->a, o->b, 0, 0, p->r, p->g, p->b);
                func_800A5BD8(&o->pos, o->a, o->b, 1.0f, D_80115BC8, 0);
                if (who != 0) func_800A9A98(who, 1000);
            }
        }
        }
        break;
    }
    *out = 1;
    return 1;
}

