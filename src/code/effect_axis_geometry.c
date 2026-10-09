#include "types.h"

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct EffectAxisGeometry {
    s8 direction;
    u8 reserved01[3];
    Vec3f vertices[6];
    u8 indices[8];
    u32 vertex_count;
    u32 axis_count;
    Vec3f *vertex_data;
    u8 *index_data;
} EffectAxisGeometry;

EffectAxisGeometry gEffectAxisGeometry = {
    1,
    { 0, 0, 0 },
    {
        { -10.0f, 0.0f, 0.0f },
        {  10.0f, 0.0f, 0.0f },
        { 0.0f, -10.0f, 0.0f },
        { 0.0f,  10.0f, 0.0f },
        { 0.0f, 0.0f, -10.0f },
        { 0.0f, 0.0f,  10.0f },
    },
    { 0, 1, 2, 3, 4, 5, 0, 0 },
    6,
    3,
    gEffectAxisGeometry.vertices,
    gEffectAxisGeometry.indices,
};
