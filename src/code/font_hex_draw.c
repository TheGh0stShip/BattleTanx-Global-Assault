#include "types.h"

s16 func_80096F48(void *context, u8 *text, u16 font, s16 x, s16 y,
                   f32 scale_x, f32 scale_y);
s16 func_800973E0(u8 *text, u16 font, f32 scale);

s16 func_80096E14(void *context, s32 value, u16 font, s16 x, s16 y,
                   f32 scale_x, f32 scale_y, u16 align) {
    u8 text[20];
    u8 *cursor;
    s16 shift;
    s16 digit;
    s16 offset;

    shift = 28;
    cursor = text;
    do {
        digit = (value >> shift) & 0xF;
        *cursor = digit < 10 ? digit | '0' : digit + 'A' - 10;
        shift -= 4;
        cursor++;
    } while (shift >= 0);
    *cursor = 0;

    switch (align) {
    case 0:
        offset = 0;
        break;
    case 1:
        offset = func_800973E0(text, font, scale_x);
        break;
    case 2:
        offset = func_800973E0(text, font, scale_x) >> 1;
        break;
    default:
        offset = 0;
        break;
    }
    return func_80096F48(context, text, font, x - offset, y, scale_x, scale_y);
}
