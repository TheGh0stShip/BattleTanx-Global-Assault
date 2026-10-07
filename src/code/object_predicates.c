#include "types.h"

s32 func_80083FCC(u8* object) {
    s32 result = 0;

    if (object != 0) {
        result = *(s32*)(object + 0xC) != 0;
    }
    return result;
}

s32 func_80083FE4(u8* object) {
    u8* child = *(u8**)(object + 0x20);

    return child[0x3D] == 7;
}
