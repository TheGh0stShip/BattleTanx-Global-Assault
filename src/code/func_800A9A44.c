#include "types.h"

void func_800A9A44(void *p, s32 index) {
    s32 freeIndex;
    u8 *bytes = p;

    if (index == 11) {
        return;
    }
    freeIndex = 2;
    while (*(bytes + freeIndex + 0x1FE) != 11) {
        freeIndex++;
    }
    *(bytes + freeIndex + 0x1FE) = bytes[0x209];
    bytes[0x209] = *(bytes + index + 0x1FE);
    *(bytes + index + 0x1FE) = 11;
}
