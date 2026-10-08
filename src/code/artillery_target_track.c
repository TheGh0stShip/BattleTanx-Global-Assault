typedef struct { char pad[0x10]; float y; } Obj10;
typedef struct T {
    int *h; char p0[0xC]; float y10; char p1[0x98-0x14]; int idx; char p2[0x150-0x9C];
    float x, z; char p3[0x1D0-0x158]; struct { char p[0x10]; int id; } *team; char p4[0x1E0-0x1D4]; int flags;
} T;
typedef struct S {
    char p0[0xC]; float pos[3]; float vx, vz, vy; unsigned char kind; char p1[3];
    struct { char p[0x10]; int id; } *owner; int time; T *tgt; int tgtid;
} S;
typedef struct { float x, y, z; } V3;
typedef struct { char p[0xC]; T *obj; } HitE; typedef struct { HitE *e; char pad[36]; } Hit;
extern int D_8021945C; extern float D_80219488; extern short D_80397650;
extern short D_80122E46[];
extern float D_800774A0, D_800774A4, D_800774A8, D_800774AC, D_800774B0, D_800774B4, D_800774B8;
extern char D_801156F0[];
extern int func_8009D914(void); extern T *func_800E2810(float *, int, int, int, int);
extern void func_8009EC0C(float *); extern unsigned short func_800B49E0(float *, float *, int, int, int, int, Hit *);
extern void func_800966D4(T *, int); extern float func_8009D8A0(float);
extern void func_800A5BD8(float *, int, int, float, void *, int);
float sqrtf(float);

void func_800F872C(S *s, int *done)
{
    Hit hit; float np[3]; float d[3]; float len, m;
    T *t;
    if (D_8021945C - s->time > 1800) { *done = 1; return; }
    t = s->tgt;
    if (t != 0) {
        if (!(t->flags & 1)) s->tgt = 0;
        else if (*t->h != s->tgtid) s->tgt = 0;
    }
    if ((func_8009D914() & 0xF) == 0) {
        if ((s->tgt = func_800E2810(s->pos, s->kind, s->owner->id, 1000000000, 0x800)) == 0) goto slow;
        s->tgtid = *s->tgt->h;
    }
    if (s->tgt == 0) goto slow;
    d[0] = s->tgt->x - s->pos[0];
    d[2] = (s->tgt->y10 + D_80122E46[s->tgt->idx * 104]) - D_800774A0 - s->pos[2];
    d[1] = s->tgt->z - s->pos[1];
    func_8009EC0C(d);
    s->vx = s->vx * D_800774A4 + d[0];
    s->vy = s->vy * D_800774A4 + d[2];
    s->vz = s->vz * D_800774A4 + d[1];
    len = sqrtf(s->vx * s->vx + s->vy * s->vy + s->vz * s->vz);
    m = D_800774A8;
    if (len > m) {
        s->vx = s->vx / len * m;
        s->vy = s->vy / len * m;
        s->vz = s->vz / len * m;
    }
    goto move;
slow:
    s->vx *= D_800774AC;
    s->vy *= D_800774AC;
    s->vz *= D_800774AC;
move:
    np[0] = s->pos[0] + D_80219488 * s->vx;
    np[2] = s->pos[2] + D_80219488 * s->vy;
    np[1] = s->pos[1] + D_80219488 * s->vz;
    if ((func_800B49E0(s->pos, np, 8, s->kind, 0, (D_80397650 = 1, 0), &hit)) != 0) {
        T *o = hit.e->obj;
        if (o->team->id != s->owner->id) {
            func_800966D4(o, 150);
            s->tgt = 0;
            s->vx /= D_800774B0;
            s->vy /= D_800774B0;
            s->vz /= D_800774B0;
        }
    }
    { float r = D_800774B4;
    *(V3 *)s->pos = *(V3 *)np;
    if (func_8009D8A0(r) < D_800774B8)
        func_800A5BD8(s->pos, 0, s->kind, D_800774B8, D_801156F0, 0);
    }
}
