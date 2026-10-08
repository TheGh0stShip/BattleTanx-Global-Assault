#include "types.h"

typedef struct MatrixLimit8007A9EC {
    u8 pad00[0xC];
    u8 *end;
} MatrixLimit8007A9EC;

typedef struct DisplayCommand8007A9EC {
    u32 word0;
    u32 word1;
} DisplayCommand8007A9EC;

typedef struct DisplayContext8007A9EC {
    u8 pad00[0x90];
    MatrixLimit8007A9EC limits[3];
    u16 record_index;
    u8 padC2[6];
    DisplayCommand8007A9EC *commands;
    u8 padCC[4];
    u8 *matrix_cursor;
} DisplayContext8007A9EC;

typedef struct MatrixRecord8007A9EC {
    u8 pad00[0x40];
    void *converted;
} MatrixRecord8007A9EC;

extern DisplayContext8007A9EC *D_80114500;
extern void guMtxF2L(void *source, void *destination);

void func_8007A9EC(MatrixRecord8007A9EC *matrix, u8 mode) {
    DisplayContext8007A9EC *context = D_80114500;
    DisplayCommand8007A9EC *packet;
    MatrixLimit8007A9EC *limit = &context->limits[context->record_index];
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
    packet->word0 = 0xDA380000 | ((mode ^ 1) & 0xFF);
    packet->word1 = (u32)converted - 0x80000000;
}
