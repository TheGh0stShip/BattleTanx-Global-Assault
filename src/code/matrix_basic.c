#include "types.h"

typedef struct {
    f32 m[4][4];
} Matrix4f;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

extern f32 D_80072640;

void func_8009EEE0(Matrix4f* matrix) {
    s32 row;
    s32 column;

    for (row = 0; row < 4; row++) {
        for (column = 0; column < 4; column++) {
            if (row != column) {
                matrix->m[row][column] = 0.0f;
            } else {
                matrix->m[row][column] = D_80072640;
            }
        }
    }
}

void func_8009EF30(Matrix4f* matrix, Vec3f* vector) {
    matrix->m[3][0] = vector->x;
    matrix->m[3][1] = vector->z;
    matrix->m[3][2] = vector->y;
}
