#include "types.h"

extern u32 D_80117EB8;
extern u8 D_80118EC4[];
extern u8 D_80118ECC[];
extern u8 D_80118ED4[];

s32 func_800C0ADC(void *unused, u8 *state) {
    u8 *entry;
    u32 value;

    D_80117EB8--;
    if (D_80117EB8 == 0) {
        D_80117EB8 = 3;
    }
    value = D_80117EB8;
    entry = state + 0x10;
    if (value == 2) goto set_middle;
    if (value < 3) {
        if (value == 1) goto set_first;
        goto set_middle;
    }
    if (value == 3) goto set_last;
    goto set_middle;
set_first:
    *(u8 **)(state + 0x18) = D_80118EC4;
    goto done;
set_last:
    *(u8 **)(state + 0x18) = D_80118ED4;
    goto done;
set_middle:
    *(u8 **)(entry + 8) = D_80118ECC;
done:
    return 0;
}
