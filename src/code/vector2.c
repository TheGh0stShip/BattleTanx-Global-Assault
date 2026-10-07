#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} Vec2f;

extern u32 D_80114C80;
extern f32 func_8009D4B0(u16 angle);
extern f32 func_8009D510(u16 angle);

void func_8009DA34(u32 seed) {
    D_80114C80 = seed;
}

void func_8009DA44(Vec2f* output, Vec2f* first, Vec2f* second) {
    f32 x = second->x - first->x;
    f32 y = second->y - first->y;

    output->x = x;
    output->y = y;
}

void func_8009DA68(Vec2f* output, u16 angle) {
    output->x = func_8009D4B0(angle);
    output->y = func_8009D510(angle);
}
