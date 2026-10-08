/* ---- 0x800F2000/f4ff0.c ---- */
typedef struct { int x, y, z; } V3;
typedef struct { char p[0xc]; V3 a; V3 b; int s24; int s28; char t; } N;
extern int D_8021945C;
extern void *func_800A18D0(int, int);
extern unsigned int func_8009D914(void);
void func_800F4FF0(V3 *a, V3 *b, char t) {
    N *n = func_800A18D0(62, 48);
    if (n != 0) {
        n->a = *a; n->b = *b; n->s24 = 0; n->t = t;
        n->s28 = D_8021945C + func_8009D914() % 5;
    }
}

