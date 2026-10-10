#include "types.h"

/* RODATA_VRAM 0x80071F90 */

typedef struct {
    f32 m[4][4];
} Matrix4f;

extern Matrix4f D_8007192C;
void func_8009F538(Matrix4f *first, Matrix4f *second, Matrix4f *output);

void func_8009277C(Matrix4f *matrix, f32 arg1, f32 arg2, f32 arg3) {
    Matrix4f scale = D_8007192C;
    Matrix4f result;
    f32 var_f4;
    f32 var_f6;
    f32 var_f8;
    f32 one;
    f32 delta;
    Matrix4f *dst;

    if (0.66666f < arg3) {
        var_f6 = 0.1f;
        var_f8 = var_f6;
        var_f4 = ((1.0f - ((arg3 - 0.666666f) / 0.3333333f)) * 0.9f) + var_f6;
    } else if (0.333333f < arg3) {
        var_f4 = 1.0f;
        var_f6 = 0.1f;
        var_f8 = ((var_f4 - ((arg3 - 0.33333334f) / 0.3333333f)) * 0.9f) + var_f6;
    } else {
        var_f4 = 1.0f;
        var_f8 = var_f4;
        var_f6 = ((var_f4 - (arg3 / 0.3333333f)) * 0.9f) + 0.1f;
    }

    one = 1.0f;
    delta = one - var_f6;
    scale.m[0][0] = var_f8;
    scale.m[1][1] = var_f6;
    scale.m[2][2] = var_f4;
    scale.m[0][1] = delta * (arg2 / arg1) * var_f8;
    func_8009F538(&scale, matrix, &result);

    result.m[3][0] = matrix->m[3][0];
    dst = matrix;
    result.m[3][1] = matrix->m[3][1] + ((arg2 * delta) / 2.0f);
    result.m[0][3] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][3] = 0.0f;
    result.m[3][3] = one;
    result.m[3][2] = dst->m[3][2];
    *dst = result;
}
