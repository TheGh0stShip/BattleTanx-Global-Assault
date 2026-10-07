#include "types.h"

extern u8 D_80224EF0[];
extern u8 D_80235EF0[];

s32 func_800A2B74(void* object) {
    register s32 result __asm__("$2") = 0;

    if ((u8*)object >= D_80224EF0) {
        result = (u8*)object < D_80235EF0;
    }
    return result;
}
