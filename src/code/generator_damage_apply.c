/* ---- 0x800F2000/b/f5e18.c ---- */
typedef struct { float x, y, z; } V3;
typedef struct { char p[0xc]; V3 pos; unsigned short a; unsigned char b; unsigned char idx; short hp; char p2[6]; int h; } O;
typedef struct { char p[0x1e4]; unsigned char r, g, b; char p2[0x250 - 0x1e7]; } P;
extern P D_80235F00[];
extern char D_80115BC8[];
extern void func_800DA7D0(int, V3 *, int, int, int, int, int, int, int);
extern void func_800A5BD8(V3 *, int, int, float, void *, int);
extern void func_800A9A98(int, int);
static inline P *getp(int i) {
    P *r;
    if (i == 127) r = 0; else r = &D_80235F00[i];
    return r;
}
void func_800F5E18(O *o, int dmg, int who) {
    P *p;
    if (o->hp > 0) {
        o->hp -= dmg;
        if (o->hp <= 0) {
            p = getp(o->idx);
            func_800DA7D0(o->h, &o->pos, o->a, o->b, 0, 0, p->r, p->g, p->b);
            func_800A5BD8(&o->pos, o->a, o->b, 1.0f, D_80115BC8, 0);
            if (who != 0) func_800A9A98(who, 1000);
        }
    }
}

