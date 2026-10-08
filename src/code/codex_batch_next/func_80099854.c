#include "types.h"

extern u8 D_80219463[];
extern s32 D_80114860;
extern u8 D_80219258[];
extern void *D_80219490;

void func_80099854(void) {
    s32 offset;

    for (offset = 12; offset >= 0; offset -= 4) {
        D_80219463[offset] = 0;
    }
    D_80114860 = (D_80114860 + 1) & 1;
    D_80219490 = D_80219258 + (D_80114860 << 8);
}
