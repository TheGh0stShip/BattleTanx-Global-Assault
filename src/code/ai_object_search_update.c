/* RODATA_VRAM 0x800712F0 */
typedef unsigned char u8;
typedef struct { char p0[0x3D]; u8 kind; } Def;
typedef struct { char p0[0xC]; int alive; float x; float y; char p18[8]; Def *def; char p24[0x1D - 0x24 + 8]; } Dummy;
typedef struct Obj { char p0[0xC]; int alive; float x; float y; char p18[5]; u8 wp; char p1e[2]; Def *def; } Obj;
typedef struct { char p0[0x10]; int team; char p14[0x250 - 0x14]; } Way;
typedef struct { unsigned int state; Obj *obj; char p8[4]; int timer; } Ai;
typedef struct { void *obj; char p4[0x24]; } Hit;
typedef struct {
    int b0; char p4[4]; float x; float y; char p10[0x94 - 0x10]; u8 color; u8 wp; char p96[0xA0 - 0x96]; int busy;
    char pa4[0x170 - 0xA4]; Ai ai; char p180[0x1BC - 0x180]; float range; char p1c0[0x1D0 - 0x1C0];
    struct { char p[0x10]; int team; } *owner;
} Tank;
extern Way D_80235F00[];
extern int D_8021945C;
extern short D_80397650;
Obj *func_800A1A28(Obj *, int);
void func_80088B6C(Tank *, float *, int, int);
unsigned short func_800B49E0(float *, float *, int, int, int, int, Hit *);
void func_80089040(Tank *, Obj *);
void func_80086208(Tank *, int);
int func_80083FF8(Tank *, Obj *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

static inline Way *wp_at(int i) { return &D_80235F00[i]; }
static inline Way *waypoint(int i) {
    if (i == 127) return 0;
    return wp_at(i);
}

static inline int alive(Obj *o) {
    int result = 0;
    if (o != 0) result = o->alive != 0;
    return result;
}

static inline int is7(Obj *o) { return o->def->kind == 7; }

void func_80084278(Tank *e) {
    Ai *ai = &e->ai;
    Obj *o;
    Hit hit;

    switch (e->ai.state) {
    case 1:
        for (o = func_800A1A28(0, 20); o != 0; o = func_800A1A28(o, 20)) {
            if (waypoint(o->wp)->team != e->owner->team && o->alive != 0) break;
        }
        ai->obj = o;
        if (e->busy != 0) {
            ai->state = 4;
        } else if (o == 0) {
            func_80086208(e, 6);
        } else {
            func_80088B6C(e, &o->x, 0, 1);
            ai->state = 2;
        }
        break;
    case 2:
        if (e->busy != 0) {
            ai->state = 4;
        } else if (!alive(ai->obj)) {
            ai->state = 1;
        } else if (ai->timer <= D_8021945C) {
            ai->timer = D_8021945C + 30;
            o = ai->obj;
            if (is7(o)) break;
            {
                int k = e->color;
                int b0 = e->b0;
                D_80397650 = 0;
                if (func_800B49E0(&e->x, &o->x, 0x241009, k, 0, b0, &hit) != 0) break;
            }
            if (DIST(e->x - ai->obj->x, e->y - ai->obj->y) <= e->range) {
                func_80089040(e, ai->obj);
                ai->state = 3;
            }
        }
        break;
    case 3:
        if (e->busy == 0) {
            if (!alive(ai->obj)) ai->state = 1;
            break;
        }
        ai->state = 4;
        break;
    case 4:
        o = ai->obj;
        if (alive(o) && func_80083FF8(e, o)) break;
        for (o = func_800A1A28(0, 20); o != 0; o = func_800A1A28(o, 20)) {
            if (o->wp == e->wp) break;
        }
        if (o == 0) {
            func_80086208(e, 6);
        } else {
            func_80088B6C(e, &o->x, 0, 1);
            ai->state = 5;
        }
        break;
    case 5:
        if (e->busy == 0) ai->state = 6;
        break;
    case 6:
        func_80086208(e, 6);
        break;
    }
}
