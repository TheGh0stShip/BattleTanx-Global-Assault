#include "types.h"

void *Steps_InitStep_Free(u16, void *);

void *func_8007E210(void *arg0, void *arg1) {
    register void *base __asm__("$16") = arg0;
    register s32 target_type __asm__("$17") = 3;

loop:
    if (arg1 == 0) {
        return base + 0x110;
    }
    if (*(u8 *)arg1 == target_type) {
        return arg1 + 4;
    }
    arg1 = Steps_InitStep_Free(*(u16 *)(arg1 + 2), arg1);
    goto loop;
}
