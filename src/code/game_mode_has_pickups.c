/* ---- 0x800E4800/b/src/func_800E804C.c ---- */
#include "types.h"

extern u32 D_802194A0;

s32 func_800E804C(void) {
    switch (D_802194A0) {
        case 1:
        case 3:
        case 11:
            return 1;
    }
    return 0;
}

