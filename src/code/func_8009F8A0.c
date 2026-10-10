#include "types.h"

typedef struct {
    f32 m[4][4];
} Matrix4f;

extern f32 func_8009D4B0(u16 angle);
extern f32 func_8009D510(u16 angle);

void func_8009F8A0(Matrix4f *input, Matrix4f *output, u16 angle) {
    Matrix4f rotation;
    f32 sine;
    f32 cosine;
    s32 row;
    s32 column;

    {
        f32 one;
        f32 *row_base;
        f32 *diagonal;
        f32 *cell;
        f32 *diagonal_cell;
        s32 init_row;
        s32 init_column;

        init_row = 0;
        one = 1.0f;
        row_base = &rotation.m[0][0];
        diagonal = row_base;
        while (init_row < 4) {
            init_column = 0;
            diagonal_cell = diagonal;
            cell = row_base;
            while (init_column < 4) {
                if (init_row != init_column) {
                    *cell = 0.0f;
                } else {
                    *diagonal_cell = one;
                }
                init_column++;
                cell++;
            }
            diagonal += 5;
            init_row++;
            row_base += 4;
        }
    }

    sine = func_8009D510(angle);
    cosine = func_8009D4B0(angle);
    rotation.m[0][1] = cosine;
    rotation.m[0][0] = sine;
    rotation.m[1][1] = sine;
    rotation.m[1][0] = -cosine;

    for (column = 0; column < 3; column++) {
        for (row = 0; row < 3; row++) {
            output->m[row][column] =
                input->m[row][0] * rotation.m[0][column] +
                input->m[row][1] * rotation.m[1][column] +
                input->m[row][2] * rotation.m[2][column];
        }
    }

    output->m[0][3] = input->m[0][3];
    output->m[1][3] = input->m[1][3];
    output->m[2][3] = input->m[2][3];
    output->m[3][3] = input->m[3][3];
    output->m[3][2] = input->m[3][2];
    output->m[3][1] = input->m[3][1];
    output->m[3][0] = input->m[3][0];
}

void func_8009FA04(Matrix4f *input, Matrix4f *output, u16 angle) {
    Matrix4f rotation;
    f32 sine;
    f32 cosine;
    s32 row;
    s32 column;

    {
        f32 one;
        f32 *row_base;
        f32 *diagonal;
        f32 *cell;
        f32 *diagonal_cell;
        s32 init_row;
        s32 init_column;

        init_row = 0;
        one = 1.0f;
        row_base = &rotation.m[0][0];
        diagonal = row_base;
        while (init_row < 4) {
            init_column = 0;
            diagonal_cell = diagonal;
            cell = row_base;
            while (init_column < 4) {
                if (init_row != init_column) {
                    *cell = 0.0f;
                } else {
                    *diagonal_cell = one;
                }
                init_column++;
                cell++;
            }
            diagonal += 5;
            init_row++;
            row_base += 4;
        }
    }

    sine = func_8009D510(angle);
    cosine = func_8009D4B0(angle);
    rotation.m[2][0] = cosine;
    rotation.m[0][0] = sine;
    rotation.m[2][2] = sine;
    rotation.m[0][2] = -cosine;

    for (column = 0; column < 3; column++) {
        for (row = 0; row < 3; row++) {
            output->m[row][column] =
                input->m[row][0] * rotation.m[0][column] +
                input->m[row][1] * rotation.m[1][column] +
                input->m[row][2] * rotation.m[2][column];
        }
    }

    output->m[0][3] = input->m[0][3];
    output->m[1][3] = input->m[1][3];
    output->m[2][3] = input->m[2][3];
    output->m[3][3] = input->m[3][3];
    output->m[3][2] = input->m[3][2];
    output->m[3][1] = input->m[3][1];
    output->m[3][0] = input->m[3][0];
}

void func_8009FB68(Matrix4f *input, Matrix4f *output, u16 angle) {
    Matrix4f rotation;
    f32 sine;
    f32 cosine;
    s32 row;
    s32 column;

    {
        f32 one;
        f32 *row_base;
        f32 *diagonal;
        f32 *cell;
        f32 *diagonal_cell;
        s32 init_row;
        s32 init_column;

        init_row = 0;
        one = 1.0f;
        row_base = &rotation.m[0][0];
        diagonal = row_base;
        while (init_row < 4) {
            init_column = 0;
            diagonal_cell = diagonal;
            cell = row_base;
            while (init_column < 4) {
                if (init_row != init_column) {
                    *cell = 0.0f;
                } else {
                    *diagonal_cell = one;
                }
                init_column++;
                cell++;
            }
            diagonal += 5;
            init_row++;
            row_base += 4;
        }
    }

    sine = func_8009D510(angle);
    cosine = func_8009D4B0(angle);
    rotation.m[1][2] = cosine;
    rotation.m[1][1] = sine;
    rotation.m[2][2] = sine;
    rotation.m[2][1] = -cosine;

    for (column = 0; column < 3; column++) {
        for (row = 0; row < 3; row++) {
            output->m[row][column] =
                input->m[row][0] * rotation.m[0][column] +
                input->m[row][1] * rotation.m[1][column] +
                input->m[row][2] * rotation.m[2][column];
        }
    }

    output->m[0][3] = input->m[0][3];
    output->m[1][3] = input->m[1][3];
    output->m[2][3] = input->m[2][3];
    output->m[3][3] = input->m[3][3];
    output->m[3][2] = input->m[3][2];
    output->m[3][1] = input->m[3][1];
    output->m[3][0] = input->m[3][0];
}
