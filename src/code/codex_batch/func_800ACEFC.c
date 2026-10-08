#include "types.h"

extern s32 D_8023A064;
extern s32 D_8023A068;

s32 func_800ACEFC(s32 arg0) {
    register s32 available __asm__("$3") = D_8023A064;
    register s32 result __asm__("$2");

    if (available >= arg0) {
        result = D_8023A068;
    } else {
        result = 0;
    }
    return result;
}
