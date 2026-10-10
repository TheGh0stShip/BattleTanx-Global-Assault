#include "types.h"

extern f32 D_80114870[];

f32 func_8009D4B0(u16 value) {
    register u32 index asm("$2") = ((u32)value & 0xFFFF) >> 8;
    register u32 next asm("$3") = index + 1;
    f32 start = D_80114870[index];
    f32 result = (D_80114870[next] - start) * (f32)(value & 0xFF);

    result *= 0.00390625f;
    return start + result;
}
