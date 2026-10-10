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

extern f32 D_80219488;

extern f32 func_8009D8A0(f32 value);

void func_800F7650(EffectMotion *effect, s32 *done) {
    EffectMotion *motion = effect;
    f32 angle;
    f32 range = 0.2f;
    f32 offset = 0.9f;

    motion->life -=
        (func_8009D8A0(range) + offset) * 0.005f * D_80219488;
    if (motion->life <= 0.0f) {
        *done = 1;
        return;
    }

    motion->radius +=
        (func_8009D8A0(range) + offset) * 0.3f * D_80219488;
    if (motion->direction != 0) {
        angle = motion->angle +
                (func_8009D8A0(range) + offset) * 512.0f * D_80219488;
        motion->angle = (u32)angle;
    } else {
        angle = motion->angle -
                (func_8009D8A0(range) + offset) * 512.0f * D_80219488;
        motion->angle = (u32)angle;
    }

    range = 1.2f;
    offset = -0.4f;
    motion->position.x += (func_8009D8A0(range) + offset) * D_80219488;
    motion->position.z +=
        (func_8009D8A0(0.5f) + 1.0f) * D_80219488;
    motion->position.y += (func_8009D8A0(range) + offset) * D_80219488;
}
