#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} Vec2f;

extern s32 func_8009DB2C(Vec2f* vector, f32 magnitude);

void func_8009E044(Vec2f* first, Vec2f* second) {
    f32 x = first->x + second->x;
    f32 y = first->y + second->y;

    first->x = x;
    first->y = y;
}

s32 func_8009E068(Vec2f* position, Vec2f* target, f32 maximum_step) {
    Vec2f delta;

    delta.x = target->x - position->x;
    delta.y = target->y - position->y;
    if (func_8009DB2C(&delta, maximum_step) != 0) {
        position->x += delta.x;
        position->y += delta.y;
        return 1;
    }
    return 0;
}
