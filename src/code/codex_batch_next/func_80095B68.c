#include "types.h"

s32 func_80095B68(void *object) {
    register u32 flags __asm__("$4");
    register u32 active __asm__("$3");
    register s32 result __asm__("$2") = 0;

    if (object != 0) {
        flags = *(u32 *)((u8 *)object + 0x1E0);
        active = flags & 1;
        __asm__ volatile("" : "=r"(active) : "0"(active));
        if (active != 0) {
            result = flags & 8;
            __asm__ volatile("" : "=r"(result) : "0"(result));
            result = (u32)result < 1;
        }
    }
    return result;
}
