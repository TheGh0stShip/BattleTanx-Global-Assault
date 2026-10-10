typedef struct { char pad[11]; unsigned char b11; } Q;
typedef struct { char pad[12]; float v[3]; unsigned char b24; char p[3]; int t28; float f32; Q *q; } Obj;
extern int D_8021945C;
extern int D_801150A4;
extern void func_800A5BD8(float *, int, int, float, void *, int);
extern void func_800A2C6C(int, float *, short, int, int, int);
void func_800ED0AC(Obj *a0, int *a1) {
    float f = (D_8021945C - a0->t28) * 0.12f + 1.0f;
    if (a0->f32 < f) {
        func_800A5BD8(a0->v, 0, a0->b24, f, &D_801150A4, 0);
        func_800A2C6C(a0->b24, a0->v, f * 172.0f, f * 1e+01f, a0->q->b11, 0xE49D0A);
        *a1 = 1;
    }
}
