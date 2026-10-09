#include "types.h"

void func_800967F0(void *object) {
    u8 *linked = *(u8 **)((u8 *)object + 0xC);

    if (linked != 0) {
        *(s32 *)(linked + 0x290) = 0;
        *(s32 *)(linked + 0x294) = 0;
        *(s32 *)(linked + 0x298) = 0;
    }
}
