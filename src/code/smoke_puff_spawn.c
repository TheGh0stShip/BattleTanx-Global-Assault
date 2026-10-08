/* ---- 0x800F6800/src/f7ba4.c ---- */
typedef struct { int x, y, z; } V3;
typedef struct { char pad[12]; V3 pos; unsigned char b18; char pad19[3]; float f1c; unsigned char b20; unsigned char b21; } Obj;
extern Obj *func_800A18D0(int, int);
Obj *func_800F7BA4(V3 *pos, int b, float f) {
    Obj *o = func_800A18D0(59, 36);
    if (o == 0) return 0;
    o->pos = *pos;
    o->f1c = f;
    o->b18 = b;
    o->b20 = 0;
    o->b21 = 0;
    return o;
}

