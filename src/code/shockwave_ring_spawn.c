/* ---- 0x800E9000/r2/ed010.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[12]; Vec3 pos; unsigned char b24; char p25[3]; int w28; float f32; int w36; short h40; } Obj;
extern void *func_800A18D0(int, int);
extern int D_8021945C;
void func_800ED010(Vec3 *pos, int a1, short a2, int a3, int a4) {
    Obj *o = func_800A18D0(42, 44);
    if (o != 0) {
        float f = 1e+01f;
        o->pos = *pos;
        o->b24 = a1;
        o->w36 = a4;
        o->h40 = a2;
        o->f32 = f;
        o->w28 = D_8021945C;
    }
}

