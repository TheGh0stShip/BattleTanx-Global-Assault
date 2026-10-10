/* SPAN 0x800AAAE0 */
typedef unsigned char u8;
typedef int s32;
typedef float f32;

typedef struct { f32 x, y; } Vec2;

typedef struct {
    f32 x, y;           /* 0x00 */
    f32 c0, c1;         /* 0x08 */
    f32 c2, c3;         /* 0x10 */
    f32 ox, oy;         /* 0x18 */
    char p20[4];
    u8 all;             /* 0x24 */
    u8 kind;            /* 0x25 */
} Region;

s32 func_800AA8A8(Vec2 *pts, u8 kind, Region *g) {
    Vec2 q[4];
    u8 code[4];
    u8 mask = 0xFF;
    s32 i;

    if (g->kind != kind) {
        return 0;
    }
    if (g->all != 0) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        f32 px = pts[i].x + g->ox;
        f32 py = pts[i].y + g->oy;

        q[i].x = g->c2 * px - g->c3 * py;
        q[i].y = g->c0 * py + g->c1 * px;
        if (q[i].x > 0.0f) {
            if (q[i].y < 0.0f) {
                code[i] = 5;
            } else if (q[i].x < q[i].y) {
                return 1;
            } else {
                code[i] = 1;
            }
        } else if (q[i].y < 0.0f) {
            code[i] = 6;
        } else if (-q[i].x < q[i].y) {
            return 1;
        } else {
            code[i] = 2;
        }
    }
    for (i = 0; i < 4; i++) {
        mask &= code[i];
    }
    if (mask != 0) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if ((code[i] | code[(i + 1) % 4]) == 3) {
            return 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if ((code[i] ^ code[(i + 1) % 4]) == 7) {
            u8 j = (i + 1) % 4;

            if (-q[i].x / (q[j].x - q[i].x) * (q[j].y - q[i].y) + q[i].y > 0.0f) {
                return 1;
            }
        }
    }
    return 0;
}

