#include "types.h"

extern f32 D_8007260C;
extern f32 D_80072610;
extern f32 D_80072614;
extern f32 D_80072618;
extern f32 D_8007261C;
extern f32 D_80072620;
extern f32 D_80072624;
extern f32 D_80072628;

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
        y = x + y * D_8007260C / D_80072610;
    } else {
        y = y + x * D_80072614 / D_80072618;
    }
    if (y < z) {
        return z + y * D_80072624 / D_80072628;
    }
    return y + z * D_8007261C / D_80072620;
}
