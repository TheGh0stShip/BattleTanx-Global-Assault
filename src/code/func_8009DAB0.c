#include "types.h"

extern f32 func_8009D4B0(u16 value);
extern f32 func_8009D510(u16 value);

void func_8009DAB0(f32 *out, f32 scale, u16 value) {
    out[0] = scale * func_8009D4B0(value);
    out[1] = scale * func_8009D510(value);
}
