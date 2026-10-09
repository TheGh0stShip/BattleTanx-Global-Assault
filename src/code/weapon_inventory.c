/* SPAN 0x8008F3FC */
/* RODATA_VRAM 0x80071AF8 */
typedef struct { char pad[0xC]; int c; char p10[4]; int e; } Bank;
typedef struct { char pad[0xB]; unsigned char b0B; char p0C[4]; Bank *bank; char p14[0x1DC - 0x14]; float rate; char p1E0[0x1FE - 0x1E0]; unsigned char next[1]; } Ctl;
typedef struct { int rate; char pad[0xCC]; } WeaponDef;
typedef struct {
    char pad[0x20]; unsigned short ang20; char p22[2]; unsigned short ang24; char p26[2]; float vel[3]; char p34[0x40 - 0x34];
    unsigned short u40; char p42[0x90 - 0x42]; int w90; char p94; unsigned char b95; char p96[2]; unsigned int kind; int w9C; char pA0[0x1D0 - 0xA0];
    Ctl *ctl; int maxhp; int hp; int shield; int flags; char p1E4[0x1F6 - 0x1E4]; unsigned short ammo[(0x21C - 0x1F6) / 2];
    int sel[1]; int cur; int last[2]; int time; char p230[0x260 - 0x230]; int w260; char p264[0x290 - 0x264]; int w290; int w294; int w298;
} Obj;
extern unsigned char D_801146D4[];
typedef struct { int ammo; char pad[0xCC]; } VAmmo;
extern VAmmo D_80122E48[];
extern unsigned short D_8011F218[];
extern int D_80117EB4;
extern unsigned int func_8009D914(void);
extern void func_80095B90(Obj *, int);
extern void func_800CA620(unsigned char, char *, int);
extern WeaponDef D_80122E58[];
extern int D_8021945C;
extern unsigned int D_802194A0;
extern int D_80117ECC;
extern int D_802195D4;
extern int func_8008C5D8(Obj *, int, int);
extern void func_800B5DA8(float *, float, unsigned short);
extern void func_80098B58(unsigned char, int, int, int);
extern void func_800C924C(unsigned short *, unsigned char, int);

static inline int func_8008E620(Obj *o, int w) {
    if (w == 1) {
        if (o->w90 != 0) {
            return D_80122E58[o->kind].rate / 3.0f;
        }
        return D_80122E58[o->kind].rate;
    }
    return D_801146D4[w];
}
static inline int func_8008E6BC(Obj *o, int w) {
    int ok = 1;

    if (w != 1) {
        ok = D_801146D4[w];
        ok = ok < 11;
    }
    return ok;
}

int func_8008E6E0(Obj *o) {
    int below;

    switch (o->cur) {
    case 2: case 3: case 4: case 8: case 9: case 14:
        below = o->ammo[o->cur] < 15;
        break;
    case 6: case 15:
        below = o->ammo[o->cur] < 60;
        break;
    case 13: case 16:
        below = o->ammo[o->cur] < 6;
        break;
    case 12:
        below = o->ammo[o->cur] < 300;
        break;
    default:
        goto return_zero;
    }
    if (below != 0) {
        goto return_zero;
    }
    return 1;

return_zero:
    return 0;
}

void func_8008E780(Obj *o, int slot, int force, int arg3) {
    int w = o->sel[slot];
    int t;
    float elapsed;
    short k;
    int i;

    if (force == 0 && !func_8008E6BC(o, w)) {
        return;
    }
    if (o->ammo[w] == 0) {
        return;
    }
    elapsed = D_8021945C - o->last[slot];
    t = func_8008E620(o, w);
    if (elapsed < (float)t / o->ctl->rate) {
        if (slot == 0) {
            return;
        }
        if (!(o->flags & 0x800)) {
            return;
        }
    }
    if (func_8008C5D8(o, w, arg3) < 0) {
        return;
    }
    switch (w) {
    case 5: case 6: case 8: case 10: case 11: case 13: case 15: case 16: case 17:
        break;
    default:
        if (o->w260 != -1) {
            o->w260 = D_8021945C;
        }
        break;
    }
    o->last[slot] = D_8021945C;
    switch (slot) {
    case 0:
        if (D_802194A0 != 6 && D_80117ECC == 0) {
            o->ammo[w]--;
        }
        switch (o->kind) {
        case 1:
            k = -7;
            w = 2;
            break;
        case 5:
            k = -3;
            w = 2;
            break;
        case 0: case 2: case 3: case 4: case 6: case 7: case 10: case 12: case 13: case 14:
            k = -10;
            w = 5;
            break;
        default:
            k = 0;
            w = 0;
            break;
        }
        if (k != 0) {
            func_800B5DA8(o->vel, k * o->u40, o->ang20 + o->ang24);
            if (o->flags & 2) {
                func_80098B58(o->ctl->b0B, w, 10, 0);
            }
        }
        break;
    case 1:
        if (!(o->flags & 0x800) || D_802195D4 % 3 == 0) {
            o->ammo[w]--;
        }
        if (o->ammo[w] == 0) {
            o->flags &= ~0x800;
            i = w;
            do {
                i = o->ctl->next[i];
            } while ((o->ammo[i] == 0) & (i != w));
            if (i == w) {
                o->cur = 0;
                if (o->flags & 2) {
                    func_800C924C(0, o->ctl->b0B, 0);
                }
            } else {
                o->cur = i;
                if (o->flags & 2) {
                    func_800C924C(&o->ammo[i], o->ctl->b0B, i);
                }
            }
        }
        break;
    }
}

void func_8008EB4C(Obj *o) {
    int i;
    Ctl *c;

    if (!(o->flags & 0x800) && (o->w290 == 0 || o->w294 == 0 || o->w298 == 0)) {
        i = o->cur;
        if (i != 0) {
            if (D_8021945C - o->time > 120) {
                func_800A9A44(o->ctl, i);
            }
            c = o->ctl;
            do {
                i = c->next[i];
            } while (o->ammo[i] == 0 && i != o->cur);
            o->cur = i;
            if (o->flags & 2) {
                func_800C924C(&o->ammo[i], o->ctl->b0B, i);
            }
            o->time = D_8021945C;
        }
    }
}

inline void func_8008EC40(Obj *o, int slot, int n) {
    o->ammo[slot] += n;
    if (slot != 1 && o->cur == 0) {
        o->cur = slot;
        if (o->flags & 2) {
            func_800C924C(&o->ammo[slot], o->ctl->b0B, slot);
        }
    }
}

void func_8008ECB4(Obj *o, int type, unsigned short show) {
    char *msg = 0;
    float f;

    switch (type) {
    case 1:
        func_8008EC40(o, 2, 5);
        msg = "PICKED UP SWARMERS";
        break;
    case 2:
        func_8008EC40(o, 3, 5);
        msg = "PICKED UP LASERS";
        break;
    case 3:
        func_8008EC40(o, 4, 5);
        msg = "PICKED UP GUIDED MISSILES";
        break;
    case 5:
        if (show || func_8009D914() % 10 == 0) {
            func_8008EC40(o, 7, 1);
        }
        msg = "PICKED UP NUKE";
        break;
    case 6:
        func_8008EC40(o, 6, 20);
        msg = "PICKED UP MINES";
        break;
    case 12:
        func_8008EC40(o, 10, 5);
        msg = "PICKED UP TURBO";
        break;
    case 4:
        if (show) {
            func_8008EC40(o, 16, 2);
        } else {
            func_8008EC40(o, 16, 1);
        }
        msg = "PICKED UP GUN BUDDIES";
        break;
    case 14:
        func_8008EC40(o, 15, 20);
        msg = "PICKED UP BOUNCING BETTIES";
        break;
    case 15:
        func_8008EC40(o, 12, 100);
        msg = "PICKED UP FLAMETHROWER";
        break;
    case 16:
        func_8008EC40(o, 13, 2);
        msg = "PICKED UP CLOAKING";
        break;
    case 17:
        func_8008EC40(o, 14, 5);
        msg = "PICKED UP GRENADES";
        break;
    case 18:
        o->ctl->bank->c += 5;
        o->ctl->bank->e += 5;
        msg = "PICKED UP TANK BUCKS";
        break;
    case 19:
        if ((o->flags & 2) || o->w9C == 0) {
            if (o->ctl->b0B < 2) {
                D_8011F218[o->ctl->b0B] = 1;
            }
            o->flags |= 0x200;
        }
        msg = "PICKED UP RADAR";
        break;
    case 7:
        msg = "PICKED UP HEALTH";
        if (o->hp == o->maxhp) {
            func_8008EC40(o, 11, 1);
        } else {
            switch (o->kind) {
            case 2:
                f = 2.0f;
                break;
            case 6:
                f = 0.66f;
                break;
            default:
                f = 1.0f;
                break;
            }
            o->hp += (int)(f * 30.0f);
            func_80095B90(o, 0x3B);
            if (o->hp > o->maxhp) {
                o->hp = o->maxhp;
            }
        }
        break;
    case 9:
        msg = "PICKED UP TELEPORTERS";
        func_8008EC40(o, 8, 5);
        break;
    case 10:
        msg = "PICKED UP PLASMA BOLTS";
        func_8008EC40(o, 9, 5);
        break;
    case 13:
        msg = "PICKED UP SHIELDS";
        o->shield = 100;
        break;
    case 11:
        msg = "PICKED UP AMMO";
        func_8008EC40(o, 1, D_80122E48[o->kind].ammo / 3);
        break;
    case 8:
        msg = "PICKED UP STAR";
        func_8008EC40(o, 1, D_80122E48[o->kind].ammo / 6);
        switch (o->kind) {
        case 2:
            f = 2.0f;
            break;
        case 6:
            f = 0.66f;
            break;
        default:
            f = 1.0f;
            break;
        }
        o->hp += (int)(f * 15.0f);
        func_80095B90(o, 0x3B);
        if (o->hp > o->maxhp) {
            o->hp = o->maxhp;
        }
        break;
    }
    if ((show != 0) & (msg != 0) && D_80117EB4 != 10) {
        func_800CA620(o->b95, msg, 45);
    }
}
