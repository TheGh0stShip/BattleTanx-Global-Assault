#include "types.h"

extern s32 D_8023A064;
extern s32 D_8023A068;

s32 func_800ACEB4(s32 arg0) {
    s32 size = (arg0 + 7) & ~7;
    register s32 available __asm__("$3") = D_8023A064;
    register s32 result __asm__("$2");

    if (available >= size) {
        result = D_8023A068;
        D_8023A064 = available - size;
        D_8023A068 = result + size;
    } else {
        result = 0;
    }
    return result;
}
