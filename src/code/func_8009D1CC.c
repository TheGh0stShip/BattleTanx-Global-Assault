/* SPAN 0x8009D268 */
/* RODATA_VRAM 0x80072560 */
#include "types.h"

extern s32 D_80117EBC;
extern u8 D_802195B8;
extern f32 D_8007255C;

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
