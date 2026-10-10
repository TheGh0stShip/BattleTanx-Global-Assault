/* RODATA_VRAM 0x80073184 */
typedef short s16;
typedef unsigned short u16;

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    unsigned int kind;
    int unk04;
    u16 links[4];
    s16 x;
    s16 z;
    s16 y;
    s16 dx0;
    s16 dx1;
    s16 dz0;
    s16 dz1;
    s16 height0;
    s16 height1;
    s16 radius;
    u16 angle;
    u16 grid;
} GridNode;

typedef struct {
    void *data;
    u16 id;
    char pad06[0x24 - 6];
} Contact;

extern GridNode D_803978E0[];

void func_800B2364(Vec2 *input, u16 angle, Vec2 *output);
u16 func_800B5130(Vec2 *point, int mask, u16 grid, Contact *out, u16 single);
float func_800B83A8(u16 id, Vec2 *point);
int abs(int value);

static inline float sloped_height(u16 id, Vec2 *point) {
    GridNode *node = &D_803978E0[id];
    Vec2 delta;
    Vec2 rotated;

    delta.x = point->x - node->x;
    delta.y = point->y - node->z;
    func_800B2364(&delta, node->angle, &rotated);
    return (node->height0 + node->y) +
        (float)(node->height1 - node->height0) *
        (rotated.y - node->dz0) / (node->dz1 - node->dz0);
}

static inline float flat_height(u16 id) {
    GridNode *node = &D_803978E0[id];

    if (abs(node->height0) > abs(node->height1)) {
        return node->height0 + node->y;
    }
    return node->height1 + node->y;
}

float func_800B93A4(Vec2 *point, u16 grid) {
    Contact hits[32];
    float best = -1000.0f;
    float height;
    u16 count;
    u16 i;
    GridNode *node;

    count = func_800B5130(point, 0xE0, grid, hits, 0);
    for (i = 0; i < count; i++) {
        node = &D_803978E0[hits[i].id];
        switch (node->kind & 0xE0) {
        case 0x40:
            height = func_800B83A8(hits[i].id, point);
            break;
        case 0x80:
            height = sloped_height(hits[i].id, point);
            break;
        case 0x20:
            height = flat_height(hits[i].id);
            break;
        default:
            height = 0.0f;
            break;
        }
        if (best < height) {
            best = height;
        }
    }
    if (best == -1000.0f) {
        return 0.0f;
    }
    return best;
}
