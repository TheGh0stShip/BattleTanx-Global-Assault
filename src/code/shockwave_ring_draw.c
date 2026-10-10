/* ---- 0x800E9000/c/ed1b0.c ---- */
typedef struct { float m[17]; } M;
typedef struct { char pad[12]; float x, y, z; unsigned char b24; char p[3]; int t28; char p2[8]; unsigned short h40; } Obj;
extern M D_80076758;
extern int D_8021945C;
extern int D_803A5620[];
extern unsigned char func_800AD14C(float, float, float, int);
extern int func_800AA058(int, float *);
extern void func_8009EFD4(M *, float, float, float, int);
extern void func_8009F824(M *, float, float, float);
extern void func_800AE4D0(int, int, M *, int, int, int, int);
void func_800ED1B0(Obj *a0) {
    M m;
    float v[3];
    float f;
    int s;
    unsigned char r;
    float k2, k3;
    m = D_80076758;
    f = (D_8021945C - a0->t28) * 0.12f + (k2 = 1.0f);
    r = func_800AD14C(a0->x, a0->y, f * (k3 = 5.0f), a0->b24);
    if (r) {
        v[0] = a0->x;
        v[1] = a0->y;
        v[2] = a0->z + (f - k2) * k3;
        s = func_800AA058(a0->b24, v);
        func_8009EFD4(&m, v[0], v[2], v[1], a0->h40);
        func_8009F824(&m, f, f, f);
        func_800AE4D0(D_803A5620[0], s, &m, 0, 0, 0, r);
    }
}

