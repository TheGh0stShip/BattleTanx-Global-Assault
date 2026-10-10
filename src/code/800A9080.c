/* SPAN 0x800A9660 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Player Player;

typedef struct {
    int unk0;
    int type;           /* 0x04 */
    char p8[0xC - 8];
    Player *owner;      /* 0x0C */
    char p10[0x14 - 0x10];
    int idx;            /* 0x14 */
} CamTarget;

typedef struct {
    CamTarget *target;  /* 0x00 */
    char p4[0x1E0 - 4];
    int flags;          /* 0x1E0 */
} Seat;

typedef struct {
    char p0[8];
    float x;            /* 0x08 */
    float y;            /* 0x0C */
    char p10[0x20 - 0x10];
    u16 ang;            /* 0x20 */
    char p22[0x94 - 0x22];
    u8 grid;            /* 0x94 */
    char p95[0x9C - 0x95];
    int occupied;       /* 0x9C */
    char pA0[0x1E0 - 0xA0];
    int flags;          /* 0x1E0 */
} Vehicle;

struct Player {
    char p0[0xB];
    u8 hud;             /* 0x0B */
    char pC[0x78 - 0xC];
    char cam[0x10C - 0x78]; /* 0x78 */
    CamTarget *view;    /* 0x10C */
    u8 active;          /* 0x110 */
    char p111[0x1B8 - 0x111];
    Seat *seat;         /* 0x1B8 */
    char p1BC[0x1C4 - 0x1BC];
    Seat *seat2;        /* 0x1C4 */
    char p1C8[0x1E0 - 0x1C8];
    int flags;          /* 0x1E0 */
    char p1E4[0x1E8 - 0x1E4];
    struct SpawnPt *spawns; /* 0x1E8 */
    CamTarget *fallback; /* 0x1EC */
    char p1F0[0x1F4 - 0x1F0];
    int unk1F4;         /* 0x1F4 */
    int unk1F8;         /* 0x1F8 */
};

typedef struct SpawnPt {
    float x, y;
    u16 ang;
    u8 grid;
} SpawnPt;

typedef struct {
    float x, y;         /* 0x00 */
    int unk8;           /* 0x08 */
    u16 ang;            /* 0x0C */
    u8 grid;            /* 0x0E */
    u8 hud;             /* 0x0F */
    int id;             /* 0x10 */
    int unk14;          /* 0x14 */
    int unk18;          /* 0x18 */
    int unk1C;          /* 0x1C */
    int a;              /* 0x20 */
    int b;              /* 0x24 */
    int unk28;          /* 0x28 */
} SpawnReq;

typedef struct {
    int unk0;
    char p4[0xD0 - 4];
} Weapon;

extern Weapon D_80122E38[];

CamTarget *func_800A6B7C(float x, float y, u16 ang, u8 grid, int kind, int hud);
void func_800CAED0(u8 hud);
void func_800CB0C0(u8 hud);
void func_800A6ABC(void *cam, CamTarget *t);
void func_800A8B38(Player *p);
void func_800A8F34(Player *p, int *a, int *b);
void func_800CAFA8(u8 hud);
void func_800CAD9C(int w, u8 hud);
void func_800D6E40(Player *p);
void func_80095BE4(Player *p);

typedef short s16;

int func_8008F4AC(int id, s16 x, s16 y, u16 ang, u8 grid, int a, int b, int c);
CamTarget **func_8008AE04(SpawnReq *r);

static inline void setView(Player *p, CamTarget *t) {
    int a[16];
    int b[4];

    if (p->view->type == 6) {
        func_800CAED0(p->hud);
        func_800CB0C0(p->hud);
    }
    p->view = t;
    if (p->active) {
        func_800A6ABC(p->cam, t);
        func_800A8B38(p);
        if (t->type == 6) {
            func_800A8F34(t->owner, a, b);
            func_800CAFA8((t->owner)->hud);
            func_800CAD9C(D_80122E38[a[t->idx]].unk0, (t->owner)->hud);
        }
    }
}

int func_800A9080(Player *p, int id, int reset) {
    SpawnPt *sp;
    SpawnReq r;
    int found = 0;
    int i;

    for (i = 0; i < 5; i++) {
        sp = &p->spawns[i];
        if (func_8008F4AC(id, sp->x, sp->y, sp->ang, sp->grid, 1, 0, 0) != 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return -1;
    }
    r.unk14 = 0;
    r.id = id;
    r.unk18 = 4;
    r.ang = sp->ang;
    r.x = sp->x;
    r.unk8 = 0;
    r.y = sp->y;
    r.grid = sp->grid;
    r.unk1C = 2;
    r.hud = p->hud;
    if (reset) {
        r.b = 0;
        r.a = 0;
    } else {
        r.a = p->unk1F4;
        r.b = p->unk1F8;
    }
    r.unk28 = 0;
    setView(p, *func_8008AE04(&r));
    return 0;
}

void func_800A929C(Player *p, Vehicle *v) {
    if (v->flags & 2) {
        setView(p, func_800A6B7C(v->x, v->y, v->ang, v->grid, v->occupied == 0 ? 1 : 2, p->hud));
    } else {
        func_800D6E40(p);
    }
}

int func_800A93D0(Player *p) {
    Player *o;
    Seat *s;

    if (p->view->type == 4) {
        o = p->view->owner;
        o->flags &= ~2;
        func_80095BE4(o);
    }
    s = p->seat2;
    s->flags |= 2;
    setView(p, s->target);
    return 0;
}

int func_800A94FC(Player *p) {
    CamTarget *t = 0;
    Seat *s = 0;
    Player *o;

    if (p->seat != 0) {
        s = p->seat;
        t = s->target;
    } else if (p->fallback != 0) {
        t = p->fallback;
    }
    if (t == 0) {
        return -1;
    }
    if (p->view->type == 4) {
        o = p->view->owner;
        o->flags &= ~2;
        func_80095BE4(o);
    }
    if (s != 0) {
        s->flags |= 2;
    }
    setView(p, t);
    return 0;
}
