/* ---- 0x800E9000/h1.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[12]; Vec3 pos; unsigned char b24; char p[3]; int time; float f32; } Ent;
extern int D_8021945C;
extern float D_80076700;
void *func_800A18D0(int, int);
void func_800EB838(Vec3 *, int);
float func_8009D8A0(float);
void func_800EB720(Vec3 *pos, unsigned char b, float f, int t) {
    Ent *e = func_800A18D0(61, 36);
    if (e != 0) {
        e->f32 = f;
        e->pos = *pos;
        e->time = D_8021945C + t;
        e->b24 = b;
        func_800EB838(pos, b);
    }
}

