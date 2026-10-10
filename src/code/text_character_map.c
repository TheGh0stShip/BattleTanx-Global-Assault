#include "types.h"

extern u8 D_80114814[];

static inline s32 text_character_index(u8 value) {
    return value;
}

void func_80099160(u8 *dst, u8 *src, s32 length) {
    s32 value = *src;
    s32 index = 0;
    u8 output;

    goto test;
body:
    output = 0x20;
    index++;
    if ((u32)(value - 0x42) < 0x54) {
        output = 0x2A;
    } else if ((u32)(value - 0x10) < 0x33) {
        output = D_80114814[text_character_index(value)];
    }
    *dst = output;
    value = *src;
    dst++;
test:
    if ((index < length) && (value != 0)) {
        src++;
        goto body;
    }
    *dst = 0;
}
