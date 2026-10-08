typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[10];
    short s10;
    Vec3 pos;
    unsigned short u24;
    unsigned char b26;
    unsigned char b27;
    short s28;
    short s30;
    int h[6];
    int z56;
    float f60;
    int z64;
} Obj52;
typedef struct { unsigned char b0; unsigned char b1; unsigned short m[7]; } Mdl52;
extern int D_80117EB4;
extern int D_802194A0;
extern unsigned char D_80219582[];
extern unsigned char D_803AD950[];
extern Obj52 *func_800A18D0(int, int);
extern int func_800DF758(void *, int, int);
extern short func_800DF89C(void *, Vec3 *, int, int, int, int, int, Obj52 *);
extern int func_8009D144(void);
void func_800F5298(Mdl52 *m, void *a1, Vec3 *pos, unsigned short a3, unsigned char a4, int a5) {
    Obj52 *o;
    short v;
    if (D_80117EB4 == 10) goto ok;
    if (D_802194A0 == 4) goto ok;
    if (D_802194A0 != 10) return;
ok:
    {
        o = func_800A18D0(28, 68);
        if (o != 0) {
            o->h[0] = func_800DF758(a1, a5, m->m[0]);
            o->h[1] = func_800DF758(a1, a5, m->m[1]);
            o->h[2] = func_800DF758(a1, a5, m->m[3]);
            o->h[3] = func_800DF758(a1, a5, m->m[4]);
            o->h[4] = func_800DF758(a1, a5, m->m[5]);
            o->h[5] = func_800DF758(a1, a5, m->m[6]);
            o->pos = *pos;
            o->u24 = a3;
            o->b26 = a4;
            o->b27 = D_80219582[m->b1];
            o->s10 = func_800DF89C(a1, pos, a3, a4, a5, m->m[0], 0x40000, o);
            o->z56 = 0;
            o->z64 = 0;
            o->f60 = ((unsigned char *)m)[6];
            v = func_8009D144() ? 50 : 250;
            o->s30 = v;
            o->s28 = v;
            D_803AD950[o->b27]++;
        }
    }
}
