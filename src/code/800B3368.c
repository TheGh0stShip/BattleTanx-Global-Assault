/* SPAN 0x800B33FC */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
    int unk0;
    int unk4;           /* 0x04 */
    u16 link[4];        /* 0x08 */
    s16 x, z, y;        /* 0x10 */
    s16 dx0, dx1;       /* 0x16 */
    s16 dz0, dz1;       /* 0x1A */
    u16 unk1E, unk20;   /* 0x1E */
    s16 radius;         /* 0x22 */
    u16 unk24;          /* 0x24 */
    u16 grid;          /* 0x26 */
} GridNode;

typedef struct {
    s16 x0, z0;         /* 0x00 */
    u16 w, h;           /* 0x04 */
    u16 cw, ch;         /* 0x08 */
    u16 *cells;         /* 0x0C */
    float scale;        /* 0x10 */
    void *tbl;          /* 0x14 */
} Grid;

typedef struct {
    int unk0;
    s16 x0, z0, x1, z1; /* 0x04 */
    char padC[0x14];
} Area;

typedef struct {
    Area areas[6];      /* 0x00 */
    char padC0[4];
    u8 nAreas;          /* 0xC4 */
} World;

extern GridNode D_803978E0[];
extern Grid D_803977F0[];

typedef struct { float x, y; } Vec2;
typedef struct { float x0, x1, y0, y1; } Box;

float func_8009D4B0(u16 a);
float func_8009D510(u16 a);
u16 func_800B2890(s16 x, s16 z, u16 id);

extern inline void func_800B2364(Vec2 *in, u16 ang, Vec2 *out) {
    float s, c;

    switch (ang) {
    case 0:
        out->x = in->x;
        out->y = in->y;
        break;
    case 0x4000:
        out->x = -in->y;
        out->y = in->x;
        break;
    case 0x8000:
        out->x = -in->x;
        out->y = -in->y;
        break;
    case 0xC000:
        out->x = in->y;
        out->y = -in->x;
        break;
    default:
        s = func_8009D4B0(ang);
        c = func_8009D510(ang);
        out->x = c * in->x + -s * in->y;
        out->y = s * in->x + c * in->y;
        break;
    }
}

u16 func_800B2A60(s16 x, s16 z, s16 y, u16 id);

extern inline void func_800B2AE4(GridNode *a, GridNode *b, GridNode *out) {
    float dx = b->x - a->x;
    float dz = b->z - a->z;
    float c = func_8009D510(a->unk24);
    float s = func_8009D4B0(a->unk24);

    out->x = c * dx + -s * dz;
    out->z = s * dx + c * dz;
    out->unk24 = b->unk24 - a->unk24;
    out->dx0 = b->dx0;
    out->dx1 = b->dx1;
    out->dz0 = b->dz0;
    out->dz1 = b->dz1;
}

void func_800B2BE4(GridNode *n, Vec2 *q);

void func_8009E948(float *v);

void func_800B2488(s16 x, s16 z, u16 id, Vec2 *out);

u16 func_800B2890(s16 x, s16 z, u16 id);

u16 func_800B2DF4(GridNode *a, GridNode *b);

u16 func_800B3018(u16 ia, u16 ib, Vec2 *out, s16 *side);


extern u16 D_80397650;

u16 func_800B33FC(int ia, int ib, Vec2 *out, s16 *side);

typedef struct {
    int data;           /* 0x00 */
    u16 id;             /* 0x04 */
    float x, z, y;      /* 0x08 */
    Vec2 push;          /* 0x14 */
    float unk1C;        /* 0x1C */
    s16 side;           /* 0x20 */
} Contact;

extern u16 D_801166F0;
extern u16 D_80397658[];

extern inline void func_800B0B8C(u16 id) {
    GridNode *n = &D_803978E0[id];

    n->grid |= 0x1000;
    D_80397658[D_801166F0] = id;
    D_801166F0++;
}

u16 func_800B3504(u16 id, u16 head, u16 dir, int mask, Contact *out, u16 base, u16 single);

extern u16 D_801166E0[4];
extern u16 D_801166E8[4];

extern inline void func_800B0BE0(void) {
    u16 i;

    for (i = 0; i < D_801166F0; i++) {
        (&D_803978E0[(&D_80397658[i])[0]])->grid &= ~0x1000;
    }
    D_801166F0 = 0;
}

static inline u16 query_done(GridNode *n, u16 grow, u16 cnt) {
    if (grow) {
        n->dx1 -= grow;
        n->dz0 += grow;
        n->dx0 += grow;
        n->dx1 -= grow;
    }
    func_800B0BE0();
    return cnt;
}

u16 func_800B3748(u16 id, int mask, Contact *out, u16 grow, u16 single);

u16 func_800B4F38(Vec2 *p, u16 layer, u16 head, u16 dir, int mask, Contact *out, u16 base, u16 single);

typedef struct { float x, y, z; } Vec3f;
typedef struct { float x, y, k; } Normal;

u16 func_800B3B60(Vec3f *a, Vec3f *b, s16 id, Vec3f *pt, Normal *nrm);
u16 func_800B4294(Vec3f *a, Vec3f *b, s16 id, Vec3f *pt, Normal *nrm);

u16 func_800B4684(Vec3f *a, Vec3f *b, u16 head, u16 dir, int mask, u16 layer, u16 grow, int excl, Contact *best, u32 *bestd);


u16 func_800B3368(u16 ia, u16 ib, Vec2 *out, s16 *side) {
    GridNode *a = &D_803978E0[ia];
    GridNode *b = &D_803978E0[ib];

    if (a->y + (s16)a->unk1E > b->y + (s16)b->unk20) return 0;
    if (a->y + (s16)a->unk20 < b->y + (s16)b->unk1E) return 0;
    return func_800B3018(ia, ib, out, side);
}
