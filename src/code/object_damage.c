typedef struct { char pad0[0x10]; int team; char pad14[0x1D8-0x14]; float div; char pad[0x250-0x1DC]; } Unit;
typedef struct { char pad[0x20]; unsigned char kind; char p21; unsigned char owner; char pad2[0x3C-0x23]; int hp; } Obj;
typedef struct { char pad[0xC]; int v; char p[0x60-0x10]; } KindInfo;
extern Unit D_80235F00[];
extern int D_80123C0C[][24];
extern void func_800A9A98(int *, int);

static inline Unit *getUnit(int id) { if (id == 127) return 0; return &D_80235F00[id]; }

void func_800E4530(Obj *o, int dmg, int *src) {
    if (o->hp > 0) {
        o->hp -= (int)(dmg / getUnit(o->owner)->div);
        if (o->hp < 1 && src != 0) {
            if (src[4] != getUnit(o->owner)->team) {
                func_800A9A98(src, D_80123C0C[o->kind][0]);
            }
        }
    }
}
