#include "types.h"

extern s32 D_80219208[];
extern u16 D_80219238[][3];

s32 func_80098334(s32 index) {
    return (D_80219238[D_80219208[index]][0] & 0xF) == 0xF;
}
