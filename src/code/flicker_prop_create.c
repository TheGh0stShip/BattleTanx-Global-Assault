/* ---- 0x800F6800/b/src/f7c30.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[2]; unsigned short a; unsigned short b; } Src;
typedef struct {
    char pad[12]; Vec3 pos; unsigned short h18; unsigned char b1a; char p1b;
    int w1c; int w20; unsigned short h24;
} Obj;
extern Obj *func_800A18D0(int, int);
extern int func_800DF758(int, int, int);
extern unsigned short func_800DF89C(int, Vec3 *, int, int, int, int, int, Obj *);
void func_800F7C30(Src *s, int a1, Vec3 *v, unsigned short a3, unsigned char a4, int a5) {
    Obj *o = func_800A18D0(30, 40);
    if (o != 0) {
        o->w1c = func_800DF758(a1, a5, s->a);
        o->w20 = func_800DF758(a1, a5, s->b);
        o->h24 = func_800DF89C(a1, v, a3, a4, a5, s->a, 0x100000, o);
        o->h18 = a3;
        o->b1a = a4;
        o->pos = *v;
    }
}

