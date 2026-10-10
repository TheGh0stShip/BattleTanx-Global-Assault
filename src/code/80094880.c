/* SPAN 0x80094F40 */
/* RODATA_VRAM 0x80072158 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { float m[4][4]; u8 flag; } Mtx17;
typedef struct { float x, y; } Vec2;

typedef struct {
    char p0[0xB];
    u8 id;              /* 0x0B */
    char pc[4];
    int team;           /* 0x10 */
    char p14[0x114 - 0x14];
    int f114;           /* 0x114 */
} Unit;

typedef struct {
    char p0[0x90];
    int flags;          /* 0x90 */
    float ox, oy, oz;   /* 0x94 */
    char pa0[0xD0 - 0xA0];
} Zone;

typedef struct {
    float dx, dy, speed; /* 0x00 */
    float unkC, unk10, unk14;
    u16 mass;           /* 0x18 */
} Mover;

typedef struct Tank {
    int b0;
    char p4[4];
    float x, y, z;      /* 0x08 */
    char p14[8];
    float f1C;          /* 0x1C */
    u16 heading;        /* 0x20 */
    u16 h22;
    u16 h24, h26;       /* 0x24 */
    Mover mv;           /* 0x28 */
    char p44[0x94 - 0x28 - sizeof(Mover)];
    u8 b94;             /* 0x94 */
    char p95[3];
    int zone;           /* 0x98 */
    char p9c[0x1D0 - 0x9C];
    Unit *owner;        /* 0x1D0 */
    char p1d4[4];
    int hp;             /* 0x1D8 */
    int shield;         /* 0x1DC */
    int flags;          /* 0x1E0 */
    char p1e4[0x260 - 0x1E4];
    int hitTime;        /* 0x260 */
    char p264[0x4C0 - 0x264];
    float scale;        /* 0x4C0 */
} Tank;

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

static inline void damage(Tank *t, int dmg, Unit *src) {
    if (src != 0 && src->team == t->owner->team) {
        func_800A9B64(src, 7);
    }
    if (D_80125AB0 != 0 && D_80114694 == 0 && (t->flags & 2)) return;
    t->hp -= dmg;
    if (t->hitTime != -1) t->hitTime = D_8021945C;
    if (t->hp > 0) return;
    t->hp = 1;
    func_8008B82C(t, src ? src->id : 127, 1);
}

void func_80094880(Tank *t, Tank *o, Proj *p, int *out) {
    Mover *m;
    u16 lvl;
    u16 d1, d2;

    if (p == 0) return;
    m = p->mv;
    if (m == 0) return;
    if (t->flags & 2) {
        lvl = (u16)(m->speed * m->mass) >> 8;
        lvl--;
        if (lvl != 0) {
            if (lvl > 10) lvl = 10;
            func_80098B58(t->owner->id, 10, lvl, 10 - lvl);
        }
    }
    func_800B62A4(&t->mv, m, 1.0f, &t->x, p, &d1, &d2);
    if (d2 != 0) {
        *out = 10;
        func_8009060C(o, out);
        damage(o, d2, t->owner);
    } else {
        *out = 9;
        func_8009060C(o, out);
    }
    if (d1 != 0) {
        damage(t, d1, o->owner);
        *out = 10;
    } else {
        *out = 9;
    }
}

void func_80094B40(Tank *t, int unused, int *mode, Mtx17 *m) {
    Vec2 off;

    if (*mode == 0) {
        func_8009EEE0(m);
        func_8009EFD4(m, t->x, t->z, t->y, t->heading);
    } else {
        if (D_80122E28[t->zone].flags & 4) {
            func_8009FF1C(&D_80122E28[t->zone].ox, &off, t->heading);
        } else {
            off.x = off.y = 0.0f;
        }
        func_8009EEE0(m);
        if (t->owner->f114 == 0) {
            func_8009EFD4(m, t->x + off.x, t->f1C * 0.7f, t->y + off.y, t->heading + t->h24 + t->h26);
        } else {
            func_8009EFD4(m, t->x + off.x, t->z, t->y + off.y, t->heading + t->h24 + t->h26);
        }
    }
    if (t->scale != 1.0f) {
        func_8009F824(m, t->scale, t->scale, t->scale);
    }
    m->flag = t->b94;
}

void func_80094CE4(Tank *t, Msg *msg, Proj *p, int *out) {
    Tank *o;
    int r;

    switch (msg->type) {
    case 4:
        *out = 1;
        o = msg->obj;
        if (o->scale == 1.0f && t->scale < 1.0f) {
            damage(t, 10000, o->owner);
        } else {
            func_80094880(t, msg->obj, p, out);
        }
        return;
    case 11:
    case 37:
    case 38:
    case 50:
        func_8008BF5C(t, p->dmg, p->f14, 0, p, p->kind, out + 1, 0);
        if (t->shield <= 0) {
            r = 1;
        } else {
            switch (msg->type) {
            case 37:
            case 38:
            case 50:
                r = 5;
                break;
            default:
                r = 1;
                break;
            }
        }
        break;
    case 35:
        switch ((unsigned int)t->zone) {
        case 2:
        case 12:
        case 13:
        case 14:
            p->dmg /= 3;
            break;
        }
        func_8008BF5C(t, p->dmg, p->f14, 0, p, p->kind, out + 1, 1);
        *out = 1;
        return;
    case 28:
        r = 1;
        break;
    default:
        return;
    }
    *out = r;
}
