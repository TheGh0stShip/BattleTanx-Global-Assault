/* RODATA_VRAM 0x80071238 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { float x, y; } Pt;
typedef struct { u16 fx; char p2[2]; int timer; u16 ang; char pa[2]; float range; Pt pt; } Aim;
typedef struct {
    char p0[8]; Pt pos; char p10[0x20 - 0x10]; u16 heading; char p22[0x30 - 0x22]; float speed; char p34[0x48 - 0x34];
    u16 turret; char p4a[0x94 - 0x4A]; u8 color; char p95[0x134 - 0x95]; Aim aim;
} Tank;
extern int D_8021945C;
u16 func_8009D6F8(int, int, int);
int func_8009D72C(int, int, int);
void func_800B109C(int, short, short, int, int, int, int, int, int, int, int, int);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

void func_80082198(Tank *e) {
    int done = 0;
    Aim *a = &e->aim;
    Aim *b;

    if (a->timer > D_8021945C) {
        u16 lo = func_8009D6F8(a->ang, 512, 2);
        u16 hi = func_8009D6F8(a->ang, 512, 1);
        if (func_8009D72C(lo, e->turret, hi)) {
            if (!(DIST(e->pos.x - a->pt.x, e->pos.y - a->pt.y) > a->range / 2.0f)) done = 1;
        }
    }
    if (done) return;
    b = &e->aim;
    if (e->speed <= 0.0f) {
        e->aim.ang = e->heading;
        e->aim.range = 0.0f;
    } else {
        e->aim.range = e->speed * 600.0f / 60.0f + 100.0f;
        e->aim.ang = e->turret;
    }
    b->pt.x = e->pos.x;
    b->pt.y = e->pos.y;
    if (a->fx != 0xFFFF) {
        if (e->speed <= 0.0f) {
            func_800B109C(a->fx, a->pt.x, a->pt.y, 0, 0, 0, 0, 0, 0, 0, a->ang, e->color);
        } else {
            func_800B109C(a->fx, a->pt.x, a->pt.y, 0, -150, 150, -100, (short)(a->range + 100.0f), 0, 0, a->ang, e->color);
        }
    }
    a->timer = D_8021945C + 10;
}
