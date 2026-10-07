#include "types.h"

s16 func_8009D850(u32 first, u32 second, u16* difference_out) {
    s32 direction;
    u32 difference;
    u32 first16 = first & 0xFFFF;
    u32 second16 = second & 0xFFFF;

    if (second16 < first16) {
        difference = first - second;
        direction = -1;
    } else {
        difference = second - first;
        direction = 1;
    }
    if ((u16)difference > 0x8000) {
        difference = ~difference;
        direction = -direction;
    }
    *difference_out = difference;
    return direction;
}
