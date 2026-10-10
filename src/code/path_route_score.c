/* SPAN 0x80080818 */
/* RODATA_VRAM 0x800711A4 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct Node { u8 type; char p1; u16 link; float x; float y; } Node;
typedef struct { char p0[0x110]; float start[2]; float goal[2]; } Ent;
typedef struct { char p0[4]; float key; float dist; } Item;
Node *Steps_InitStep_Free(int);
float func_8007F774(Ent *, Item *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

static inline float *node_pos(Ent *e, Node *n) {
    for (;;) {
        if (n == 0) return e->start;
        if (n->type == 3) return &n->x;
        n = Steps_InitStep_Free(n->link);
    }
}

void func_80080110(Ent *e, Item *it) {
    it->key = func_8007F774(e, it);
    it->dist = DIST(node_pos(e, (Node *)it)[0] - e->goal[0], node_pos(e, (Node *)it)[1] - e->goal[1]);
}
