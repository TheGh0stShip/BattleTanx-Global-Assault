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
    u8 pad94[0xE8];
    f32 result_x;
    f32 result_y;
} Object2;

void func_8009DA44(f32 *output, Transform2 *transform, u8 *input);
void func_8009DB0C(f32 *vector, f32 scale);
void func_8009E044(f32 *vector, Transform2 *transform);

void func_80085250(Object2 *object, u8 *input) {
    f32 first[2];
    f32 second[2];
    Transform2 *transform = object->transform;
    f32 dot;

    func_8009DA44(first, transform, input + 8);
    dot = first[0] * transform->scale_x + first[1] * transform->scale_y;
    func_8009DA44(second, transform, (u8 *)object + 8);
    object->result_x = transform->scale_x;
    object->result_y = transform->scale_y;
    func_8009DB0C(&object->result_x, dot);
    func_8009E044(&object->result_x, transform);
}
