typedef struct {
    char pad0[10];
    unsigned short id;
    float x, y;
    char pad20[4];
    unsigned short u24;
    unsigned char b26, b27;
    short h28;
    char pad30[26];
    float speed, maxspeed;
    int timer;
} Obj;
typedef struct { char pad[16]; short h16; char pad18[22]; } Ent;
typedef struct { char pad[16]; int team; char pad20[96]; int state; char pad120[472]; } Pl;
extern short D_80397650;
extern Ent D_803978F4[];
extern unsigned char D_803AD950[];
extern struct { char pad[2]; unsigned char n; } D_802194A4;
extern Pl D_80235F00[];
extern int D_80117EB4;
extern int D_8021945C;
extern float D_80219488;
extern unsigned short func_800B3748(int, int, void *, int, int);
extern void func_800F5E18(Obj *, int, int);
extern void func_800B22F8(int);
extern void func_800B129C(int, short, short);
extern int func_800F5548(Obj *, float);
extern float func_8009D4B0(int);
extern float func_8009D510(int);
extern void func_800F5470(Obj *);
extern void func_800C8238(short, short, int, int, int, int);
static __inline__ Pl *getpl(int i) { if (i == 127) return 0; return &D_80235F00[i]; }
void func_800F577C(Obj *o_, int *done_) {
    char buf[1152];
    Obj *o = o_;
    int *done = done_;
    int i;
    D_80397650 = 0;
    if (func_800B3748(o->id, 0x200000, buf, 0, 1)) {
        func_800F5E18(o, 10000, 0);
    }
    if (o->h28 <= 0) {
        func_800B22F8(o->id);
        *done = 1;
        D_803AD950[o->b27]--;
        if (D_803AD950[o->b27] == 0) {
            for (i = 0; i < D_802194A4.n; i++) {
                if (getpl(i)->team == getpl(o->b27)->team) {
                    getpl(i)->state = 2;
                }
            }
        }
        return;
    }
    if (D_80117EB4 == 10) {
        int id = o->id;
        short x = o->x;
        short y = o->y;
        D_803978F4[id].h16 = o->u24;
        func_800B129C(id, x, y);
        return;
    }
    if (o->speed == 0.0f) {
        if (D_8021945C - o->timer >= 91) {
            if (func_800F5548(o, 50.0f)) {
                o->speed = 0.0001f;
            } else {
                o->timer = D_8021945C;
            }
        }
    } else {
        o->speed += 0.1f;
        if (o->maxspeed < o->speed) o->speed = o->maxspeed;
        if (!func_800F5548(o, o->speed * D_80219488 + 50.0f)) {
            if ((o->speed -= 0.4f) <= 0.0f || !func_800F5548(o, o->speed * D_80219488)) {
                o->speed = 0.0f;
                o->timer = D_8021945C;
            }
        }
        if (0.0f < o->speed) {
            o->x += func_8009D4B0(o->u24) * o->speed * D_80219488;
            o->y += func_8009D510(o->u24) * o->speed * D_80219488;
            func_800B129C(o->id, o->x, o->y);
        }
    }
    func_800F5470(o);
    func_800C8238(o->x, o->y, o->u24, o->b26, o->b27, 0);
}
