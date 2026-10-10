typedef struct { char pad[0x10]; int team; } Owner;
typedef struct {
    char pad0[0x94]; unsigned char b94; char p95[3]; int i98; char p9C[0x150-0x9C];
    float x; float y; char p158[0x1D0-0x158]; Owner *owner;
} Obj;
typedef struct { char pad[8]; unsigned short next; char pA[2]; Obj *obj; char p10[0x44-0x10]; } Node;
extern unsigned short D_80224E70;
extern Node D_80224EF0[];
extern int func_80096250(Obj *);

Obj *func_800E2810(float *pos, unsigned char team, int excl, int maxd, unsigned int mask) {
    short i = D_80224E70;
    float best = 1e+09f;
    Obj *res = 0;
    while (i != -1) {
        Node *n = &D_80224EF0[i];
        Obj *o = n->obj;
        if (o != 0 && o->b94 == team && o->owner->team != excl && func_80096250(o) &&
            !(mask & (1 << o->i98))) {
            float dx = o->x - pos[0];
            float dy = o->y - pos[1];
            float d = dx * dx + dy * dy;
            if (d < best) {
                best = d;
                res = o;
            }
        }
        i = D_80224EF0[i].next;
    }
    if (best < (float)maxd) return res;
    return 0;
}
