#include "types.h"

void *func_800A1A28(void *, s32);

void *func_80083F74(void *owner) {
    register void *owner_reg __asm__("$16") = owner;
    register void *item __asm__("$4");
    register s32 type __asm__("$5");

    item = 0;
    type = 0x14;
    goto advance;
check:
    if (*(u8 *)((u8 *)item + 0x1D) == *(u8 *)((u8 *)owner_reg + 0x95)) {
        goto done;
    }
    type = 0x14;
advance:
    item = func_800A1A28(item, type);
    if (item != 0) {
        goto check;
    }
done:
    return item;
}
