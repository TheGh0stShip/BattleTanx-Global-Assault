/* ---- 0x800E9000/r2/ea964.c ---- */
typedef struct { char pad[12]; float x, y; char p2[4]; unsigned short h24; unsigned char b26, b27; char p3[12]; void *p40; char p4[4]; void *p48; unsigned char b52, b53, p54, b55; } Obj;
typedef struct { unsigned char b0; char p[3]; int w4; int w8; int w12; int w16; } Arg;
typedef struct { unsigned char b0; char p[3]; float x, y; } Out;
void func_800EA964(Obj *a0, int a1, Arg *a2, Out *a3) {
    int t;
    if (a2->w4 == 0 && a2->b0 == a0->b26 && a0->b55 != 1) { t = a0->b53; if (t < 4) if (t >= 0) {
        a3->b0 = 1;
        a3->x = a0->x;
        a3->y = a0->y;
    }}
}

