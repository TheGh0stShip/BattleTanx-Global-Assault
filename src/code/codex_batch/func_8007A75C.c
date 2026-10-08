#include "types.h"

extern u8 *D_80114500;

s32 func_8007A75C(void *target) {
    s16 i;
    s32 stride;
    register u8 *state __asm__("$2");
    register u8 *state_ref __asm__("$7");
    void *current;

    if (target == 0) {
        return -1;
    }
    state = D_80114500;
    current = *(void **)(state + 0xBC);
    state_ref = state;
    for (i = 0; i < 3; i++) {
        if (current == target) {
            break;
        }
        stride = *(s32 *)(state_ref + 0xB8) * 2;
        current = (u8 *)current + stride;
    }
    return i;
}
