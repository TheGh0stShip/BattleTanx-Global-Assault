/* SPAN 0x800AD6A8 */
/* RODATA_VRAM 0x80072E90 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { float x, y; } Vec2;

typedef struct {
    Vec2 pos;
    char p8[0x28 - 8];
} View;

typedef struct {
    void *a;
    void *b;
    void *c;
} Part;

typedef struct {
    Part *parts;
    u8 n;
} Lod;

typedef struct {
    Lod *lods;
    char p4[0xC - 4];
    u8 lodMode;         /* 0x0C */
} Model;

typedef struct {
    char p0[0x30];
    float x;            /* 0x30 */
    float unk34;
    float y;            /* 0x38 */
} Obj;

typedef struct {
    char p0[0x18];
    int x;              /* 0x18 */
    int y;              /* 0x1C */
} FixObj;

typedef struct {
    void *world;        /* 0x80219498 */
    int track;          /* 0x8021949C */
    unsigned int mode;  /* 0x802194A0 */
    u8 nHuman;          /* 0x802194A4 */
    u8 nPlayers;        /* 0x802194A5 */
    u8 nTanks;          /* 0x802194A6 */
    u8 nSlots;          /* 0x802194A7 */
    u8 teams[5];        /* 0x802194A8 */
    char pad[3];
    int time;           /* 0x802194B0 */
    View views[4];      /* 0x802194B4 */
} GameState;

extern GameState D_80219498;

int func_800AA664(float x, float y, float r, u8 kind, View *v);

u8 func_800AD14C(float x, float y, float r, u8 kind) {
    u8 mask = 0;
    int n = D_80219498.nPlayers;
    int i;

    for (i = 0; i < n; i++) {
        if (func_800AA664(x, y, r, kind, &D_80219498.views[i]) != 0) {
            mask |= 1 << i;
        }
    }
    return mask;
}

inline int func_800AD214(Vec2 *a, Vec2 *b, int kind) {
    float dx;
    float dy;
    float d;

    switch (kind) {
    case 2:
        dx = a->x - b->x;
        dy = a->y - b->y;
        if (90000.0f < dx * dx + dy * dy) {
            return 1;
        }
        return 0;
    case 3:
        dx = a->x - b->x;
        dy = a->y - b->y;
        d = dx * dx + dy * dy;
        if (2890000.0f < d) {
            return 2;
        }
        if (1000000.0f < d) {
            return 1;
        }
        return 0;
    }
    return 0;
}

void func_8007B1F0(void *a, void *c, void *b, void *arg1, int a5, Obj *o, FixObj *t, int view, int a4, int z);
void func_8007B498(void *a, void *c, void *b, int a7, Vec2 *p, float x, float y, float z, u16 ang, int view,
                   int a6, u8 flag);

void func_800AD2D0(Model *m, void *arg1, Obj *o, FixObj *t, int a4, int a5, int view) {
    Vec2 p;
    Lod *lod;
    int i;

    if (m != 0) {
        if (o != 0) {
            p.x = o->x;
            p.y = o->y;
        } else {
            p.x = t->x / 65536.0f;
            p.y = t->y / 65536.0f;
        }
        lod = &m->lods[func_800AD214(&D_80219498.views[view].pos, &p, m->lodMode)];
        for (i = 0; i < lod->n; i++) {
            Part *pt = &lod->parts[i];

            func_8007B1F0(pt->a, pt->c, pt->b, arg1, a5, o, t, view, a4, 0);
        }
    }
}

void func_800AD4C4(Model *m, Vec2 *p, float x, float y, float z, u16 ang, int a6, int a7, int view, u8 flag) {
    Lod *lod;
    int i;

    if (m != 0) {
        lod = &m->lods[func_800AD214(&D_80219498.views[view].pos, p, m->lodMode)];
        for (i = 0; i < lod->n; i++) {
            Part *pt = &lod->parts[i];

            func_8007B498(pt->a, pt->c, pt->b, a7, p, x, y, z, ang, view, a6, flag);
        }
    }
}
