/* SPAN 0x800B8870 */
typedef short s16;
typedef unsigned short u16;

typedef struct { float x, y; } Vec2;

typedef struct {
    int unk0;
    int unk4;           /* 0x04 */
    u16 link[4];        /* 0x08 */
    s16 x, z, y;        /* 0x10 */
    s16 dx0, dx1;       /* 0x16 */
    s16 dz0, dz1;       /* 0x1A */
    s16 h0, h1;         /* 0x1E */
    s16 radius;         /* 0x22 */
    u16 unk24;          /* 0x24 */
    u16 grid;           /* 0x26 */
} GridNode;

extern GridNode D_803978E0[];

void func_800B2364(Vec2 *in, u16 ang, Vec2 *out);


float func_800B8870(u16 id);


float func_800B87A0(u16 id, Vec2 *p) {
    GridNode *n = &D_803978E0[id];
    Vec2 d;
    Vec2 r;

    d.x = p->x - n->x;
    d.y = p->y - n->z;
    func_800B2364(&d, n->unk24, &r);
    return (n->h0 + n->y) + (float)(n->h1 - n->h0) * (r.y - n->dz0) / (n->dz1 - n->dz0);
}
