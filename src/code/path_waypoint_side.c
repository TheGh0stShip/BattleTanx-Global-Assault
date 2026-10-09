/* SPAN 0x8007F59C */
/* RODATA_VRAM 0x80071124 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { float x, y; } Pt;
typedef struct { char p0[0xC]; u16 right; u16 left; } Wp;
typedef struct { char p0[0xA]; u16 cur; char pc[0x16 - 0xC]; u16 prev; unsigned int side; char p1c[0x28 - 0x1C]; Pt goal; } Path;
typedef struct { char p0[0xF0]; Path path; } Ent;
Wp *func_8007DA5C(int);
void func_80080818(Ent *, int);
u16 func_800808B4(Ent *, int);
unsigned int func_8009D914(void);
void func_8007E118(Wp *, Pt *);
void func_8007D5B0(Pt *, float, Pt *, Pt *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

void func_8007F128(Ent *e) {
    u16 prev = e->path.prev;
    Wp *w = func_8007DA5C(prev);
    Path *p = &e->path;
    u16 id;

    switch (e->path.side) {
    case 0:
        break;
    case 2:
        if (w->right == 0) func_80080818(e, prev);
        break;
    case 1:
        if (w->left == 0) func_80080818(e, prev);
        break;
    }
    id = func_800808B4(e, 0);
    p->cur = id;
    p->prev = id;
    if (id == 0) {
        p->side = 0;
        return;
    }
    w = func_8007DA5C(id);
    if (w->left != 0) {
        p->side = 1;
    } else if (w->right != 0) {
        p->side = 2;
    } else {
        unsigned int r = func_8009D914();
        int c;
        if ((r & 3) == 3) {
            c = !((r >> 2) & 1);
        } else {
            Pt l, rt, pt;
            func_8007E118(w, &pt);
            func_8007D5B0(&pt, 0.0f, &l, &rt);
            c = DIST(p->goal.x - l.x, p->goal.y - l.y) < DIST(p->goal.x - rt.x, p->goal.y - rt.y);
        }
        if (c == 0) p->side = 1; else p->side = 2;
    }
}
