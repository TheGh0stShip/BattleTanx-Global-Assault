/* SPAN 0x80088F84 */
/* RODATA_VRAM 0x800716B8 */
typedef unsigned char u8;
typedef struct { float x, y; } Pt;
typedef struct { char p0[4]; int type; char p8[4]; float x; float y; char p14[8]; short hp; } Thing;
typedef struct Tank Tank;
typedef struct { Pt pos; char p8[0x10]; u8 valid; u8 kind; char p1a[2]; void *obj; } Target;
struct Tank {
    char p0[0xA4]; int bit; int seen; char pac[4]; void *bb0; char pb4[0xD0 - 0xB4]; Target tgt;
    char pf0[0x150 - 0xF0]; float lx; float ly;
};
extern int D_801ACEF0;
int func_80083FCC(void *);
int func_80083FE4(void *);
int func_80095B68(void *);
int func_80088F84(Tank *, void *);
Tank *func_80089DBC(int);
void func_80089008(Tank *);

void func_80088CA8(Tank *e) {
    Target *t = &e->tgt;
    Tank *o;
    Thing *th;

    e->tgt.valid = 0;
    switch (e->tgt.kind) {
    case 0:
        return;
    case 5:
    case 6:
        switch (t->kind) {
        case 5:
            th = t->obj;
            if (func_80083FCC(th) && !func_80083FE4(th)) t->valid = 1;
            break;
        case 6: {
            Thing *p = t->obj;
            if (p != 0 && p->type == 28 && p->hp > 0) {
                t->pos.x = p->x;
                t->pos.y = p->y;
                t->valid = 1;
            }
            break;
        }
        }
        if (t->valid) return;
    case 1:
    case 2:
    case 3:
    case 4:
        if (func_80095B68(e) && (e->seen != 0 || e->bb0 != 0)) {
            switch (t->kind) {
            case 2:
                o = t->obj;
                if (o != 0 && (o->bit & e->seen) && func_80095B68(o)) {
                    t->pos.x = o->lx;
                    t->pos.y = o->ly;
                    t->valid = 1;
                }
                break;
            case 3:
                o = t->obj;
                if ((o->bit & e->seen) && func_80095B68(o)) {
                    t->pos.x = o->lx;
                    t->pos.y = o->ly;
                    t->valid = 1;
                }
                break;
            case 4:
                th = t->obj;
                if (func_80088F84(e, th)) {
                    t->pos.x = th->x;
                    t->pos.y = th->y;
                    t->valid = 1;
                }
                break;
            case 1:
            case 5:
            case 6:
                break;
            }
            if (!t->valid && e->seen != 0) {
                o = func_80089DBC(e->seen & D_801ACEF0);
                if (func_80095B68(o)) {
                    t->kind = 3;
                } else {
                    o = func_80089DBC(e->seen & ~D_801ACEF0);
                    if (!func_80095B68(o)) goto other;
                    t->kind = 2;
                }
                t->obj = o;
                t->pos.x = o->lx;
                t->pos.y = o->ly;
                t->valid = 1;
            }
        other:
            if (!t->valid) {
                th = e->bb0;
                if (func_80088F84(e, th)) {
                    t->kind = 4;
                    t->obj = th;
                    t->pos.x = th->x;
                    t->pos.y = th->y;
                    t->valid = 1;
                } else {
                    e->bb0 = 0;
                }
            }
        }
        if (!t->valid && t->kind != 1) func_80089008(e);
        break;
    }
}
