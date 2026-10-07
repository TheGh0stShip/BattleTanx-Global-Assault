#include "types.h"

s32 func_8009D72C(u32 base, u32 limit, u32 value) {
    limit &= 0xFFFF;
    limit += 0x10000;
    base &= 0xFFFF;
    limit -= base;
    value &= 0xFFFF;
    value += 0x10000;
    value -= base;
    limit &= 0xFFFF;
    value &= 0xFFFF;

    return value < limit;
}
