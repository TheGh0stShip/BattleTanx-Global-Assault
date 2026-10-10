/* ---- 0x800F2000/b/f38c0.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad[12];
    int a;
    Vec3 pos;
    unsigned char b;
    char pad1;
    short c;
    int time;
    int d;
} Obj38;
extern int D_8021945C;
extern int D_803A5658, D_803A567C, D_803A5680, D_803A5684;
extern Obj38 *func_800A18D0(int, int);
extern int func_8009D914(void);
void func_800F38C0(int a0, Vec3 *a1, int a2, int a3) {
    Obj38 *o = func_800A18D0(63, 40);
    if (o != 0) {
        o->a = a0;
        o->pos = *a1;
        o->b = a2;
        o->c = a3;
        o->time = D_8021945C;
        switch ((unsigned)func_8009D914() & 3) {
        case 0: o->d = D_803A5658; break;
        case 1: o->d = D_803A567C; break;
        case 2: o->d = D_803A5680; break;
        case 3: o->d = D_803A5684; break;
        }
    }
}

/* ---- 0x800F2000/b/f39c8.c ---- */
typedef struct { float w[17]; } M17;
typedef struct { unsigned int hi, lo; } Gfx2;
typedef struct {
    char pad0[16];
    float x, y, z;
    unsigned char b28;
    char pad29;
    unsigned short r30;
    int time;
    int h36;
} Obj39;
static const M17 D_80077130 = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } };
extern int D_8021945C;
extern unsigned char func_800AD14C(float, float, float, int);
extern void func_8009EFD4(M17 *, float, float, float, int);
extern void func_800AD9A8(int, int, M17 *, int, int, int, Gfx2 *, int);
void func_800F39C8(Obj39 *arg0) {
    Obj39 *o;
    Gfx2 g[2];
    M17 m = D_80077130;
    unsigned char vis;
    int t;
    unsigned char a;
    o = arg0;
    vis = func_800AD14C(o->x, o->y, 20.0f, o->b28);
    if (vis) {
        func_8009EFD4(&m, o->x, o->z, o->y, o->r30);
        t = D_8021945C - o->time;
        if (t < 120) {
            if (t < 12) {
                a = (t << 8) / 12;
            } else if (t < 60) {
                a = 255;
            } else {
                a = ~(((t - 60) << 8) / 60);
            }
            g[0].hi = 0xFA000000;
            g[0].lo = a | 0xFF000000;
            g[1].hi = 0xFB000000;
            g[1].lo = 0xFFFF0000;
            func_800AD9A8(o->h36, 0, &m, 1, 0, vis, g, 2);
        }
    }
}

/* ---- 0x800F2000/f3b54.c ---- */
extern int D_8021945C;
void func_800F3B54(int *a, int *b) { if (D_8021945C - a[8] >= 120) *b = 1; }

