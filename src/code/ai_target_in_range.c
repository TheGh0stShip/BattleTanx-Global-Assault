/* SPAN 0x800841D4 */
/* RODATA_VRAM 0x800712D0 */
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)
typedef struct { char pad[0x3D]; unsigned char kind; } Def;
typedef struct { char pad[0x10]; float x; float y; char p18[8]; Def *def; } Tgt;
typedef struct { char pad[8]; float x; float y; } Obj;

static inline int is7(Tgt *t) { return t->def->kind == 7; }

int func_80083FF8(Obj *o, Tgt *t) {
    int r = 0;

    if (is7(t)) {
        if (DIST(o->x - t->x, o->y - t->y) <= 300.0f) r = 1;
    }
    return r;
}
