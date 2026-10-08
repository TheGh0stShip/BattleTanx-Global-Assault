/* ---- 0x800E9000/c/ed320.c ---- */
typedef struct { float x, y, z; } Vec;
typedef struct { char pad[2]; unsigned short h2, h4, h6; unsigned char b8, b9; } Desc;
typedef struct {
    char pad[12]; int sub; int h; int w; Vec v; int t36; unsigned short id; unsigned short s42;
    unsigned char b44, b45, b46, b47;
} Obj;
extern Obj *func_800A18D0(int, int);
extern int ****func_800DF758(void *, int, int);
extern unsigned char func_800DF63C(void *, int, int, int, float, int);
extern int func_800DF558(void *, int ****, Vec *, int, int, int, int, int);
extern unsigned short func_800DF89C(void *, Vec *, int, int, int, int, int, Obj *);
extern unsigned char func_800B9C68(int *);
void func_800ED320(Desc *desc, void *ctx, Vec *pos, unsigned short a3, unsigned char b, int s6) {
    Obj *o;
    int ****s7;
    unsigned char r;
    o = func_800A18D0(desc->b9 ? 25 : 24, 48);
    if (o != 0) {
        s7 = func_800DF758(ctx, s6, desc->h2);
        o->h = (int)func_800DF758(ctx, s6, desc->h4);
        o->w = (int)func_800DF758(ctx, s6, desc->h6);
        o->b46 = desc->b8;
        r = func_800DF63C(ctx, s6, desc->h2, 0, pos->z, b);
        o->sub = func_800DF558(ctx, s7, pos, a3, b, 0, 240, r);
        o->id = func_800DF89C(ctx, pos, a3, b, s6, desc->h2, o->b46 ? 0x100002 : 0x100000, o);
        o->t36 = 50;
        o->s42 = a3;
        o->b44 = b;
        o->b45 = 0;
        o->v = *pos;
        if (desc->b9) {
            o->b47 = func_800B9C68(***s7);
        } else {
            o->b47 = 0;
        }
    }
}

