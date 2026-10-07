#include "types.h"

typedef struct {
    u8 pad00[0xB8];
    s32 stride;
    void* slots;
} DisplaySlotState;

extern DisplaySlotState* D_80114500;

void* func_8007A720(s16 index) {
    register void* result __asm__("$2");

    if (index == -1) {
        result = 0;
    } else {
        result = (u8*)D_80114500->slots + index * D_80114500->stride * 2;
    }
    return result;
}
