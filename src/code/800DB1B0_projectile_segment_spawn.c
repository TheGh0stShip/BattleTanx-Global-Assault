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
