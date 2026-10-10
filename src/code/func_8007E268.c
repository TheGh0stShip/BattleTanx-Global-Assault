#include "types.h"

void *Steps_InitStep_Free(u16);

void *func_8007E268(void *arg0, s32 arg1) {
    register void *result __asm__("$16") = arg0;
    register s32 target_type __asm__("$17");
    register void *object __asm__("$3");

    object = Steps_InitStep_Free(arg1 & 0xFFFF);
    result += 0x110;
    target_type = 3;
check:
    if (object == 0) {
        goto done;
    }
    if (*(u8 *)object == target_type) {
        result = object + 4;
        goto done;
    }
    object = Steps_InitStep_Free(*(u16 *)(object + 2));
    goto check;
done:
    return result;
}
