/* RODATA_VRAM 0x80076E40 */
#include "types.h"

typedef struct Vec2f {
    f32 x;
    f32 y;
} Vec2f;

typedef struct TextureVertex {
    u8 pad00[8];
    s16 s;
    s16 t;
    u8 pad0C[4];
} TextureVertex;

extern f32 func_8009D510(u16);
extern f32 func_8009D4B0(u16);
extern Vec2f D_80125634[];

void func_800F0A20(u16 angle, TextureVertex* vertices) {
    f32 cosine = func_8009D510(angle);
    f32 sine = func_8009D4B0(angle);
    s32 i;

    for (i = 0; i < 4; i++) {
        f32 x = D_80125634[i].x;
        f32 y = D_80125634[i].y;
        f32 s = x * cosine - y * sine;
        f32 t = x * sine + y * cosine;

        s += 0.5;
        t += 0.5;
        s *= 2048.0f;
        t *= 2048.0f;
        vertices[i].s = s;
        vertices[i].t = t;
    }
}
