/* ---- 0x800E0000/f/f462c.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char p0[0x10]; int team; char p14[0x250 - 0x14]; } Player;
typedef struct { float f0; int i4; char p8[0x60 - 8]; } Info;
typedef struct { char p0[0x1E]; unsigned char state; char p1f; unsigned char kind; char p21; unsigned char owner; char p23[0x2C - 0x23]; int time; char p30[0x38 - 0x30]; int count; } Obj;
typedef struct { char p0[0x62]; unsigned char a; unsigned char b; } Cfg;
extern Player D_80235F00[];
extern Info D_80123BD8[];
extern Cfg *D_8021958C;
extern int D_8021945C;
extern int D_801155EC;
extern void func_800EB720(Vec3 *, unsigned char, float, int);
extern void func_80097FB4(int, float, float, float, int);
extern int func_80096294(int, int, int, int, int *);
extern void func_800F6ED0(int, float, Vec3 *, int);
extern void func_800A9B64(Player *, int);
extern void func_800A5BD8(Vec3 *, int, int, float, void *, int);

static inline Player *getp(int i) {
    if (i == 127) {
        return 0;
    }
    return &D_80235F00[i];
}

void func_800E462C(Obj *o, Vec3 *pos, int c, int unused, int who, int p5, int p6) {
    int t;
    float f;
    Player *p;
    unsigned char cc;

    t = 0;
    if (o->owner != 127) {
        p = &D_80235F00[o->owner];
        if (p->team == (getp(who))->team) {
            return;
        }
    }
    cc = c;
    func_800EB720(pos, cc, 0.4f, 12);
    func_80097FB4(18, pos->x, pos->y, f = 1.0f, cc);
    if (func_80096294(D_8021958C->a, D_8021958C->b, p5, p6, &t) != 0) {
        Vec3 v; float g;
        g = D_80123BD8[o->kind].i4;
        v.x = pos->x;
        g = g * 1.5f;
        v.z = pos->z + D_80123BD8[o->kind].f0 / 2.0f;
        v.y = pos->y;
        func_800F6ED0(0, g, &v, cc);
        func_800F6ED0(0, g, &v, cc);
        func_800F6ED0(0, g, &v, cc);
        o->owner = who;
        o->count = 0;
        o->state = 0;
        func_800A9B64(getp(who), 1);
    } else {
        Vec3 w;
        w.x = pos->x;
        w.z = pos->z + 3e+01f;
        w.y = pos->y;
        func_800A5BD8(&w, 0, cc, f, &D_801155EC, 0);
        o->state = 3;
        o->count = 0;
        o->time = D_8021945C + t;
    }
}

