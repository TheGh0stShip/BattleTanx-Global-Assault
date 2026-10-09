/* SPAN 0x80087798 */
/* RODATA_VRAM 0x800715E8 */
typedef unsigned short u16;
typedef struct { float x, y; } Pt;
typedef struct { char p0[8]; float pos[2]; char p10[0x10]; u16 id; char p22[0x150 - 0x22]; Pt last; char p158[0x1C4 - 0x158]; float speed; } Ent;
typedef struct { float dist; float speed; u16 heading; char pa[6]; int arrived; } Move;
Pt *func_80082834(Ent *);
int func_80082870(Ent *, Pt *);
void func_8009DA44(Pt *, float *, Pt *);
u16 func_8009DFAC(Pt *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

void func_80087570(Ent *e, Move *m) {
    Pt *p = func_80082834(e);
    Pt d;

    e->last.x = e->pos[0];
    e->last.y = e->pos[1];
    if (func_80082870(e, p) != 0) {
        m->dist = 0.0f;
        m->heading = e->id;
        m->speed = 0.0f;
        m->arrived = 1;
        return;
    }
    func_8009DA44(&d, e->pos, p);
    m->dist = DIST(e->pos[0] - p->x, e->pos[1] - p->y);
    m->speed = e->speed;
    m->heading = func_8009DFAC(&d);
    m->arrived = 0;
}
