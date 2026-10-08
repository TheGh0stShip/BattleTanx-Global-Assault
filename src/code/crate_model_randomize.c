/* ---- 0x800E4800/src/func_800E64F0.c ---- */
#include "types.h"

extern s32 D_803A57A8;
extern s32 D_803A57AC;
extern s32 func_8009D914(void*);

void func_800E64F0(s32* o) {
    if (!(func_8009D914(o) & 1)) {
        o[5] = D_803A57A8;
    } else {
        o[5] = D_803A57AC;
    }
}

