#include "types.h"

u16 func_80089D98(u32 value) {
    u16 shift = 0;

    while ((value & 1) == 0) {
        value >>= 1;
        shift++;
    }
    return shift;
}
