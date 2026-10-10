typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;

typedef struct { float x, y, z; } Vec3;
typedef struct { s8 r, g, b; } Rgb;
typedef struct { char hdr[0xC]; } FxHdr;
typedef struct { float m[4][4]; } Mtx;

typedef struct {
    FxHdr hdr;
    Vec3 pos;
    Vec3 vel;
    Rgb col;
    s8 kind;
    int time;
    u8 count;
    u8 head;
    s16 (*trail)[3];
    float scale;
} FxTrail;

extern int D_8021945C;
extern char D_80114E84[];

void func_8009EEE0(Mtx *m);
void func_8009EF30(Mtx *m, float *v);
void func_800F4698(void *mesh, int n, Mtx *mtxs, u8 *ca, u8 *cb, int off, int mod, u8 flag);

static inline int trailSlot(int head, int back) {
    return head - back - 1;
}

void func_800A3560(FxTrail *o) {
    u8 ca[4];
    u8 cb[4];
    Mtx m[16];
    Vec3 p;
    int i, k, d;
    u8 alpha;

    if (!o->count) return;
    d = D_8021945C - o->time;
    if (d >= 15) {
        alpha = 255 - (d - 15) * 51;
    } else {
        alpha = 255;
    }
    func_8009EEE0(&m[o->count]);
    func_8009EF30(&m[o->count], &o->pos.x);
    for (i = 0; i < o->count; i++) {
        k = trailSlot(o->head, i - 7) % 7;
        p.x = o->trail[k][0];
        p.z = o->trail[k][1];
        p.y = o->trail[k][2];
        func_8009EEE0(&m[o->count - i - 1]);
        func_8009EF30(&m[o->count - i - 1], &p.x);
    }
    ca[0] = o->col.r;
    ca[1] = o->col.g;
    ca[2] = o->col.b;
    ca[3] = 0;
    cb[0] = o->col.r;
    cb[1] = o->col.g;
    cb[2] = o->col.b;
    cb[3] = alpha;
    func_800F4698(D_80114E84, o->count + 1, m, ca, cb, 0, 16, o->kind);
}
