#include "types.h"

typedef struct {
    u8 pad_00[0x8];
    f32 axis0_x;
    f32 axis0_y;
    f32 axis1_x;
    f32 axis1_y;
    f32 offset_y;
    f32 offset_x;
    u8 pad_20[4];
    u8 active;
    u8 kind;
} Region800AA5D0;

s32 func_800AA5D0(f32 x, f32 y, u8 kind, Region800AA5D0 *region) {
    f32 distance;
    register f32 extent __asm__("$f0");

    if (region->kind != kind) {
        return 0;
    }
    if (region->active != 0) {
        return 1;
    }

    y += region->offset_x;
    x += region->offset_y;
    distance = region->axis0_x * y + region->axis0_y * x;
    if (distance <= 0.0f) {
        return 0;
    }
    extent = region->axis1_x * x - region->axis1_y * y;
    if (!(0.0f < extent)) {
        extent = -extent;
    }
    if (distance <= extent) {
        return 0;
    }
    return 1;
}
