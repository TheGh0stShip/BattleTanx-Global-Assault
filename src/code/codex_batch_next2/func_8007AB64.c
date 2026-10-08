#include "types.h"

typedef struct MatrixLimit8007AB64 {
    u8 pad00[0xC];
    u8 *end;
} MatrixLimit8007AB64;

typedef struct DisplayCommand8007AB64 {
    u32 word0;
    u32 word1;
} DisplayCommand8007AB64;

typedef struct DisplayContext8007AB64 {
    u8 pad00[0x90];
    MatrixLimit8007AB64 limits[3];
    u16 record_index;
    u8 padC2[6];
    DisplayCommand8007AB64 *commands;
    u8 padCC[4];
    u8 *matrix_cursor;
} DisplayContext8007AB64;

typedef struct MatrixRecord8007AB64 {
    u8 pad00[0x40];
    void *converted;
} MatrixRecord8007AB64;

extern DisplayContext8007AB64 *D_80114500;
extern void guMtxF2L(void *source, void *destination);

void func_8007AB64(MatrixRecord8007AB64 *matrix, u16 parameter) {
    DisplayContext8007AB64 *context = D_80114500;
    MatrixLimit8007AB64 *limit = &context->limits[context->record_index];
    register DisplayCommand8007AB64 *packet __asm__("$3");
    void *converted = matrix->converted;

    if (converted == 0) {
        if (context->matrix_cursor >= limit->end + 0xC000) {
            return;
        }
        guMtxF2L(matrix, context->matrix_cursor);
        converted = context->matrix_cursor;
        matrix->converted = converted;
        context->matrix_cursor += 0x40;
    }

    packet = context->commands;
    context->commands++;
    packet->word0 = 0xDB0E0000;
    packet->word1 = parameter;

    packet = context->commands;
    context->commands++;
    packet->word0 = 0xDA380007;
    packet->word1 = (u32)converted - 0x80000000;
}
