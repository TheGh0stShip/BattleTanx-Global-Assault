typedef struct { short p0[11]; short a; short b; short pad; short c; short d; short e; short pad2[3]; } T;
typedef struct { char p[10]; unsigned short ti; float x, y, z; unsigned short u; unsigned char b; } O;
typedef struct { int pad; int type; } H;
typedef struct { H *h; char p[32]; } Hit;
typedef struct { char p[12]; unsigned short u; char p2[22]; } Info;
typedef struct { int v[7]; } R;
typedef struct { void (*fn)(H *, O *, int, Info *, R *); int p[2]; } F;
extern T D_803978E0[];
extern short D_80397650;
static const R D_80077200 = { { 2, 0, 0, 0, 0, 0, 0 } };
extern F D_80224B5C[];
extern unsigned short func_800B205C(int, int, int, int, int, int, int, int, int, int, int, int, int);
extern unsigned short func_800B3748(int, int, Hit *, int, int);
extern void _bzero(void *, int);
int func_800F5548(O *o, float r) {
    Hit hits[32];
    Info info;
    R res;
    T *t;
    unsigned short n; int i;
    Hit *h;
    unsigned short k; Hit *base = hits;
    t = &D_803978E0[o->ti];
    k = func_800B205C(0, (short)o->x, (short)o->y, (short)o->z, t->a, t->b, t->c,
                      (short)(t->c + r), t->d, t->e, o->u, 0, o->b);
    D_80397650 = 0;
    n = func_800B3748(k, 0xF4D50F, base, 0, 0);
    if (n != 0) {
        _bzero(&info, 36);
        info.u = o->u;
        for (i = 0; i < n; i++) {
            h = &base[i];
            if (h->h == (H *)o) continue;
            if (h->h == 0) return 0;
            res = D_80077200;
            if (D_80224B5C[h->h->type].fn != 0)
                D_80224B5C[h->h->type].fn(h->h, o, 0, &info, &res);
            if (res.v[0] == 1) return 0;
        }
    }
    return 1;
}
