typedef struct { float m[17]; } M;
typedef struct { char pad[12]; float x, y, z; char p2[12]; int idx; char p3[4]; unsigned short h44; char p4[2]; unsigned char b48; char p5[15]; float sc; } Obj;
extern M D_80076758;
extern float D_8007679C;
extern unsigned short D_801255B0[];
extern int D_80125580[];
extern int D_803A53A0[];
extern unsigned char func_800AD14C(float, float, float, int);
extern int func_800AA058(int, float *);
extern void func_8009EFD4(M *, float, float, float, int);
extern void func_8009F824(M *, float, float, float);
extern void func_800AE4D0(int, int, M *, int, int, int, int);
void func_800ECEBC(Obj *a0) {
    M m;
    int s1;
    int idx = a0->idx;
    unsigned char r;
    m = D_80076758;
    r = func_800AD14C(a0->x, a0->y, D_801255B0[idx], a0->b48);
    if (r) {
        s1 = func_800AA058(a0->b48, &a0->x);
        func_8009EFD4(&m, a0->x, a0->z, a0->y, a0->h44);
        if (a0->sc != D_8007679C) {
            func_8009F824(&m, a0->sc, a0->sc, a0->sc);
        }
        func_800AE4D0(D_803A53A0[D_80125580[idx]], s1, &m, 0, 0, 0, r);
    }
}
