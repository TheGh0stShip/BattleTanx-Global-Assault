#include "types.h"


void func_80088C70(void *state) {
    f32 initial = 32768.0f;
    f32 zero = 0.0f;

    *((u8 *)state + 0x18) = 0;
    *(s32 *)((u8 *)state + 8) = 0;
    *(s32 *)((u8 *)state + 0xC) = 0;
    *(u16 *)((u8 *)state + 0x14) = 0;
    *((u8 *)state + 0x16) = 0;
    *((u8 *)state + 0x17) = 0;
    *((u8 *)state + 0x19) = 0;
    *(f32 *)((u8 *)state + 4) = zero;
    *(f32 *)state = zero;
    *(f32 *)((u8 *)state + 0x10) = initial;
}
