typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
    int unk0;
    int unk4;
    u16 link[4];
    s16 x;
    s16 z;
    s16 y;
    s16 dx0;
    s16 dx1;
    s16 dz0;
    s16 dz1;
    u16 unk1E;
    u16 unk20;
    s16 radius;
    u16 unk24;
    u16 grid;
} GridNode;

typedef struct {
    float x;
    float y;
} Vec2;

extern GridNode D_803978E0[];
extern float D_80073074;

extern float func_8009D4B0(u16 angle);
extern float func_8009D510(u16 angle);
extern void func_8009E948(float *value);

static inline void rotate_vec2(Vec2 *input, u16 angle, Vec2 *output) {
    float sine;
    float cosine;

    switch (angle) {
    case 0:
        output->x = input->x;
        output->y = input->y;
        break;
    case 0x4000:
        output->x = -input->y;
        output->y = input->x;
        break;
    case 0x8000:
        output->x = -input->x;
        output->y = -input->y;
        break;
    case 0xC000:
        output->x = input->y;
        output->y = -input->x;
        break;
    default:
        sine = func_8009D4B0(angle);
        cosine = func_8009D510(angle);
        output->x = cosine * input->x + -sine * input->y;
        output->y = sine * input->x + cosine * input->y;
        break;
    }
}

void func_800B2488(s16 x, s16 z, u16 id, Vec2 *out) {
    GridNode *node = &D_803978E0[id];
    Vec2 delta;
    Vec2 rotated;
    s16 width;
    s16 height;
    float sine;
    float cosine;
    s16 midpoint_y;
    float midpoint_x;
    u16 hits;

    delta.x = x - node->x;
    delta.y = z - node->z;
    rotate_vec2(&delta, node->unk24, &rotated);
    width = node->dx1 - node->dx0;
    height = node->dz1 - node->dz0;
    if ((width < 2) & (height < 2)) {
        height = 2;
        width = 2;
    }
    out->x = out->y = 0.0f;
    hits = 0;
    if (node->unk24 == 0) {
        cosine = D_80073074;
        sine = 0.0f;
    } else {
        sine = func_8009D4B0(node->unk24);
        cosine = func_8009D510(node->unk24);
    }
    if (height >= 2) {
        if (rotated.x <= node->dx0) {
            out->x -= cosine;
            out->y += sine;
            hits++;
        } else if (node->dx1 <= rotated.x) {
            out->x += cosine;
            out->y -= sine;
            hits++;
        }
    }
    if (width >= 2) {
        if (rotated.y <= node->dz0) {
            out->x -= sine;
            out->y -= cosine;
            hits++;
        } else if (node->dz1 <= rotated.y) {
            out->x += sine;
            out->y += cosine;
            hits++;
        }
    }
    if (out->x == 0.0f && out->y == 0.0f) {
        midpoint_x = (node->dx1 - node->dx0) >> 1;
        midpoint_y = (u32)(-node->dz0 + node->dz1) >> 1;
        if (rotated.x < midpoint_x) {
            out->x -= cosine;
            out->y += sine;
            hits++;
        } else if (midpoint_x < rotated.x) {
            out->x += cosine;
            out->y -= sine;
            hits++;
        }
        if (rotated.y < midpoint_y) {
            out->x -= sine;
            out->y -= cosine;
            hits++;
        } else if (midpoint_y < rotated.y) {
            out->x += sine;
            out->y += cosine;
            hits++;
        }
    }
    if (hits >= 2) {
        func_8009E948(&out->x);
    }
}
