#include "types.h"

typedef struct State8007AF84 {
    u8 pad[0xB0];
    s16 index3;
    s16 index2;
    s16 index1;
    s16 index0;
    s32 stride;
    u8 *base;
} State8007AF84;

extern State8007AF84 *D_80114500;

void *func_8007AF84(s32 mode) {
    register State8007AF84 *state asm("$4");
    register s32 index asm("$5");
    register void *result asm("$3") = 0;
    register s32 stride asm("$2");

    if (mode == 0) {
        state = D_80114500;
        index = state->index0;
    } else if (mode == 1) {
        state = D_80114500;
        index = state->index1;
    } else if (mode == 2) {
        state = D_80114500;
        index = state->index2;
    } else if (mode == 3) {
        state = D_80114500;
        index = state->index3;
    } else {
        goto done;
    }
    if (index != -1) {
        stride = state->stride;
        stride = index * stride;
        result = state->base + stride * 2;
    }
done:
    return result;
}
