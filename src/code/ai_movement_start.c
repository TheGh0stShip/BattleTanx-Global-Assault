/* SPAN 0x8008380C */
/* RODATA_VRAM 0x800712A8 */
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)
typedef struct { int *surf; char p4[4]; float pos[2]; char p10[0x20 - 0x10]; unsigned short type; char p22[6]; } Hit;
typedef struct { int state; int t174; short h178; char p17A[2]; float cx; float cy; float r; float sx; float sy; float sr; } Mov;
typedef struct {
    int id; char p4[4]; float x; float y; char p10[0x20 - 0x10]; unsigned short ang; char p22[0x94 - 0x22]; unsigned char team;
    char p95[0x170 - 0x95]; Mov mov; char p194[0x1E0 - 0x194]; int flags;
} Obj;
extern int D_8021945C;
extern short D_80397650;
extern void func_8009DAB0(float *, float, unsigned short);
extern void func_8009E044(float *, float *);
extern unsigned short func_800B49E0(float *, float *, int, unsigned char, int, int, Hit *);
extern void func_80088720(Obj *);
extern void func_80089008(Obj *);

void func_8008355C(Obj *o) {
    Hit hit;
    float dir[2];
    float tgt[2];
    float *p;
    float d;
    float big;
    Mov *m;

    o->mov.state = 2;
    o->mov.h178 = 0;
    o->flags |= 0x100;
    o->mov.t174 = D_8021945C;
    func_8009DAB0(dir, 2000.0f, o->ang);
    tgt[0] = o->x;
    tgt[1] = o->y;
    func_8009E044(tgt, dir);
    {
        int team = o->team;
        int id = o->id;
        m = &o->mov;
        D_80397650 = 0;
        p = func_800B49E0(&o->x, tgt, 0x2C7007, team, 300, id, &hit) ? hit.pos : tgt;
    }
    m->cx = (o->x + p[0]) / 2.0f;
    m->cy = (o->y + p[1]) / 2.0f;
    m->r = DIST(m->cx - p[0], m->cy - p[1]);
    m->sx = m->cx;
    m->sy = m->cy;
    m->sr = m->r;
    func_80088720(o);
    func_80089008(o);
}
