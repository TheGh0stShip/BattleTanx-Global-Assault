#include "types.h"

void func_800F3B80(u8 *start, u8 *end, u8 *result, s32 step,
                   s32 denominator) {
    result[0] = start[0] + ((end[0] - start[0]) * step) / denominator;
    result[1] = start[1] + ((end[1] - start[1]) * step) / denominator;
    result[2] = start[2] + ((end[2] - start[2]) * step) / denominator;
    result[3] = start[3] + ((end[3] - start[3]) * step) / denominator;
}

u8 func_800F3CC4(s32 value, s32 denominator) {
    return ((value + 1) * 255) / denominator;
}
