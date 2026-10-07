#include "types.h"

extern u32 D_802194A0;

s32 func_8009D144(void) {
    register u32 value __asm__("$4") = D_802194A0;
    register s32 result __asm__("$2") = 0;

    if (value < 15) {
        result = !(value < 7);
    }
    return result;
}
