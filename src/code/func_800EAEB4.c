/* SPAN 0x800EAF6C */
typedef struct { char pad[77]; unsigned char b77; } Sub;
typedef struct { char pad[12]; Sub *p; char pad2[52-16]; unsigned char b52; } Obj;
typedef struct { char pad[4]; int type; char pad2[47-8]; unsigned char b47; } Ent;
typedef struct { Ent *e; int pad[8]; } Item;
extern int D_8021945C;
extern short D_80397650;
extern int func_800B3748(int, int, Item *, int, int);
extern void func_800EAF6C(Ent *, int);
void func_800EAEB4(unsigned short id, unsigned short arg) {
    Item buf[32];
    Item *p;
    int i;
    int n;

    D_80397650 = 0;
    p = buf;
    n = (unsigned short)func_800B3748(id, 0x8000, p, 2, 0);
    for (i = 0; i < n; i++) {
        if (p[i].e->type == 22 && p[i].e->b47 != 5) func_800EAF6C(p[i].e, arg);
    }
}
