#include "types.h"

typedef struct {
    f32 m[4][4];
} Matrix4f;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

void func_8009F1F4(Matrix4f* matrix, Vec3f* input, Vec3f* output) {
    output->x = input->x * matrix->m[0][0] +
                input->z * matrix->m[1][0] +
                input->y * matrix->m[2][0];
    output->z = input->x * matrix->m[0][1] +
                input->z * matrix->m[1][1] +
                input->y * matrix->m[2][1];
    output->y = input->x * matrix->m[0][2] +
                input->z * matrix->m[1][2] +
                input->y * matrix->m[2][2];
}

void func_8009F288(Matrix4f* matrix, Vec3f* input, Vec3f* output) {
    output->x = input->x * matrix->m[0][0] +
                input->z * matrix->m[1][0] +
                input->y * matrix->m[2][0] + matrix->m[3][0];
    output->z = input->x * matrix->m[0][1] +
                input->z * matrix->m[1][1] +
                input->y * matrix->m[2][1] + matrix->m[3][1];
    output->y = input->x * matrix->m[0][2] +
                input->z * matrix->m[1][2] +
                input->y * matrix->m[2][2] + matrix->m[3][2];
}
