#include "types.h"

extern u8* volatile D_80219498;

void func_800B06A8(u8* color, s32 index) {
    u8* destination;
    u8 red;

    index <<= 5;
    destination = D_80219498;
    red = color[1];
    index += 4;
    destination += index;
    destination[0x18] = red;
    destination[0x19] = color[2];
    destination[0x1A] = color[3];
}
