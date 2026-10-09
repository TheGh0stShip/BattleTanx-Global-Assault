#include "types.h"

typedef struct Func8008BEC4State {
    u8 pad0[0x20];
    u16 angle;
    u8 pad22[0x1C2];
    f32 x_velocity;
    f32 y_velocity;
    f32 z_velocity;
    f32 w_velocity;
} Func8008BEC4State;

extern f32 D_80071914;
extern f32 D_80071918;
extern f32 func_8009D8A0(f32 angle);
extern f32 func_8009D510(u16 angle);
extern f32 func_8009D4B0(u16 angle);

void func_8008BEC4(Func8008BEC4State *state, u16 angle, f32 magnitude) {
    u16 relative_angle;
    f32 scaled;

    relative_angle = angle - state->angle;
    scaled = magnitude * (func_8009D8A0(D_80071914) + D_80071918);
    state->x_velocity += scaled * func_8009D510(relative_angle);
    state->z_velocity -= scaled * func_8009D4B0(relative_angle);
    state->y_velocity = 0.0f;
    state->w_velocity = 0.0f;
}
