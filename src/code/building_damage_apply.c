/* ---- 0x800E9000/g.c ---- */
typedef struct { char pad[27]; unsigned char b27; } Obj;
typedef struct { int w0, w4, w8, w12; } Arg;
void func_800EAA68(Obj *a0, int a1, Arg *a2) { a0->b27 -= a2->w12; }

