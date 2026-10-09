/* Normalizer-assisted: retail assembler omits two label-adjacent FP hazard nops. */
/* RODATA_VRAM 0x80075418 */
typedef unsigned char u8; typedef float f32; typedef int s32;
typedef struct { f32 v[17]; } Blk;
static const Blk D_80075418_init = {{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1, 0}};
extern s32 D_803A5440;
extern u8 func_800AD088(f32, f32, f32, f32, u8);
extern void func_800AE4D0(s32, s32, Blk *, s32, s32, s32, s32);
#define ABS(x) (((x) > 0.0f) ? (x) : -(x))

void func_800DB1B0(f32 *a, f32 *b, f32 h, f32 spd, u8 flag) {
    Blk s = D_80075418_init;
    u8 r;
    f32 h0, h1, dx, dy, m;

    r = func_800AD088(b[0], b[1], a[0], a[1], flag);
    if (r) {
        h0 = h + (((s32)(a[0] + a[1]) + 100000) % 30 - 15);
        h1 = h + (((s32)(b[0] + b[1]) + 100000) % 30 - 15);
        dx = b[0] - a[0];
        dy = b[1] - a[1];
        m = (ABS(dx) > ABS(dy)) ? ABS(dx) : ABS(dy);
        if (ABS(dx) < ABS(dy)) {
            if (dx > 0.0f) {
                m += dx * 3.0f / 8.0f;
            } else {
                m += -dx * 3.0f / 8.0f;
            }
        } else {
            if (dy > 0.0f) {
                m += dy * 3.0f / 8.0f;
            } else {
                m += -dy * 3.0f / 8.0f;
            }
        }
        s.v[0] = -spd * dy / m;
        s.v[2] = spd * dx / m;
        s.v[5] = spd;
        s.v[8] = dx;
        s.v[9] = h1 - h0;
        s.v[10] = dy;
        s.v[12] = a[0];
        s.v[13] = h0;
        s.v[14] = a[1];
        func_800AE4D0(D_803A5440, 0, &s, 0, 1, 0, r);
    }
}

typedef struct ProjectileTrailSource {
    u8 pad00[0xC];
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x10];
    u8 *points;
    u8 pad2C[6];
    u8 count;
    u8 index;
    u8 pad34[2];
    u8 type;
} ProjectileTrailSource;

extern void *D_803A56D4;
extern s32 D_8021945C;
extern u8 func_800AD14C(f32, f32, f32, s32);
extern void func_800AD6A8(void *, void *, f32, f32, s32, s32, s32, s32,
                         s32, s32, s32, s32);

void func_800DB4E8(ProjectileTrailSource *source) {
    f32 initial[2];
    f32 position[3];
    f32 x;
    f32 y;
    f32 scale = 2.5f;
    s32 index;
    s32 next_index;
    s32 remaining;
    u8 result;

    if (source->count == 0) {
        return;
    }

    result = func_800AD14C(source->x, source->y, 20.0f, source->type);
    if (result != 0) {
        f32 radius;
        void *effect = D_803A56D4;
        register s32 jitter asm("$5");

        x = source->x;
        position[0] = x;
        y = source->y;
        position[1] = y;
        jitter = (s32)(x + y);
        jitter = (jitter + 100000) % 30 - 15;
        position[2] = source->z + jitter;
        radius = 17.5f;
        func_800AD6A8(effect, &source->x, radius, radius, 0,
                      (D_8021945C << 9) & 0xFFFF, 0, 0, result, 0, 0, 0);
    }

    initial[0] = source->x;
    initial[1] = source->y;
    index = (source->index + 7) % 8;
    func_800DB1B0(initial, (f32 *)(source->points + index * 8), source->z,
                  scale, source->type);

    for (remaining = source->count; remaining >= 2; remaining--) {
        next_index = (index + 7) % 8;
        scale *= 0.75;
        func_800DB1B0((f32 *)(source->points + index * 8),
                      (f32 *)(source->points + next_index * 8), source->z,
                      scale, source->type);
        index = next_index;
    }
}
