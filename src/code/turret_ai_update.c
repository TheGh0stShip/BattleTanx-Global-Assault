/* func_800E3460 (0x800E3460-0x800E3FDC, 0xB7C) best source.
 * Status: NEAR_MISS under the clean lane gate (production normalizer, lane-gated rules OFF).
 * 4 words: spill-reload order of tgt/ang in d1 = tgt - ang (ROM loads tgt into t1 first).
 */
/* SPAN 0x800E3FDC */
/* RODATA_VRAM 0x80075C78 */
typedef struct { float x, y, z; } Vec3;
typedef struct { char p0[0x10]; int id; } Ctl10;
typedef struct Tgt {
    int *idp; char p4[0x94 - 4]; unsigned char team; char p95[0x150 - 0x95];
    float px, py; char p158[0x1D0 - 0x158]; Ctl10 *ctl; char p1D4[0x1E0 - 0x1D4]; int flags;
} Tgt;
typedef struct {
    unsigned short speed; char p2[2]; int i4; char p8[0x18 - 8]; int idx; char p1C[0x28 - 0x1C];
    float f28; union { int i; struct { short hi, lo; } s; } u2C; char p30[0x30 - 0x30]; int i30; int i34; int i38; char p3C[0x58 - 0x3C];
    unsigned short f58; char p5A[0x60 - 0x5A];
} TypeInfo;
typedef struct {
    char p0[0x10]; int id; char p14[0x1D8 - 0x14]; float rate; char p1DC[0x1E4 - 0x1DC];
    unsigned char r, g, b; char p1E7[0x250 - 0x1E7];
} Player;
typedef struct {
    char p0[8]; unsigned short next; char pA[2]; Tgt *obj; char p10[0x44 - 0x10];
} Node;
typedef struct {
    char p0[0xA]; unsigned char flags; unsigned char timer; Vec3 pos; unsigned short a24;
    unsigned short a26; char p1C[2]; unsigned char s30; unsigned char s31; unsigned char type;
    unsigned char alpha; unsigned char owner; unsigned char b35; int id; unsigned short snd;
    char p2A[2]; int t44; int t48; int t52; Tgt *target; int hp; int **ctl;
} Obj;

extern TypeInfo D_80123BB0[];
extern Player D_80235F00[];
extern Node D_80224EF0[];
extern unsigned short D_80224E70;
extern int D_80117EB4;
extern int D_8023A060;
extern int D_8021945C;
extern float D_80219488;
extern void *D_803A53A0[];

extern void func_800E2E0C(Obj *, Vec3 *, unsigned short *, unsigned char *);
extern void func_80097FB4(int, float, float, float, int);
extern void func_800DA7D0(void *, Vec3 *, int, int, int, int, int, int, int);
extern void func_800B22F8(unsigned short);
extern int func_8009D914(void);
extern int func_80096250(Tgt *);
extern unsigned short func_8009E9C8(Vec3 *, float *);
extern int func_8009D850(unsigned short, unsigned short, unsigned short *);
extern int func_800E29A4(int, Vec3 *, int, int);
extern unsigned short func_800B1898(Obj *, short, short, int, short, short, short, short, short, short, int, int, int);
extern unsigned short func_8009D81C(unsigned short, unsigned short);
extern void func_800E2F9C(Obj *, Vec3 *, int, int);

static inline Player *getPlayer(int o) {
    if (o == 127) return 0;
    return &D_80235F00[o];
}
static inline int slot_index(unsigned char x) { return x; }
static inline Player *getPlayerC(int o) {
    int k = slot_index(o);
    if (o == 127) return 0;
    return &D_80235F00[k];
}

static inline Tgt *findTarget(Vec3 *pp, unsigned char team, Player *p, int range) {
    short i;
    float bestd;
    Tgt *best;
    Tgt *o;
    int myid;

    i = D_80224E70;
    bestd = 1e9f;
    best = 0;
    myid = p->id;
    while (i != -1) {
        { Node *n = &D_80224EF0[i]; o = n->obj; }
        if (o != 0 && o->team == team && o->ctl->id != myid && func_80096250(o)) {
            float dx = o->px - pp->x;
            float dy = o->py - pp->y;
            float d = dx * dx + dy * dy;
            if (d < bestd) { bestd = d; best = o; }
        }
        i = D_80224EF0[i].next;
    }
    return bestd < (float)range ? best : 0;
}

static inline int calcRange(Obj *e) {
    int range = D_8023A060 * D_8023A060 * 0.92f;
    if (e->type == 1) range /= 3;
    return range;
}

void func_800E3460(Obj *e, int *done) {
    TypeInfo *ti;
    Vec3 pos;
    unsigned short ang;
    unsigned char flag;
    unsigned short tmp;
    unsigned short tgt;
    int have;
    float rate;

    tgt = 0;
    have = 0;
    ti = &D_80123BB0[e->type];
    rate = getPlayer(e->owner)->rate;
    if (e->s30 == 4 || (e->ctl != 0 && *e->ctl == 0)) goto end;
    func_800E2E0C(e, &pos, &ang, &flag);
    if (e->hp <= 0) {
        if (!(e->flags & 2)) {
            Player *p = getPlayerC(e->owner);
            func_80097FB4(8, pos.x, pos.x, 1.0f, flag);
            func_800DA7D0(D_803A53A0[ti->idx], &pos, ang, flag, 0, 0, p->r, p->g, p->b);
        }
    end:
        if (e->snd != 0xFFFF) func_800B22F8(e->snd);
        *done = 1;
        return;
    }
    {
        {
            int range;
            Tgt *best;
            float bestd;
            Tgt *o;
            short i;
            int team;
            int myid;
            Player *p;
            Tgt *res;
            Vec3 *pp;

            if (D_80117EB4 == 10) return;
            if (e->timer != 0) e->timer--;
            switch (e->s30) {
            case 0:
                if ((func_8009D914() & 0xF) == 0) {
                    p = getPlayer(e->owner); res = findTarget(&pos, flag, p, calcRange(e));
                    e->target = res;
                    if (res != 0) {
                        int id = *res->idp;
                        e->s30 = 1;
                        e->timer = 0;
                        e->id = id;
                    }
                }
                break;
            case 1:
                if ((func_8009D914() & 0xF) == 0) {
                    p = getPlayer(e->owner); best = findTarget(&pos, flag, p, calcRange(e));
                    if (best == 0) {
                        e->s30 = 0;
                        e->target = 0;
                        break;
                    }
                    if (e->target != best) e->timer = 0;
                    e->target = best;
                    e->id = *best->idp;
                } else {
                    if (!(e->target->flags & 1) || *e->target->idp != e->id) {
                        e->s30 = 0;
                        break;
                    }
                }
                tgt = func_8009E9C8(&pos, &e->target->px);
                have = 1;
                break;
            case 3:
                have = 1;
                tgt = ang + 5462;
                if (D_8021945C > e->t44) e->s30 = 0;
                break;
            }
            if (have) {
                int step;
                int d1, d2;
                char c;
                d1 = tgt - ang;
                d2 = ang - tgt;
                step = (unsigned int)(D_80219488 * (float)ti->speed * rate);
                if (e->s30 == 3) step = (unsigned short)step * 3;
                if ((unsigned short)d1 < (unsigned short)d2) {
                    c = (unsigned short)step < (unsigned short)d1;
{ int x = d1; if (c) x = step; e->a24 += x; }
{ int x = d1; if (c) x = step; ang += x; }
                } else {
                    c = (unsigned short)step < (unsigned short)d2;
{ int x = d2; if (c) x = step; e->a24 -= x; }
{ int x = d2; if (c) x = step; ang -= x; }
                }
                if (ti->f58 != 0) {
                    int r = (short)func_8009D850(e->a26, e->a24, &tmp);
                    if (tmp > ti->f58) {
                        unsigned short d = tmp - ti->f58;
                        if (r == 1) { e->a24 -= d; ang -= d; }
                        else { e->a24 += d; ang += d; }
                    }
                }
            }
            switch (e->s31) {
            case 1:
                if (e->s30 == 3 || (e->s30 != 0 && ti->i34 < D_8021945C - e->t52)) {
                    if ((func_8009D914() & 7) == 0 && func_800E29A4(e->type, &e->pos, e->a26, flag)) {
                        func_80097FB4(23, pos.x, pos.y, 1.0f, flag);
                        e->s31 = 4;
                        e->snd = func_800B1898(e, e->pos.x, e->pos.y, 0,
                            -D_80123BB0[e->type].u2C.i, D_80123BB0[e->type].u2C.s.lo,
                            -D_80123BB0[e->type].u2C.i, D_80123BB0[e->type].u2C.s.lo,
                            e->pos.z - 50.0f,
                            e->pos.z + D_80123BB0[e->type].f28 > 70.0f ? (short)(e->pos.z + D_80123BB0[e->type].f28) : 70,
                            e->a26, 4096, flag);
                    }
                }
                break;
            case 4: {
                int v = e->alpha + (float)ti->i38 * D_80219488;
                if (v < 255) { e->alpha = v; break; }
                e->alpha = 255;
                e->s31 = 2;
                { unsigned short t = ti->i30 + 255 + (func_8009D914() & 3); e->b35 = t; }
                break;
            }
            case 2:
                if (e->b35 == 0 || e->s30 == 0 || e->s30 == 3) {
                    int t = D_8021945C;
                    e->s31 = 3;
                    e->t52 = t;
                }
                break;
            case 3: {
                int v = e->alpha - (float)ti->i38 * D_80219488;
                if (v <= 0) {
                e->alpha = 0;
                e->s31 = 1;
                func_800B22F8(e->snd);
                e->snd = 0xFFFF; } else e->alpha = v;
                break;
            }
            }
            if ((e->s31 == 2 || e->s31 == 0) && have && func_8009D81C(tgt, ang) < 512 && e->target != 0
                && (int)((float)ti->i4 / rate) < D_8021945C - e->t48) {
                func_800E2F9C(e, &pos, ang, flag);
            }
        }
    }
}
