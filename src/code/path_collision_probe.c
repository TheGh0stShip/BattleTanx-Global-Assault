/* SPAN 0x8007ED68 */
/* RODATA_VRAM 0x80071104 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { float x, y; } Pt;
typedef struct { int id; } Obj;
typedef struct { Obj *obj; char p4[4]; float x; float y; char p10[0x10]; u16 id; char p22[6]; } Hit;
typedef struct Node { u16 p0; u16 id; float x; float y; char pc[8]; u8 kind; } Node;
typedef struct { Node *head; u16 p4; u16 count; char p8[2]; u16 cur; char pc[8]; u16 a; u16 b; int c; int f1c; Pt start; Pt goal; int tgt; float range; } Path;
typedef struct { int b0; char p4[0x94 - 4]; u8 color; char p95[0xF0 - 0x95]; Path path; char p128[0x132 - 0x128]; u16 gcur; } Ent;
extern short D_80397650;
void func_8007DBE0(u16 *);
u16 func_8007D69C(Ent *, int);
unsigned short func_800B49E0(Pt *, Pt *, int, int, int, int, Hit *);
u16 func_8007D884(Node **, int);
void func_8009E068(float *, Pt *, float);
void func_8007D558(char *, Hit *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

void func_8007E9D0(Ent *e) {
    Hit hit;
    Node *n;
    Path *p;

    e->gcur = 0;
    func_8007DBE0(&e->path.p4);
    e->path.count = 0;
    e->path.head = 0;
    e->path.cur = 0;
    e->path.a = 0;
    e->path.b = 0;
    e->path.c = 0;
    {
        int k = e->color;
        u16 t = func_8007D69C(e, 0);
        int b0 = e->b0;
        D_80397650 = 0;
        p = &e->path;
        if (func_800B49E0(&e->path.start, &e->path.goal, 0x87007, k, t, b0, &hit) != 0
            && !(e->path.tgt != 0 && hit.obj != 0 && e->path.tgt == hit.obj->id)) {
            if (hit.id == 7) {
                p->p4 = 0;
                p->f1c = 2;
                return;
            }
            p->p4 = func_8007D884(&n, 3);
            p->count++;
            n->x = hit.x;
            n->y = hit.y;
            func_8009E068(&n->x, &p->start, p->range + 20.0f - func_8007D69C(e, 0));
            n->kind = 0;
            n->id = 0;
            func_8007D558((char *)n + 12, &hit);
            p->cur = p->p4;
            return;
        }
    }
    if (p->range < DIST(p->start.x - p->goal.x, p->start.y - p->goal.y)) {
        p->p4 = func_8007D884(&n, 3);
        p->count++;
        n->x = p->goal.x;
        n->y = p->goal.y;
        n->kind = 3;
        n->id = 0;
        p->cur = p->p4;
        return;
    }
    p->p4 = 0;
    n = 0;
}
