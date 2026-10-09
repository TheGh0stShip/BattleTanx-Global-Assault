#include "types.h"

extern f64 D_80072470;
extern f32 D_80072478;
extern f32 D_8007247C;

f32 func_80098454(u8 *data, u32 *masks, u8 index) {
    u32 tag = masks[index];
    s32 value;
    s32 magnitude;
    u32 flags;
    f32 result;
    f64 threshold;

    if (tag == 0x50000000) {
        goto read_second;
    }
    if (tag > 0x50000000) {
        if (tag != 0xA0000000) {
            goto read_flags;
        }
        value = (s8)data[0];
        goto check_magnitude;
    }
    if (tag == 0) {
        goto zero;
    }
    goto read_flags;

check_magnitude:
    {
    register f32 output __asm__("$f0");

    threshold = D_80072470;
    magnitude = value;
    if (magnitude < 0) {
        magnitude = -magnitude;
    }
    output = 0.0f;
    if (threshold < magnitude) {
        output = value;
    }
    return output;
    }

read_second:
    value = (s8)data[1];
    goto check_magnitude;

zero:
    return 0.0f;

read_flags:
    flags = *(u32 *)(data + 4);
    result = 0.0f;
    if (flags & tag) {
        result = D_80072478;
    }
    if (flags & masks[index + 1]) {
        result -= D_8007247C;
    }
    return result;
}
