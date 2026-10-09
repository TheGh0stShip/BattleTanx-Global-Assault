#include "types.h"

extern s32 D_80114804;
extern s32 D_80216FB0[];

s32 func_80099028(void) {
    volatile s32 stack_pad[2];
    register s32 i __asm__("$4");
    register s32 result __asm__("$5");
    register s32 count __asm__("$6");
    register s32 limit __asm__("$2");
    register s32 sentinel __asm__("$7");
    register s32 *entry __asm__("$3");

    i = 0;
    result = 0;
    limit = D_80114804;
    if (limit > 0) {
        sentinel = -1;
        count = limit;
        entry = D_80216FB0;
        do {
            if (*entry == sentinel) {
                i++;
            } else {
                result = *entry + 1;
                break;
            }
            entry++;
        } while (i < count);
    }
    __asm__ volatile("");
    return result;
}
