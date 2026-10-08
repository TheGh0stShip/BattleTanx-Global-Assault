#include "types.h"

extern u32 D_802194A0;
extern u8 D_802194A6;
extern u8 D_80219582[];

static inline s32 func_800E8090_mode(void) {
    switch (D_802194A0) {
    case 1:
    case 3:
    case 11:
        return 1;
    }
    return 0;
}

s32 func_800E8090(s32 i) {
    if (func_800E8090_mode()) {
        s32 a = D_80219582[i]; return a < (s32)D_802194A6;
    }
    return 0;
}
