#include "types.h"

typedef struct {
    f32 m[3][4];
} Matrix3f;

extern f32 func_8009D4B0(u16 angle);
extern f32 func_8009D510(u16 angle);

void func_8009F090(Matrix3f *matrix, u16 x, u16 y, u16 z) {
    f32 cos_z;
    f32 sin_z;
    f32 cos_y;
    f32 sin_y;
    f32 cos_x;
    f32 sin_x;
    f32 cos_z_cos_y;
    f32 sin_z_cos_y;

    cos_z = func_8009D4B0(z);
    sin_z = func_8009D510(z);
    cos_y = func_8009D4B0(y);
    sin_y = func_8009D510(y);
    cos_x = func_8009D4B0(x);
    sin_x = func_8009D510(x);

    matrix->m[0][0] = sin_y * sin_x;
    matrix->m[0][1] = sin_y * cos_x;
    matrix->m[0][2] = -cos_y;

    cos_z_cos_y = cos_z * cos_y;
    matrix->m[1][0] = cos_z_cos_y * sin_x - sin_z * cos_x;
    matrix->m[1][1] = cos_z_cos_y * cos_x + sin_z * sin_x;
    matrix->m[1][2] = cos_z * sin_y;

    sin_z_cos_y = sin_z * cos_y;
    matrix->m[2][0] = sin_z_cos_y * sin_x + cos_z * cos_x;
    matrix->m[2][1] = sin_z_cos_y * cos_x - cos_z * sin_x;
    matrix->m[2][2] = sin_z * sin_y;
}
