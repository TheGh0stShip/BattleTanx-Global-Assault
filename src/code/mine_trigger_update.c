typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[12];
    int i12;
    int i16;
    float f20, f24;
    char pad28[4];
    float f32, f36, f40, f44;
    union { int i; unsigned char b[4]; } u48;
    unsigned short u52;
    unsigned char b54;
    unsigned char b55;
} Obj;
extern float D_80077040, D_80077044, D_80077048;
extern float func_8009D8A0(float);
extern float func_800B93A4(Vec3 *, int);
extern int func_8009D914(void);
extern void func_80097FB4(int, float, float, float, int);
extern void func_800E4F3C(int, Vec3 *, int, int, int);
extern void func_800F1B30(Vec3 *, int, int, int);
void func_800F2980(Obj *o_, int *done) {
    Obj *o = o_;
    Vec3 v;
    int k;
    unsigned int j;
    
    
    
    v.x = o->f20 + o->f32 * (float)(o->i16 + 1) + o->f40 * ((float)o->i12 - 2.0f) + func_8009D8A0(60.0f) - 30.0f;
    v.y = o->f24 + o->f36 * (float)(o->i16 + 1) + o->f44 * ((float)o->i12 - 2.0f) + func_8009D8A0(60.0f) - 30.0f;
    v.z = func_800B93A4(&v, o->b55);
    if (o->b54) {
        if ((o->i12 + o->i16) % 3 == 0) {
            k = 0;
            j = func_8009D914() & 3;
            switch (j) { case 1: k = 1; break; case 0: break; case 2: k = 2; break; case 3: k = 4; break; }
            func_80097FB4(23, v.x, v.y, 1.0f, o->b55);
            func_800E4F3C(k, &v, o->u52, o->b55, o->u48.b[3]);
        }
    } else {
        func_80097FB4(19, v.x, v.y, 1.0f, o->b55);
        func_800F1B30(&v, o->b55, (func_8009D914() & 15) == 0, o->u48.i);
    }
    if (++o->i12 == 5) {
        o->i12 = 0;
        if (++o->i16 == 9) {
            *done = 1;
        }
    }
}
