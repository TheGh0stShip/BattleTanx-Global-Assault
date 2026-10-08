#include "types.h"

extern void func_80086208(void *object, s32 value);

void func_800831D0(void *object) {
    s32 *state = (s32 *)((u8 *)object + 0x170);

    if (((*state & 1) && *(s32 *)((u8 *)object + 0xA8) != 0) ||
        ((*state & 2) &&
         *(s32 *)((u8 *)object + 0x1D8) <
             *(s32 *)((u8 *)object + 0x1D4))) {
        s32 value = state[1];
        *state = 0;
        func_80086208(object, value);
    }
}
