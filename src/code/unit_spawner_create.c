/* ---- 0x800DE000/d/de930.c ---- */
typedef struct { int x, y; } V2;
typedef struct {
    char pad[0xC]; V2 pos; short s20; unsigned char b22, b23; unsigned short h24;
    unsigned char b26, b27; unsigned short h28; int i32; int i36;
    unsigned char b40, b41, b42, b43; int i44, i48, i52;
} Obj;
extern void *func_800A18D0(int, int);
extern unsigned int func_8009D914(void);
Obj *func_800DE930(V2 *pos, short a1, unsigned char a2, unsigned char a3, unsigned short a4,
                   unsigned short mask, int a6, unsigned char a7, unsigned char a8, int a9,
                   int a10, int a11, unsigned char a12, unsigned char a13) {
    Obj *o = func_800A18D0(13, 56);
    if (o == 0) return 0;
    o->pos = *pos;
    o->s20 = a1;
    o->b22 = a2;
    o->b23 = a3;
    o->h24 = a4;
    o->b27 = a13;
    do {
        o->b26 = func_8009D914() % 15;
    } while (!((mask >> o->b26) & 1));
    o->h28 = mask;
    o->i32 = -1000;
    o->i36 = a6;
    o->b40 = a7;
    o->b41 = a8;
    o->i44 = a9;
    o->i48 = a10;
    o->i52 = a11;
    o->b42 = a12;
    if (a12 == 0) o->b42 = 255;
    o->b43 = 0;
    return o;
}

/* ---- 0x800DE000/a.c ---- */
typedef struct N { char pad[0x18]; unsigned short c; char p2[0x12]; struct N *next; } N;
void func_800DEA94(N *a) { N *p; for (p = a->next; p != a; p = p->next) p->c--; }

