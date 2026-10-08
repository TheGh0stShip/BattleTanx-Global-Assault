/* ---- 0x800E9000/b/ed63c.c ---- */
typedef struct { char pad[77]; unsigned char b77; } Sub;
typedef struct { char pad[12]; Sub *sub; char p2[47-16]; unsigned char b47; } Obj;
extern int D_8021945C;
void func_800ED63C(Obj *a0) {
    int r = D_8021945C % a0->b47;
    a0->sub->b77 &= 0xF0;
    a0->sub->b77 |= r;
}

