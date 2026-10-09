#include "types.h"

extern u8 D_80236A90[];

s32 func_800866B0(s32 arg0) {
    s32 index;
    s32 mask;
    u8 *base;
    u8 *candidate;

    index = 0;
    mask = 0x08000000;
    base = D_80236A90;
loop:
    {
        register u8 *fixed_candidate __asm__("$2");

        /* Preserve the original operand order: addu v0, v0, a3. */
        __asm__(".word 0x00471021" : "=r"(fixed_candidate)
                                  : "r"((index & 0xFFFF) * 0x1C), "r"(base));
        candidate = fixed_candidate;
    }
    if (arg0 != candidate) {
        index += 1;
        mask *= 2;
        if ((u32)(index & 0xFFFF) >= 5U) {
            return 0xF8000000;
        }
        goto loop;
    }
    return mask;
}
