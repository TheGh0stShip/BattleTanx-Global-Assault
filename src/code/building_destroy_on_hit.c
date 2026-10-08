/* ---- 0x800E9000/r2/ea9c0.c ---- */
typedef struct { char pad[12]; float x, y; char p2[4]; unsigned short h24; unsigned char b26, b27; char p3[12]; void *p40; char p4[4]; void *p48; unsigned char b52, b53, p54, b55; } Obj;
typedef struct { unsigned char b0; char p[3]; int w4; int w8; int w12; int w16; } Arg;
unsigned short func_8009E9C8(int, float *);
void func_800DEEDC(void *);
void func_800DA4F0(void *, float *, int, int, int, int);
void func_800EA9C0(Obj *a0, int a1, int a2) {
    unsigned short r = func_8009E9C8(a2, &a0->x);
    int t = a0->b53;
    if (t < 4) if (t >= 0) {
        if (a0->p48 != 0) { func_800DEEDC(a0->p48); a0->p48 = 0; }
        func_800DA4F0(a0->p40, &a0->x, a0->h24, a0->b26, r, 0);
        a0->b53 = 4;
        a0->b52 = 255;
    }
}

