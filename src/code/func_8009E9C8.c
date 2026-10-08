#include "types.h"

f32 sqrtf(f32 value);
s32 func_8009D5B4(f32 value);

u16 func_8009E9C8(f32 *from, f32 *to) {
    f32 dx = to[0] - from[0];
    f32 dy = to[1] - from[1];
    f32 length = sqrtf(dx * dx + dy * dy);
    s32 result;

    if (length == 0.0f) {
        goto zero;
    }
    if (dx < 0.0f) {
        result = -func_8009D5B4(dy / length);
    } else {
        result = func_8009D5B4(dy / length);
    }
    return (u16)result;
zero:
    return 0;
}
