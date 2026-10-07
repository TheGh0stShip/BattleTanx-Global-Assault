#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} Vec2f;

extern f32 func_8009D4B0(u16 angle);
extern f32 func_8009D510(u16 angle);

void func_8009FF1C(Vec2f* input, Vec2f* output, u16 angle) {
    output->x = func_8009D510(angle) * input->x +
                func_8009D4B0(angle) * input->y;
    output->y = func_8009D510(angle) * input->y -
                func_8009D4B0(angle) * input->x;
}
