/* ---- 0x800F6800/src/f85f0.c ---- */
typedef struct { int x, y, z; } V3;
typedef struct { char pad[12]; V3 pos; unsigned char b18, b19; char pad1a[2]; int i1c; } Obj;
extern Obj *func_800A18D0(int, int);
void func_800F85F0(int a, int b, V3 *pos, int d, unsigned char e) {
    Obj *o = func_800A18D0(33, 32);
    if (o) {
        o->i1c = 40;
        o->pos = *pos;
        o->b18 = e;
        o->b19 = 0;
    }
}

