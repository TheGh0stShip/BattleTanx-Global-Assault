#include "types.h"


void func_8008F78C(void *object) {
    f32 x = *(f32 *)((u8 *)object + 0x1E4);
    f32 scale = -0.6f;
    f32 damping = 0.65f;
    f32 dx = x * scale;
    f32 dy = *(f32 *)((u8 *)object + 0x1EC) * scale;
    f32 vx = (*(f32 *)((u8 *)object + 0x1E8) + dx) * damping;
    f32 vy = (*(f32 *)((u8 *)object + 0x1F0) + dy) * damping;

    *(f32 *)((u8 *)object + 0x1E8) = vx;
    *(f32 *)((u8 *)object + 0x1F0) = vy;
    *(f32 *)((u8 *)object + 0x1E4) += vx;
    *(f32 *)((u8 *)object + 0x1EC) += vy;
}
