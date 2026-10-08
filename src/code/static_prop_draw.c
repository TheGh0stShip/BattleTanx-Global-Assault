/* ---- 0x800F6800/b/src/f7f84.c ---- */
typedef struct { float m[17]; } M44;
typedef struct { float m[16]; } Mtx;
typedef struct {
    char pad[12]; int w0c; float x; float z; float y; unsigned short h1c; unsigned short h1e; unsigned char b20;
} Obj;
extern M44 D_80077400;
extern unsigned char func_800AD14C(float, float, float, int);
extern int func_800AA058(int, float *);
extern void func_8009EF30(M44 *, float *);
extern void func_8009FB68(M44 *, Mtx *, int);
extern void func_8009FA04(Mtx *, M44 *, int);
extern void func_800AE4D0(int, int, M44 *, int, int, int, int);
void func_800F7F84(Obj *o) {
    unsigned char r;
    Mtx t;
    M44 m;
    int k;
    if (o->w0c != 0) {
        r = func_800AD14C(o->x, o->z, 200.0f, o->b20);
        if (r) {
            m = D_80077400;
            k = func_800AA058(o->b20, &o->x);
            func_8009EF30(&m, &o->x);
            func_8009FB68(&m, &t, o->h1e);
            func_8009FA04(&t, &m, o->h1c);
            func_800AE4D0(o->w0c, k, &m, 0, 0, 0, r);
        }
    }
}

