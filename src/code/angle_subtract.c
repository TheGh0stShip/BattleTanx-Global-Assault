#include "types.h"

u16 func_8009D6DC(u32 first, u32 second) {
    s32 result = first & 0xFFFF;

    result += 0x10000;
    second &= 0xFFFF;
    result -= second;
    return result & 0xFFFF;
}
