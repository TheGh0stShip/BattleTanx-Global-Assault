/* ---- 0x800F2000/b/f6144.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char p[0x74]; int s74; char p2[484 - 0x78]; unsigned char c[3]; char p3[0x250 - 487]; } P;
typedef struct {
    char pad0[10];
    unsigned short u10;
    Vec3 pos;
    unsigned short u24;
    unsigned char b26;
    unsigned char b27;
    short timer;
    char pad30[6];
    int h36;
} Obj61;
typedef struct { float x, y; int r; int idx; int pad; int h20; } Q61;
typedef struct { short pad0; short rad; char pad[36]; } T61;
extern T61 D_80397900[];
extern P D_80235F00[];
extern char D_80115BC8[];
extern int func_800EC72C(int, int);
extern void func_800DA7D0(int, Vec3 *, int, int, int, int, int, int, int);
extern void func_800A5BD8(Vec3 *, int, int, float, void *, int);
extern void func_800A9A98(P *, int);
extern void func_800A9B64(P *, int);
static inline P *getp(int i) {
    P *r;
    if (i == 127) r = 0; else r = &D_80235F00[i];
    return r;
}
void func_800F6144(Obj61 *o, int a1, Q61 *q, unsigned char *hit) {
    P *p;
    P *p2;
    int dt;
    int dx, dy;
    dx = o->pos.x - q->x;
    dx = dx * dx;
    dy = o->pos.y - q->y;
    dy = dy * dy;
    if (dx + dy <= q->r + D_80397900[o->u10].rad) {
        dt = func_800EC72C(q->h20, q->r);
        p = getp(q->idx);
        if (o->timer > 0) {
            o->timer -= dt;
            if (o->timer <= 0) {
                p2 = getp(o->b27);
                func_800DA7D0(o->h36, &o->pos, o->u24, o->b26, 0, 0, p2->c[0], p2->c[1], p2->c[2]);
                func_800A5BD8(&o->pos, o->u24, o->b26, 1.0f, D_80115BC8, 0);
                if (p != 0) {
                    func_800A9A98(p, 1000);
                }
            }
        }
        func_800A9B64(getp(o->b27), 10);
        *hit = 1;
    }
}

