#include "types.h"

u32 func_80077CE0(s32);
void func_80077CA8(s32, u32);

void func_80077FD0(s32 arg0, s32 arg1) {
    register s32 aligned __asm__("$16");
    register s32 input __asm__("$17");
    register s32 shift __asm__("$18");
    register s32 call_arg __asm__("$4");
    u32 value;

    aligned = arg0;
    shift = ~aligned;
    aligned &= ~3;
    call_arg = aligned;
    input = arg1;
    shift = (shift & 3) * 8;
    value = func_80077CE0(call_arg);
    __asm__ volatile("addu %0,%1,$0" : "=r"(call_arg) : "r"(aligned));
    value &= ~(0xFFU << shift);
    __asm__ volatile("" : "=r"(value) : "0"(value));
    input &= 0xFF;
    input <<= shift;
    value |= input;
    func_80077CA8(call_arg, value);
}
