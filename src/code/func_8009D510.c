#include "types.h"

extern f32 D_80114870[];
extern f32 D_80072594;

f32 func_8009D510(u16 value) {
    register u32 index __asm__("$2") = (u8)((value >> 8) + 0x40);
    register u32 next __asm__("$3") = index + 1;
    f32 first = D_80114870[index];
    f32 delta = D_80114870[next] - first;

    return first + delta * (value & 0xFF) * D_80072594;
}
