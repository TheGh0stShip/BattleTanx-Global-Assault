/* SPAN 0x80085C30 */
/* RODATA_VRAM 0x80071368 */
typedef unsigned char u8;
typedef struct { int state; int next; float range; float x; float y; } Goal;
typedef struct { char p0[8]; float x; float y; char p10[0x16C - 0x10]; u8 flags; char p16d[3]; Goal g; } Tank;
int func_800857F0(Tank *);
void func_80086208(Tank *, int);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

void func_800859E4(Tank *e) {
    Goal *g = &e->g;

    switch (e->g.state) {
    case 1:
        if (func_800857F0(e) != 0) e->g.state = 2;
        break;
    case 2:
        if (!(e->flags & 1)) {
            if (!(DIST(e->x - g->x, e->y - g->y) < g->range)) break;
        }
        func_80086208(e, g->next);
        break;
    }
}
