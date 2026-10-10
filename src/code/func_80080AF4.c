#include "types.h"

u8 *Steps_InitStep_Free(u16);
u16 Steps_CopyObstacleRef(u8 *);

s32 func_80080AF4(void *arg0) {
    register s32 result __asm__("$2");
    u8 *node;
    register s32 kind __asm__("$16");

    node = Steps_InitStep_Free(*(u16 *)((u8 *)arg0 + 0xF4));
    result = 1;
    if (node == 0) {
        goto done;
    }
    __asm__ volatile("addiu %0,$0,2" : "=r"(kind));
loop:
    result = *node;
    if (result == kind) {
        goto matched;
    }
    result = 0;
    __asm__ volatile("" : : "r"(result));
    node = Steps_InitStep_Free(Steps_CopyObstacleRef(node) & 0xFFFF);
    result = 1;
    if (node != 0) {
        goto loop;
    }
    goto done;
matched:
    result = 0;
done:
    return result;
}
