/* RODATA_VRAM 0x80074120 */
#include "types.h"

extern f32 D_803A5948;
extern s16 D_803A65D8;
extern s16 D_803A6580;
extern s16 D_803A663C;

s32 func_800CFA84(void) {
    D_803A65D8 -= (u32)(D_803A5948 * 3.0f);
    if (D_803A65D8 <= D_803A663C - D_803A6580) {
        D_803A65D8 = D_803A663C - D_803A6580;
        return 1;
    }
    return 0;
}

s32 func_800CFB38(void) {
    D_803A65D8 += (u32)(D_803A5948 * 3.0f);
    if (D_803A65D8 >= D_803A663C) {
        D_803A65D8 = D_803A663C;
        return 1;
    }
    return 0;
}
