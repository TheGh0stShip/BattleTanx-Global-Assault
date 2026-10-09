/* SPAN 0x8007E64C */
/* RODATA_VRAM 0x800710E0 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { float x, y; } Pt;
typedef struct { void *head; u16 p4; u16 count; char p8[0x20 - 8]; Pt a; Pt b; } Path;
typedef struct { char p0[0xF0]; Path path; char p120[0x16C - 0x120]; u8 flags; } Ent;
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

int func_8007E2D4(Ent *e, Pt *g) {
    Path *p = &e->path;
    int r = 0;

    if (g != 0 && !(e->flags & 1)) {
        if (p->p4 == 0) {
            r = 1;
        } else {
            float d = DIST(p->a.x - g->x, p->a.y - g->y);
            float d2 = DIST(p->b.x - g->x, p->b.y - g->y);
            if (d < d2 * 4.0f) {
                r = 1;
            } else if (!(e->flags & 4) && p->head == 0 && p->count < 3) {
                r = 1;
            }
        }
    }
    return r;
}
