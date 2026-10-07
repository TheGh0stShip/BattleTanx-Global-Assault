#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} Vec2;

void func_800B5F30(u8* object, Vec2* first, Vec2* second, f32 amount) {
    f32 cross = (first->x * second->y) - (first->y * second->x);

    if (cross < 0.0f) {
        amount = -amount;
    }
    *(f32*)(object + 0x1C) += amount;
}
