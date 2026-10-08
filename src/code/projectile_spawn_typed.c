/* ---- 0x800E9000/c/ec790.c ---- */
typedef struct {
    char pad[12]; float x, y, z; float f24, f28, f32; int type; int t40;
    unsigned short h44; unsigned char b46, b47, b48; char p49[3]; int i52, i56; float f60, f64;
} Obj;
extern int D_8021945C;
extern float D_8012558C[];
extern float D_80125598[];
extern Obj *func_800A18D0(int, int);
extern float func_8009D4B0(int);
extern float func_8009D510(int);
extern void func_800979F4(int);
void func_800EC790(float *pos, unsigned char a1, unsigned short a2, unsigned char a3, int type, int i52,
                   unsigned char b47, int i56, float f60, float f64) {
    Obj *o = func_800A18D0(36, 68);
    if (o != 0) {
        o->b47 = b47;
        o->type = type;
        o->x = pos[0];
        o->z = pos[2];
        o->y = pos[1];
        o->b48 = a1;
        o->b46 = a3;
        o->i56 = i56;
        o->i52 = i52;
        o->t40 = D_8021945C;
        o->f32 = D_80125598[type];
        o->f24 = func_8009D4B0(a2) * D_8012558C[type];
        o->f28 = func_8009D510(a2) * D_8012558C[type];
        o->h44 = a2;
        o->f60 = f60;
        o->f64 = f64;
        if (o->type == 1) {
            func_800979F4(65);
        }
    }
}

