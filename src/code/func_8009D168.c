/* SPAN 0x8009D268 */
/* RODATA_VRAM 0x80072558 */
#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK func_80097D14(s32, M2C_UNK);                /* extern */
f32 func_8009D8A0(f32);                             /* extern */
extern s32 D_80117EBC;
extern u8 D_802195B8;
static f32 D_80072558 __attribute__((section(".rodata"))) = 19.0f;
static f32 D_8007255C __attribute__((section(".rodata"))) = 1.0f;

void func_8009D168(void) {
    s32 temp_f2;
    s32 var_a0;

    temp_f2 = (s32) func_8009D8A0(D_80072558);
    var_a0 = temp_f2 + 6;
    if ((((var_a0 == 0x18) | (var_a0 == 9)) != 0) || (var_a0 == 0x16)) {
        var_a0 = temp_f2 + 7;
    }
    func_80097D14(var_a0, 0xA);
}

f32 func_8009D1CC(void) {
    if (D_80117EBC != 1) {
        return D_8007255C;
    }

    switch (D_802195B8) {
        case 0:
        case 1:
            return D_8007255C;
        case 2:
            return 1.1f;
        case 3:
            return 1.2f;
        case 4:
            return 1.3f;
        case 5:
            return 1.4f;
        default:
            return 1.5f;
    }
}
