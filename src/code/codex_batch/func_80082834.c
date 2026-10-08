#include "types.h"
#include "m2c_macros.h"

extern s32 func_8007E784();

s32 func_80082834(void *arg0) {
    s32 result;

    if (M2C_FIELD(arg0, u16 *, 0x130) == M2C_FIELD(arg0, u16 *, 0xF4)) {
        result = func_8007E784();
        if (result != 0) {
            return result + 4;
        }
    }
    return 0;
}
