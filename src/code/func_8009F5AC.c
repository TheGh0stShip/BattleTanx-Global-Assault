#include "types.h"

typedef struct {
    f32 m[4][4];
} Matrix4f;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

extern f32 D_80072650;

void func_8009F5AC(Matrix4f *input, Matrix4f *output, Vec3f *position,
                   Vec3f *reference) {
    Vec3f inverse_translation;
    register f32 *diagonal_base __asm__("$11");
    register f32 *row_start __asm__("$10");
    register f32 *diagonal __asm__("$9");
    register f32 *element __asm__("$7");
    register s32 init_row __asm__("$8");
    register s32 init_column __asm__("$3");

    position->x = input->m[3][0];
    position->z = input->m[3][1];
    position->y = input->m[3][2];

    reference->x = position->x + input->m[2][0];
    reference->z = position->z + input->m[2][1];
    reference->y = position->y + input->m[2][2];

    {
        register f32 identity __asm__("$f2") = D_80072650;

        diagonal_base = &output->m[0][0];
        row_start = &output->m[0][0];
        for (init_row = 0; init_row < 4;
             diagonal_base += 5, init_row++, row_start += 4) {
            diagonal = diagonal_base;
            element = row_start;
            for (init_column = 0; init_column < 4;
                 init_column++, element++) {
                if (init_row != init_column) {
                    *element = 0.0f;
                } else {
                    *diagonal = identity;
                }
            }
        }
    }

    {
        register s32 transform_column __asm__("$9");
        register s32 middle_column __asm__("$12");
        register s32 column_offset __asm__("$11");
        register f32 *middle_start __asm__("$13");
        register f32 *input_element __asm__("$7");
        register f32 *output_element __asm__("$3");
        register f32 *middle_element __asm__("$8");
        register f32 *input_end __asm__("$10");

        transform_column = 0;
        middle_column = 1;
        middle_start = &input->m[1][0];
        for (; transform_column < 3;
             transform_column++, input = (Matrix4f *)((u8 *)input + 16)) {
            column_offset = transform_column * 4;
            input_element = &input->m[0][0];
            output_element = &output->m[0][0];
            middle_element = middle_start;
            input_end = (f32 *)((u8 *)input + 12);
            do {
                if (transform_column == middle_column) {
                    output_element[1] = *middle_element;
                } else {
                    *(f32 *)((u8 *)output_element + column_offset) =
                        -*input_element;
                }
                input_element++;
                output_element += 4;
                middle_element++;
            } while ((s32)input_element < (s32)input_end);
        }
    }

    inverse_translation.x =
        position->x * output->m[0][0] +
        position->z * output->m[1][0] +
        position->y * output->m[2][0];
    inverse_translation.z =
        position->x * output->m[0][1] +
        position->z * output->m[1][1] +
        position->y * output->m[2][1];
    inverse_translation.y =
        position->x * output->m[0][2] +
        position->z * output->m[1][2] +
        position->y * output->m[2][2];

    output->m[3][0] = -inverse_translation.x;
    output->m[3][1] = -inverse_translation.z;
    output->m[3][2] = -inverse_translation.y;
}
