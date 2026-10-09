#include "types.h"

extern f32 sqrtf(f32 value);
extern s32 func_8009D5B4(f32 value);

u16 func_8009DFAC(f32 *vector) {
    f32 x;
    f32 y;
    f32 length;

    x = vector[0];
    y = vector[1];
    length = sqrtf(x * x + y * y);
    if (length > 0.0f) {
        if (x < 0.0f) {
            return (u16)-func_8009D5B4(y / length);
        }
        return (u16)func_8009D5B4(y / length);
    }
    return 0;
}
