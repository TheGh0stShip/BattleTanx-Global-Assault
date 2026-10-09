/* SPAN 0x80084C50 */
/* RODATA_VRAM 0x80071320 */
typedef struct { float x, y; } Pt;
typedef struct { int state; Pt home; Pt target; int near; } Ai;
typedef struct { char p0[8]; float x; float y; char p10[0x90 - 0x10]; Pt *leader; char p94[0x170 - 0x94]; Ai ai; } Tank;
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

int func_800848E8(Tank *e) {
    Pt *l = e->leader;
    Ai *ai;
    int r = 0;
    float d1, d2, diff;
    int c;

    if (l != 0) {
        ai = &e->ai;
        d1 = DIST(l->x - e->x, l->y - e->y);
        d2 = DIST(l->x - ai->target.x, l->y - ai->target.y);
        c = d1 < d2;
        if (c != ai->near) {
            ai->near = c;
            r = 1;
        } else {
            diff = c ? d2 - d1 : d1 - d2;
            if (diff <= 20.0f) r = 1;
        }
    }
    return r;
}
