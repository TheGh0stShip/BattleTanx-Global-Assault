/* SPAN 0x8008865C */
/* RODATA_VRAM 0x80071674 */
typedef unsigned char u8;
typedef struct { float x, y; } Pt;
typedef struct { void *obj; char p4[0x24]; } Hit;
typedef struct { char p0[4]; int type; char p8[4]; Pt pos; char p14[8]; short hp; } Thing;
typedef struct { float dist; char p4[4]; float best; int timer; u8 seen; } Track;
typedef struct Tank {
    int b0; char p4[4]; float x; float y; char p10[4]; float lx; float ly; char p1c[0x94 - 0x1C]; u8 color;
    char p95[0xA4 - 0x95]; int bit; int seen; char pac[0xB4 - 0xAC]; float tdist; char pb8[4]; float tbest; int ttimer; u8 tseen; u8 mode;
    char pc6[2]; void *tgt; char pcc[0xE8 - 0xCC]; u8 valid; u8 kind; char pea[0x150 - 0xEA]; Pt eye;
    char p158[0x16C - 0x158]; u8 flags; char p16d[0x1D0 - 0x16D]; struct { char p[0x10]; int team; } *owner;
    char p1d4[0x1E0 - 0x1D4]; int state;
} Tank;
extern int D_8021945C;
extern float D_80219488;
extern short D_80397650;
int func_80095B68(Tank *);
void func_80088720(Tank *);
unsigned short func_800B49E0(float *, Pt *, int, int, int, int, Hit *);
void func_80086190(Tank *, Pt *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

void func_80088030(Tank *e) {
    Pt *pos = 0;
    int visible = 0;
    int lost = 0;
    int mask = 0x2C7007;
    Track *tr = (Track *)&e->tdist;
    Hit hit;
    float d;
    float d2;

    switch (e->mode) {
    case 1: {
        Tank *t = e->tgt;
        if (!func_80095B68(t) || (t->state & 0x80)) {
            func_80088720(e);
            lost = 1;
            break;
        }
        pos = &t->eye;
        if (e->seen & t->bit) {
            visible = 1;
            break;
        }
        if (e->owner->team != t->owner->team) lost = 1;
        break;
    }
    case 0:
        lost = 1;
        break;
    case 3: {
        Thing *th = e->tgt;
        if (th == 0 || th->type != 28 || th->hp <= 0) {
            func_80088720(e);
        } else {
            pos = &th->pos;
            mask = 0x287007;
        }
        break;
    }
    case 2:
        pos = (Pt *)&e->tgt;
        if (pos != 0 && e->kind == 5) visible = e->valid != 0;
        lost = !visible;
        break;
    }
    if (pos == 0) {
        tr->best = 64000.0f;
        e->flags &= ~3;
        return;
    }
    d = DIST(pos->x - e->x, pos->y - e->y);
    if (lost && tr->seen) {
        e->flags &= ~3;
    } else {
        if (!visible && !lost) {
            int k = e->color;
            int b0 = e->b0;
            D_80397650 = 0;
            lost = func_800B49E0(&e->x, pos, mask, k, 0, b0, &hit);
            visible = !lost;
        }
        if (lost && tr->seen) {
            e->flags &= ~3;
        } else {
            if (d < tr->dist) e->flags |= 1; else e->flags &= ~1;
            if (visible) e->flags |= 2; else e->flags &= ~2;
        }
    }
    if (e->flags & 2) {
        tr->best = 64000.0f;
        tr->timer = D_8021945C + 300;
        return;
    }
    d2 = DIST(pos->x - e->lx, pos->y - e->ly);
    if (d2 < d) {
        tr->timer += (int)D_80219488;
    } else if (d < tr->best) {
        tr->best = d;
        tr->timer = D_8021945C + 300;
    } else if (D_8021945C >= tr->timer) {
        tr->timer = D_8021945C + 300;
        func_80086190(e, pos);
    }
}
