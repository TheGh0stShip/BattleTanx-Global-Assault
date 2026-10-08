#include "types.h"

void func_80089E28(void *state, s32 value) {
    register u32 index __asm__("$2") = *(u16 *)((u8 *)state + 2);
    register u32 offset __asm__("$3");
    register u8 *slot __asm__("$3");
    register s32 count __asm__("$3");

    __asm__ volatile("" : "=r"(index) : "0"(index));
    offset = index * 4;
    __asm__ volatile("" : "=r"(offset) : "0"(offset));
    slot = (u8 *)(offset + (u32)state);

    *(s32 *)(slot + 4) = value;
    count = *(s32 *)state;
    count++;
    *(s32 *)state = count;
}
