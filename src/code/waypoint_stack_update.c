/* SPAN 0x8008AD7C */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { int serial; int type; char p8[8]; int team; char p14[0x250 - 0x14]; } Obj;
typedef struct { char p0[0x1B]; u8 wp; } Item;
typedef struct { Obj *node; int serial; } Ent;
typedef struct { u8 active; u8 done; u16 depth; Ent e[5]; } Stack;
typedef struct { char p0[0xC00C]; Stack a; Stack b; int cur; int prev; u8 flag; u8 pad; u16 count; int team; } World;
extern World *D_80114680;
extern struct { int mode; } D_802194A0;
extern int D_8021945C;
extern Obj D_80235F00[];
Item *func_800A1A28(Item *, int);
void func_80089684(void);
Ent *func_8008A62C(Stack *);
void func_80087C38(Obj *);
void func_80086A20(Obj *);
void func_80087C30(Obj *);

static inline Obj *wp_at(int i) { return &D_80235F00[i]; }
static inline Obj *waypoint(int i) {
    if (i == 127) return 0;
    return wp_at(i);
}

inline void func_8008AA20(void) {
    Item *it;

    if (D_80114680->flag != 0) return;
    if (D_8021945C < 100) return;
    if ((D_802194A0.mode == 4) | (D_802194A0.mode == 10)) {
        it = func_800A1A28(0, 28);
        if (it != 0) {
            D_80114680->team = waypoint(it->wp)->team;
            while (it != 0) {
                D_80114680->count++;
                it = func_800A1A28(it, 28);
            }
        }
    }
    D_80114680->flag = 1;
}

void func_8008AB48(int unused, int *done) {
    Ent *e;
    int finished;

    if (D_80114680 == 0) {
        *done = 1;
        return;
    }
    func_80089684();
    e = func_8008A62C(&D_80114680->a);
    switch (e->node->type) {
    case 5:
        func_8008AA20();
        break;
    case 0x43:
        func_80086A20(e->node);
        break;
    case 4:
        func_80087C38(e->node);
        break;
    }
    finished = 0;
    do {
        e = func_8008A62C(&D_80114680->b);
        switch (e->node->type) {
        case 5:
            finished = 1;
            break;
        case 4:
            func_80087C30(e->node);
            break;
        case 0x43:
            break;
        }
    } while (!finished);
}
