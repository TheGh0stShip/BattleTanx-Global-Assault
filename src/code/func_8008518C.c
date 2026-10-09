#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 scale_x;
    f32 scale_y;
} Transform2;

typedef struct {
    u8 pad0[0x90];
    Transform2 *transform;
    u8 pad94[0xE0];
    f32 fallback_x;
    f32 fallback_y;
    f32 result_x;
    f32 result_y;
} Object2;

void func_8009DA44(f32 *output, Transform2 *transform, u8 *input);

void func_8008518C(Object2 *object, u8 *input) {
    f32 first[2];
    f32 second[2];
    Transform2 *transform = object->transform;
    f32 first_dot;
    f32 second_dot;

    func_8009DA44(first, transform, input + 8);
    first_dot = first[0] * transform->scale_x + first[1] * transform->scale_y;
    func_8009DA44(second, transform, (u8 *)object + 8);
    second_dot = second[0] * transform->scale_x + second[1] * transform->scale_y;
    if (first_dot < second_dot) {
        object->result_x = object->fallback_x;
        object->result_y = object->fallback_y;
    } else {
        object->result_x = transform->x;
        object->result_y = transform->y;
    }
}
