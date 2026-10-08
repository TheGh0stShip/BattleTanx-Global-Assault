/* ---- 0x800F6800/src/f7ec0.c ---- */
typedef struct { int x, y, z; } V3;
typedef struct { char pad[12]; int i0c; V3 pos; short h1c; char pad1e[2]; unsigned char b20, b21; } Obj;
typedef struct { short a; unsigned short h2; } Src;
extern Obj *func_800A18D0(int, int);
extern int func_800DF758(void *, int, int);
void func_800F7EC0(Src *s, void *p, V3 *pos, short h, unsigned char b, int e) {
    Obj *o = func_800A18D0(31, 36);
    if (o) {
        o->i0c = func_800DF758(p, e, s->h2);
        o->h1c = h;
        o->b20 = b;
        o->pos = *pos;
        o->b21 = 0;
    }

}

