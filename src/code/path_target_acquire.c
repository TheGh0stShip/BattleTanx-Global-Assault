/* SPAN 0x8007E778 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef struct { int active; u16 p4; u16 cur; u16 prev; char pa[0x12]; int state; } Path;
typedef struct {
    char p0[8]; float x; float y; char p10[0xB8 - 0x10]; int bb8; char pbc[0xF0 - 0xBC];
    Path path; char p110[0x10C - 0x110 + 0x20];
} Dummy;
typedef struct {
    char p0[8]; float x; float y; char p10[0xB8 - 0x10]; int bb8; char pbc[0xF0 - 0xBC];
    Path path; float sx, sy, tx, ty; int s120; char p124[0x16C - 0x124]; u8 flags;
} Ent;
typedef struct { float x, y; } Pt;
Pt *func_8008865C();
int func_8007E2D4(Ent *, Pt *);
void func_8007E9D0(Ent *);
int func_8007F59C(Ent *);
void func_80080920(Ent *);

void func_8007E64C(Ent *e) {
    Path *p = &e->path;
    Pt *g;
    u32 i;

    if (e->path.state == 2) return;
    g = func_8008865C();
    if (g == 0) return;
    e->s120 = e->bb8;
    if (func_8007E2D4(e, g) == 0) return;
    { float a = e->x, b = e->y, c = g->x, d = g->y; e->sx = a; e->sy = b; e->tx = c; e->ty = d; }
    func_8007E9D0(e);
    i = 0;
    if (e->path.state == 2) {
        e->path.state = 0;
        e->flags |= 8;
        return;
    }
    while (func_8007F59C(e) && p->active == 0 && ++i < 100);
    if (p->p4 && p->state) p->state = 0;
    func_80080920(e);
    p->prev = p->cur;
    if (p->active == 0) e->flags |= 4; else e->flags &= ~4;
}
