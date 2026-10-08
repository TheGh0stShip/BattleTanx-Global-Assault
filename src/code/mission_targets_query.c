typedef struct { int pad; int idx; } Ent;
typedef struct { Ent *e; char pad[32]; } Hit;
typedef struct { void (*fn)(Ent *, void *, int, int, int); int a, b; } Handler;
extern Handler D_80224B5C[];
extern short D_80397650;
extern int func_800B3748(int, int, Hit *, int, int);
typedef struct { char pad[56]; unsigned short id; } Obj;
void func_800EA14C(Obj *a0) {
    Hit buf[32];
    int i, n; Hit *h;
    unsigned short id = a0->id;
    if (id != 0xFFFF) {
        D_80397650 = 0;
        h = buf; n = func_800B3748(id, 0x100000, h, 2, 0) & 0xFFFF;
        for (i = 0; i < n; i++) {
            if (h[i].e != 0) {
                if (D_80224B5C[h[i].e->idx].fn != 0) {
                    D_80224B5C[h[i].e->idx].fn(h[i].e, a0, 0, 0, 0);
                }
            }
        }
    }
}
