#include "types.h"

s32 func_80088F84(void *unused, void *object) {
    s32 result = 0;
    u8 type;

    if (object != 0) {
        if (*(s32 *)((u8 *)object + 4) == 0x10) {
            if (*(s32 *)object != 0) {
                if (*(s32 *)((u8 *)object + 0x40) == 0) {
                    type = *(u8 *)((u8 *)object + 0x1F);
                    result = type != 1 && type != 3;
                }
            }
        }
    }
    return result;
}
