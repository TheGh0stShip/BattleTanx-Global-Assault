/* SPAN 0x80095B50 */
/* RODATA_VRAM 0x80072228 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { float m[4][4]; u8 flag; } Mtx17;
typedef struct { float x, y; } Vec2;

typedef struct Unit {
    char p0[0xA];
    u8 fA;              /* 0x0A */
    u8 id;              /* 0x0B */
    char pc[4];
    int team;           /* 0x10 */
    char p14[0x114 - 0x14];
    int f114;           /* 0x114 */
    char p118[0x1B8 - 0x118];
    struct Tank *slots[4]; /* 0x1B8 */
    u8 counts[4];       /* 0x1C8 */
    char p1cc[0x250 - 0x1CC];
} Unit;

typedef struct {
    char p0[0x1A];
    s16 h1A;            /* 0x1A */
    char p1c[2];
    s16 h1E;            /* 0x1E */
    char p20[0x90 - 0x20];
    int flags;          /* 0x90 */
    float ox, oy, oz;   /* 0x94 */
    char pa0[0xD0 - 0xA0];
} Zone;

typedef struct {
    float dx, dy, speed; /* 0x00 */
    float unkC, unk10, unk14;
    u16 mass;           /* 0x18 */
    char p1a[0x54 - 0x1A];
    float f54[4];       /* 0x54 */
    u16 hits;           /* 0x64 */
} Mover;

typedef struct Tank {
    int b0;
    struct Tank *next;  /* 0x04 */
    float x, y, z;      /* 0x08 */
    char p14[8];
    float f1C;          /* 0x1C */
    u16 heading;        /* 0x20 */
    u16 h22;
    u16 h24, h26;       /* 0x24 */
    Mover mv;           /* 0x28 */
    char p44[0x94 - 0x28 - sizeof(Mover)];
    u8 b94;             /* 0x94 */
    u8 b95;             /* 0x95 */
    char p96[2];
    int zone;           /* 0x98 */
    int slot;           /* 0x9C */
    char pa0[0x168 - 0xA0];
    int state;          /* 0x168 */
    u8 f16C;            /* 0x16C */
    char p16d[0x1D0 - 0x16D];
    Unit *owner;        /* 0x1D0 */
    char p1d4[4];
    int hp;             /* 0x1D8 */
    int shield;         /* 0x1DC */
    int flags;          /* 0x1E0 */
    char p1e4[0x1F4 - 0x1E4];
    u16 node;           /* 0x1F4 */
    char p1f6[0x260 - 0x1F6];
    int hitTime;        /* 0x260 */
    char p264[0x4A0 - 0x264];
    struct Attach *att; /* 0x4A0 */
    char p4a4[0x4C0 - 0x4A4];
    float scale;        /* 0x4C0 */
} Tank;

typedef struct Attach {
    int unk0;
    int type;           /* 0x04 */
    char p8[0x2B - 8];
    u8 cnt;             /* 0x2B */
} Attach;

typedef struct {
    Vec2 pos;           /* 0x00 */
    char p8[4];
    u16 kind;           /* 0x0C */
    char pe[2];
    int dmg;            /* 0x10 */
    int f14;            /* 0x14 */
    char p18[8];
    Mover *mv;          /* 0x20 */
} Proj;

typedef struct {
    int unk0;
    int type;           /* 0x04 */
    char p8[4];
    Tank *obj;          /* 0x0C */
} Msg;

extern u8 D_80125AB0;
extern u16 D_80114694;
extern int D_8021945C;
extern Zone D_80122E28[];

void func_800A9B64(Unit *u, int a);
void func_8008B82C(Tank *t, int id, int big);
void func_80098B58(int team, int a, int b, int c);
void func_800B62A4(Mover *m, Mover *o, float f, void *a, void *b, u16 *d1, u16 *d2);
void func_8009060C(Tank *t, int *msg);
void func_8009EEE0(Mtx17 *m);
void func_8009EFD4(Mtx17 *m, float x, float z, float y, u16 ang);
void func_8009F824(Mtx17 *m, float sx, float sy, float sz);
void func_8009FF1C(float *v, Vec2 *out, u16 ang);
void func_8008BF5C(Tank *t, int dmg, int a2, int a3, Proj *p, u16 kind, int *out, int a7);


typedef struct {
    Vec2 pos;           /* 0x00 */
    int r2;             /* 0x08 */
    int unit;           /* 0x0C */
    int kind;           /* 0x10 */
    int dmg;            /* 0x14 */
} Blast;

typedef struct {
    u8 b0;
    char p1[0xB];
    int dmg;            /* 0x0C */
    int unit;           /* 0x10 */
} DmgMsg;

typedef struct { float x, y, z; } Vec3;
typedef struct { char p[0x60]; u8 b60, b61; } Rules;

extern Unit D_80235F00[];
extern Rules *D_8021958C;
extern char D_801155EC[];
extern int D_80397902[];

void func_80085DA8(Tank *t, int a, int b);
void func_80089F40(Tank *t, int a, int b, int c);
void func_80095B90(Tank *t, int a);
void func_800EB720(float *pos, u8 kind, float f, int n);
int func_80096294(u8 a, u8 b, int c, int d, int *out);
void func_800F6ED0(Tank *t, float f, int a, int b);
void func_800A5BD8(Vec3 *pos, u16 ang, u8 kind, float scale, void *def, void *arg);
float func_8009D8A0(float max);
void func_8009E948(float *v);
void func_800B5D1C(Mover *m, Vec2 *v);
void func_800B5F30(Mover *m, Vec2 *a, Vec2 *b, float amount);
void func_800B2BE4(void *n, Vec2 *q);
void func_80095BE4(Tank *t);
float sqrtf(float);
typedef struct { char p[0x22]; s16 radius; char p24[4]; } GridNodeR;
extern char D_803978E0[];
void func_80094B40(Tank *t, int a1, void *msg, s8 *out);
void func_80094CE4(Tank *t, int a1, void *msg, s8 *out);
typedef struct { char p[0xC]; Tank *t; } Holder;

static inline Unit *get_unit(int id) {
    return id != 127 ? &D_80235F00[id] : 0;
}

static inline void tank_damage(Tank *t, int dmg, Unit *src, u16 big) {
    if (src != 0 && src->team == t->owner->team) {
        func_800A9B64(src, 7);
    }
    if (D_80125AB0 != 0 && D_80114694 == 0 && (t->flags & 2)) return;
    t->hp -= dmg;
    if (t->hitTime != -1) t->hitTime = D_8021945C;
    if (t->hp > 0) return;
    t->hp = 1;
    if (big) {
        func_8008B82C(t, src ? src->id : 127, 1);
    } else {
        func_8008B82C(t, src ? src->id : 127, (dmg > 500) * 2);
    }
}

static inline void tank_wreck(Tank *t, int arg) {
    if (t->zone == 11) {
        t->f16C |= 0x10;
    } else {
        t->state = 26;
        func_80085DA8(t, arg, 0);
    }
}

void func_80094FBC(Tank *t, Unit *u) {
    unsigned int slot = t->slot;
    Unit *old = t->owner;
    int ns = 0;
    Tank **pp;

    if (slot == 0) return;
    if (t->flags & 0x40) {
        tank_wreck(t, 60);
        return;
    }
    func_800A9B64(u, 1);
    if (t->att != 0 && t->att->type == 13) {
        t->att->cnt--;
        t->att = 0;
    }
    func_80089F40(t, t->slot, 4, 0);
    pp = &old->slots[t->slot];
    while (*pp != 0 && *pp != t) {
        pp = &(*pp)->next;
    }
    *pp = t->next;
    t->owner->counts[slot]--;
    if (slot < 4) {
        if (slot != 0) {
            ns = 1;
            if (u->fA & 2) {
                ns = 2;
            }
        }
    }
    t->next = u->slots[ns];
    u->slots[ns] = t;
    t->owner = u;
    t->slot = ns;
    t->b95 = u->id;
    t->owner->counts[ns]++;
    func_80089F40(t, 4, t->slot, 0);
}

void func_80095168(Tank *t, int unused, int unit, int a3, int a4) {
    Unit *u;
    int pick;

    if (unit == 127) {
        u = 0;
    } else {
        u = &D_80235F00[unit];
    }
    pick = 0;
    if (u->team == t->owner->team) return;
    if (t->flags & 2) return;
    func_80095B90(t, 18);
    func_800EB720(&t->x, t->b94, 0.4f, 12);
    if ((unsigned int)(t->zone - 12) < 2) return;
    if (t->zone == 14) return;
    if (func_80096294(D_8021958C->b60, D_8021958C->b61, a3, a4, &pick)) {
        func_800F6ED0(t, D_80122E28[t->zone].h1A * 1.5f, 0, 0);
        func_800F6ED0(t, D_80122E28[t->zone].h1A * 1.5f, 0, 0);
        func_800F6ED0(t, D_80122E28[t->zone].h1A * 1.5f, 0, 0);
        func_80094FBC(t, u);
        return;
    }
    {
        Vec3 v;

        v.x = t->x;
        v.z = t->z + D_80122E28[t->zone].h1E;
        v.y = t->y;
        func_800A5BD8(&v, 0, t->b94, 1.0f, D_801155EC, 0);
    }
    tank_wreck(t, pick);
}

void func_800953E4(Tank *t, int unused, Blast *b, s8 *out) {
    Vec2 q[4];
    Vec2 d;
    Vec2 d2;
    int dist;
    Mover *mv;
    u16 corners;
    u16 bit;
    u16 i;
    float dd, k, f;
    int dmg;
    Unit *u;

    if (t == 0 || (t->flags & 8)) {
        *out = 1;
        return;
    }
    d.x = t->x - b->pos.x;
    d.y = t->y - b->pos.y;
    dist = d.x * d.x + d.y * d.y;
    if (b->r2 + ((GridNodeR *)D_803978E0)[t->node].radius < dist) return;
    mv = &t->mv;
    if (b->kind == 1) {
        func_80095168(t, b->dmg, b->unit, dist, b->r2);
        *out = 1;
        return;
    }
    corners = 0;
    bit = 0x1000;
    for (i = 0; i < 4; i++, bit <<= 1) {
        if (mv->hits & bit) continue;
        if (!corners) {
            func_800B2BE4(&((GridNodeR *)D_803978E0)[t->node], q);
            corners = 1;
        }
        d2.x = q[i].x - b->pos.x;
        d2.y = q[i].y - b->pos.y;
        if (b->r2 < (int)(d2.x * d2.x + d2.y * d2.y)) continue;
        if (mv->hits & 0xF) {
            mv->f54[i] += func_8009D8A0(5.0f);
            if (mv->f54[i] > 5.0f) {
                mv->f54[i] = 5.0f;
            }
        } else {
            mv->f54[i] -= func_8009D8A0(5.0f);
        }
        if (!(mv->hits & 0xF000)) {
            dd = sqrtf(dist);
            if (4000.0f <= dd) {
                k = 0.5f;
            } else {
                k = 1.0f - dd / 4000.0f * 0.5f;
            }
            f = k * (func_8009D8A0(1000.0f) + 3000.0f);
            func_8009E948(&d.x);
            d.x *= f;
            d.y *= f;
            func_800B5D1C(mv, &d);
            func_800B5F30(mv, &d, &d2, k * (func_8009D8A0(50.0f) + 50.0f));
            tank_damage(t, k * b->dmg, get_unit(b->unit), 0);
        }
        mv->hits |= bit;
    }
    if ((mv->hits & 0xF000) == 0xF000) {
        *out = 1;
    }
}

inline void func_80095858(Tank *t, int unused, DmgMsg *m) {
    tank_damage(t, m->dmg, get_unit(m->unit), 0);
}

void func_80095974(Holder *o, int a1, unsigned int kind, void *msg, s8 *out) {
    Tank *t;

    if (o->t == 0) return;
    switch (kind) {
    case 1:
        func_80094B40(o->t, a1, msg, out);
        break;
    case 0:
        func_80094CE4(o->t, a1, msg, out);
        break;
    case 4:
        t = o->t;
        if (t == 0) {
            *out = 0;
        } else if (t->b94 == *(u8 *)msg) {
            *out = 2;
            t->mv.hits &= 0xFFF;
        }
        break;
    case 5:
        func_800953E4(o->t, a1, msg, out);
        break;
    case 3:
        func_80095858(o->t, a1, msg);
        break;
    case 2:
        func_80095BE4(o->t);
        break;
    }
}
