/* ---- 0x800F6800/b/src/f7d24.c ---- */
typedef struct { float m[17]; } M44;
typedef struct {
    char pad[12]; float x; float z; float y; unsigned short h18; unsigned char b1a; char p;
    int w1c; int w20; unsigned short h24;
} Obj;
static const M44 D_800773B0 = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } };
extern unsigned char func_800ACF20(int, int);
extern void func_8009EFD4(M44 *, float, float, float, int);
extern int func_8009D914(void);
extern void func_800AE4D0(int, int, M44 *, int, int, int, int);
void func_800F7D24(Obj *o) {
    M44 m = D_800773B0;
    unsigned char r = func_800ACF20(o->h24, o->b1a);
    if (r) {
        func_8009EFD4(&m, o->x, o->y, o->z, o->h18);
        func_800AE4D0(!(func_8009D914() & 1) ? o->w1c : o->w20, 0, &m, 0, 1, 0, r);
    }
}

