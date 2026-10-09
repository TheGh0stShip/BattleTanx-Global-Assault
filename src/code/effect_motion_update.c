/* RODATA_VRAM 0x80077364 */
#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} EffectVec3;

typedef struct {
    u8 pad0[0xC];
    EffectVec3 position;
    u8 kind;
    u8 pad19;
    u16 angle;
    f32 radius;
    f32 life;
    u8 color;
    u8 direction;
} EffectMotion;

extern const f32 D_80077350;
extern const f32 D_80077354;
extern f32 D_80077358;
extern f32 D_8007735C;
extern f32 D_80077360;
extern f32 D_80077370;
extern f32 D_80077374;
extern f32 D_80077378;
extern f32 D_8007737C;
extern f32 D_80219488;

extern f32 func_8009D8A0(f32 value);

void func_800F7650(EffectMotion *effect, s32 *done) {
    EffectMotion *motion = effect;
    f32 angle;
    f32 range = D_80077350;
    f32 offset = D_80077354;

    motion->life -=
        (func_8009D8A0(range) + offset) * D_80077358 * D_80219488;
    if (motion->life <= 0.0f) {
        *done = 1;
        return;
    }

    motion->radius +=
        (func_8009D8A0(range) + offset) * D_8007735C * D_80219488;
    if (motion->direction != 0) {
        angle = motion->angle +
                (func_8009D8A0(range) + offset) * D_80077360 * D_80219488;
        motion->angle = (u32)angle;
    } else {
        angle = motion->angle -
                (func_8009D8A0(range) + offset) * 512.0f * D_80219488;
        motion->angle = (u32)angle;
    }

    range = D_80077370;
    offset = D_80077374;
    motion->position.x += (func_8009D8A0(range) + offset) * D_80219488;
    motion->position.z +=
        (func_8009D8A0(D_80077378) + D_8007737C) * D_80219488;
    motion->position.y += (func_8009D8A0(range) + offset) * D_80219488;
}
