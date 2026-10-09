/* SPAN 0x80086BB0 */
/* RODATA_VRAM 0x80071540 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct Unit {
    char p0[0xB]; u8 side; char pc[4]; int team; char p14[4]; unsigned int state; char p1c;
    u8 side2; char p1e[0x68 - 0x1E]; int timer; u16 val; u8 blink; char p6f[0x1D0 - 0x6F];
    struct Unit *owner;
} Unit;
typedef struct { char p0[4]; int kind; char p8[4]; Unit *unit; } Ref;
typedef struct { char p0[0x10]; unsigned int type; char p14[4]; Unit *unit; Ref *ref; } Item;
extern char *D_80114680;
extern int D_802194A0;
extern int D_8021945C;
Item *func_800A1A28(Item *, int);
u16 func_800E8358(Unit *);
int func_80095B68(Unit *);
int func_8008723C(Unit *, int, Unit **, int);
void func_80086FF4(Unit *);

int func_80086840(Unit *self) {
    Unit *flag = 0;
    Unit *mine = 0;
    Unit *enemy = 0;
    Unit *best = 0;
    u16 bestv = 0;
    Item *it;
    Unit *u;
    Unit *tgt;
    u16 v;
    int r;

    for (it = func_800A1A28(0, 7); it != 0; it = func_800A1A28(it, 7)) {
        switch (it->type) {
        case 1:
            flag = (Unit *)it;
            break;
        case 0:
            u = it->unit;
            v = func_800E8358(u);
            if ((u->side2 != self->side) & (v > bestv)) {
                best = u;
                bestv = v;
            }
            break;
        case 2:
            if (it->ref->kind == 4) {
                u = it->ref->unit;
                if (func_80095B68(u) != 0) {
                    if (u->owner->team != self->team) {
                        enemy = u;
                    } else if (u->owner == self) {
                        mine = u;
                    }
                }
            }
            break;
        }
    }
    if (mine != 0) {
        tgt = mine;
        r = func_8008723C(self, 6, &tgt, 1);
    } else if (enemy != 0) {
        tgt = enemy;
        r = func_8008723C(self, 3, &tgt, 1);
    } else if (flag != 0) {
        tgt = flag;
        r = func_8008723C(self, 5, &tgt, 1);
    } else if (best != 0) {
        tgt = best;
        r = func_8008723C(self, 4, &tgt, 1);
    } else {
        r = func_8008723C(self, 2, 0, 0);
    }
    return r;
}
void func_80086A20(Unit *o) {
    switch (o->state) {
    case 0:
        if (*(u8 *)(D_80114680 + 0xC06C) == 0) break;
        switch (D_802194A0) {
        case 9:
            break;
        case 1: case 2: case 3: case 11:
            func_8008723C(o, 2, 0, 0);
            break;
        case 4: case 10:
            if (*(int *)(D_80114680 + 0xC070) != o->team) {
                func_8008723C(o, 7, 0, 0);
                break;
            }
        case 14:
        default:
            func_8008723C(o, 1, 0, 0);
            break;
        }
        break;
    case 2:
        if (func_80086840(o) != 0) break;
        func_80086FF4(o);
        break;
    case 6:
        func_80086840(o);
        break;
    case 3: case 4: case 5:
        if (func_80086840(o) != 0) break;
        func_80086FF4(o);
        break;
    case 7:
        if (o->timer <= D_8021945C) {
            o->blink = o->blink == 0;
            o->timer = D_8021945C + 150;
            if (o->blink) o->val = *(u16 *)(D_80114680 + 0xC06E);
        }
        if (D_802194A0 == 10) break;
        func_80086FF4(o);
        break;
    }
}
