#include "types.h"

extern void *D_80219498;

void func_800B03F4(u8 *command, float *values, u16 half, u8 byte) {
    u8 row = command[1];
    u8 column = command[2];
    u8 *out = (u8 *)D_80219498 + 0xC8 + row * 60 + column * 12;

    *(float *)out = values[0];
    *(float *)(out + 4) = values[1];
    *(u16 *)(out + 8) = half;
    out[0xA] = byte;
}
