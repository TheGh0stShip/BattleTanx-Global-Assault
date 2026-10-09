#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

extern f32 D_8007264C;

void func_8009F334(f32 *matrix, Vec3f *input, Vec3f *output) {
    f32 scale;
    register f32 output_x __asm__("$f2");
    register f32 output_z __asm__("$f0");

    output->x = input->x * matrix[0] + input->z * matrix[4] +
                input->y * matrix[8] + matrix[12];
    output->z = input->x * matrix[1] + input->z * matrix[5] +
                input->y * matrix[9] + matrix[13];
    output->y = input->x * matrix[2] + input->z * matrix[6] +
                input->y * matrix[10] + matrix[14];

    scale = D_8007264C /
            (input->x * matrix[3] + input->z * matrix[7] +
             input->y * matrix[11] + matrix[15]);
    output_x = output->x * scale;
    output_z = output->z * scale;
    output->x = output_x;
    output->z = output_z;
    __asm__ volatile("" : : : "memory");
    output->y *= scale;
}
