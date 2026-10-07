#include "types.h"

void func_800A9054(u8* object, s32 bit) {
    object[0xC] &= ~(1 << bit);
    if (object[0xC] == 0) {
        object[0xC] = 0x1F;
    }
}
