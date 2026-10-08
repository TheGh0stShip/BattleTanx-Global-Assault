/* ---- 0x800E9000/r2/eae58.c ---- */
typedef struct { char pad[77]; unsigned char b77; } Sub;
typedef struct { char pad[12]; Sub *p; char pad2[52-16]; unsigned char b52; } Obj;
typedef struct { char pad[4]; int type; char pad2[47-8]; unsigned char b47; } Ent;
typedef struct { Ent *e; int pad[8]; } Item;
extern int D_8021945C;
extern short D_80397650;
extern int func_800B3748(int, int, Item *, int, int);
extern void func_800EAF6C(Ent *, int);

void func_800EAE58(Obj *o) {
    int r = D_8021945C % o->b52;
    o->p->b77 &= 0xF0;
    o->p->b77 |= r;
}


