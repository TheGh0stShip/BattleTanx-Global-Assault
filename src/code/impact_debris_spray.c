typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[12]; Vec3 pos; unsigned char b24; char p[3]; int time; float f32; } Ent;
extern int D_8021945C;
extern float D_80076700;
void *func_800A18D0(int, int);
void func_800EB838(Vec3 *, int);
float func_8009D8A0(float);
void func_800EB7BC(Ent *e, int *done) {
    int now = D_8021945C;
    if (e->time < now) { *done = 1; return; }
    while (e->f32 > func_8009D8A0(D_80076700)) {
        func_800EB838(&e->pos, e->b24);
    }
}
