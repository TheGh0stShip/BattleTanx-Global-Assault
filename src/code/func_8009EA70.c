#include "types.h"


f32 func_8009EA70(f32 *a, f32 *b) {
    f32 x = a[0] - b[0];
    f32 z;
    f32 y;

    if (!(0.0f < x)) {
        x = -x;
    }
    z = a[2] - b[2];
    if (!(0.0f < z)) {
        z = -z;
    }
    y = a[1] - b[1];
    if (!(0.0f < y)) {
        y = -y;
    }
    if (y < x) {
        y = x + y * 3.0f / 8.0f;
    } else {
        y = y + x * 3.0f / 8.0f;
    }
    if (y < z) {
        return z + y * 3.0f / 8.0f;
    }
    return y + z * 3.0f / 8.0f;
}
