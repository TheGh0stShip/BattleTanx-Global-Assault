#include "types.h"

typedef struct {
    f32 m[4][4];
} Matrix4f;

void func_8009F768(Matrix4f* source, Matrix4f* output) {
    s32 column;
    s32 row;

    for (column = 0; column < 3; column++) {
        for (row = 0; row < 3; row++) {
            output->m[row][column] = source->m[column][row];
        }
    }
}

void func_8009F7B4(Matrix4f* source, Matrix4f* output) {
    s32 column;
    s32 row;

    for (column = 0; column < 3; column++) {
        for (row = 0; row < 3; row++) {
            output->m[row][column] = source->m[column][row];
        }
    }
    output->m[3][0] = -source->m[3][0];
    output->m[3][1] = -source->m[3][1];
    output->m[3][2] = -source->m[3][2];
}

void func_8009F824(Matrix4f* matrix, f32 x, f32 y, f32 z) {
    f32 xx = matrix->m[0][0] * x;
    f32 yx = matrix->m[1][0] * x;
    f32 zx = matrix->m[2][0] * x;
    f32 xy = matrix->m[0][1] * y;
    f32 yy = matrix->m[1][1] * y;
    f32 zy = matrix->m[2][1] * y;
    f32 xz = matrix->m[0][2] * z;
    f32 yz = matrix->m[1][2] * z;
    f32 zz = matrix->m[2][2] * z;

    matrix->m[0][0] = xx;
    matrix->m[1][0] = yx;
    matrix->m[2][0] = zx;
    matrix->m[0][1] = xy;
    matrix->m[1][1] = yy;
    matrix->m[2][1] = zy;
    matrix->m[0][2] = xz;
    matrix->m[1][2] = yz;
    matrix->m[2][2] = zz;
}
