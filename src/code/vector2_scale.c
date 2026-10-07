#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} Vec2f;

void func_8009DB0C(Vec2f* vector, f32 scale) {
    vector->x *= scale;
    vector->y *= scale;
}
