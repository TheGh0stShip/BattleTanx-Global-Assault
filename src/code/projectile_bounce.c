typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[0xC]; Vec3 pos; float vx, vy, vz; float f24; struct { float x, y; } *pts; int t;
    unsigned short angle; unsigned char n; unsigned char idx;
    unsigned char b52, b53, b54; char pad37; int i56; int i60;
} S;
typedef struct { int pad0; int type; } Obj;
typedef struct { Obj *obj; int pad4; Vec3 pos; float nx, ny, nz; unsigned short kind; } Hit;
typedef struct { Vec3 pos; short angle; int b52; int i60; int b53; int pad; int zero; } Ev;
typedef struct { unsigned int result; int a, b, c, d, e, f; } Info;
typedef struct { void (*fn)(Obj *, S *, int, Ev *, Info *); int pad[2]; } Handler;
extern int D_8021945C; extern float D_80219488; extern short D_80397650;
extern Handler D_80224B5C[]; extern char D_801157F8[];
extern float func_8009D8A0(float); extern void func_800A1AFC(void *);
extern unsigned short func_800B49E0(Vec3 *, Vec3 *, int, int, int, int, Hit *);
extern int func_8009D914(void);
extern void func_80097FB4(int, float, float, float, int);
extern int func_8009DDF0(float *);
extern float func_8009D4B0(int); extern float func_8009D510(int);
extern void func_800A5BD8(Ev *, int, int, float, void *, int);
extern int func_800B02D4(float, float, int);

static inline float rnd(float r) { return func_8009D8A0(r) - r / 2.0f; }

void func_800DAC34(S *arg, int *done) {
    S *s = arg;
    Vec3 pos; Hit hit; Vec3 a; Vec3 b; Ev ev;
    float d; unsigned short r; int kill = 0; int bounce = 0; int keep = 1;

    if (D_8021945C - s->t > 240) {
        *done = 1;
        func_800A1AFC(s->pts);
        return;
    }
    s->pts[s->idx].x = s->pos.x;
    s->pts[s->idx].y = s->pos.y;
    s->idx = (s->idx + 1) % 8;
    if (s->n < 8) s->n++;
    pos.x = s->pos.x + D_80219488 * (rnd(s->f24 * 0.8f) + s->vx);
    pos.y = s->pos.y + D_80219488 * (rnd(s->f24 * 0.8f) + s->vy);
    pos.z = s->pos.z;
    a = s->pos;
    b = pos;
    if (pos.z > 20.0f) {
        a.z = 37.0f; b.z = 37.0f;
    }
    {
        int team = s->b54; int ign = s->i56;
        D_80397650 = 1;
        r = func_800B49E0(&a, &b, 0x64940B, team, 0, ign, &hit);
    }
    if (r) {
        hit.pos.z = s->pos.z;
        if (hit.obj) {
            Info info = { 2, 0, 0, 0, 0, 0, 0 };
            ev.pos = hit.pos;
            ev.angle = s->angle;
            ev.b52 = s->b52;
            ev.i60 = s->i60;
            ev.b53 = s->b53;
            ev.zero = 0;
            if (D_80224B5C[hit.obj->type].fn)
                D_80224B5C[hit.obj->type].fn(hit.obj, s, 0, &ev, &info);
            switch (info.result) {
            case 1: kill = 1; break;
            case 3: kill = 1; keep = 0; break;
            case 5: bounce = 1; break;
            }
        } else {
            bounce = 1;
        }
    }
    if (bounce) {
        switch ((unsigned)func_8009D914() & 3) {
        case 0: func_80097FB4(25, s->pos.x, s->pos.y, 1.0f, s->b54); break;
        case 1: func_80097FB4(26, s->pos.x, s->pos.y, 1.0f, s->b54); break;
        case 2: func_80097FB4(27, s->pos.x, s->pos.y, 1.0f, s->b54); break;
        case 3: func_80097FB4(28, s->pos.x, s->pos.y, 1.0f, s->b54); break;
        }
        if (hit.kind != 7) {
            d = s->vx * hit.nx + s->vz * hit.nz + s->vy * hit.ny;
            s->vx = s->vx - (hit.nx + hit.nx) * d;
            s->vz = s->vz;
            s->vy = s->vy - (hit.ny + hit.ny) * d;
            s->angle = func_8009DDF0(&s->vx);
            s->pos.x = hit.pos.x + func_8009D4B0(s->angle) / 100.0f;
            s->pos.y = hit.pos.y + func_8009D510(s->angle) / 100.0f;
            s->i56 = 0;
            return;
        }
    } else if (kill) {
        func_80097FB4(57, s->pos.x, s->pos.y, 1.0f, s->b54);
        *done = 1;
        if (keep) {
            ev.pos.z = hit.pos.z;
            ev.pos.x = hit.pos.x + (s->pos.x - hit.pos.x) * 0.1f;
            ev.pos.y = hit.pos.y + (s->pos.y - hit.pos.y) * 0.1f;
            func_800A5BD8(&ev, 0, s->b54, 1.0f, D_801157F8, 0);
        }
        return;
    } else {
        s->pos.x = pos.x;
        s->pos.y = pos.y;
        if (func_800B02D4(pos.x, pos.y, s->b54)) return;
    }
    *done = 1;
}
