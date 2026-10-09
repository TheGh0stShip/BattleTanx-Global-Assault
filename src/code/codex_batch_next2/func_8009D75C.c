/* SPAN 0x8009D81C */
#include "types.h"

extern s32 func_8009D81C(s32 target, s32 current);

u16 func_8009D75C(u16 *current_arg, s32 target_arg, s32 step_arg) {
    register u16 *current_ptr asm("$17") = current_arg;
    register s32 target asm("$19") = target_arg;
    register s32 step_value asm("$18");
    register u16 current asm("$21");
    s32 target_value;
    s32 current_value;
    register s32 wrap asm("$22");
    register s32 sum asm("$2");
    u32 difference;
    s32 distance;
    s32 step_work;
    s32 result_work;

    current = *current_ptr;
    target_value = target & 0xFFFF;
    wrap = 0x10000;
    sum = target_value + wrap;
    step_value = step_arg;
    current_value = current & 0xFFFF;
    difference = sum - current_value;
    distance = func_8009D81C(target_value, current_value);
    step_work = step_value;
    result_work = distance;
    step_work &= 0xFFFF;

    if ((u32)(result_work & 0xFFFF) < (u32)step_work) {
        *current_ptr = target;
    } else if ((difference >> 15) & 1) {
        register s32 wrapped_current asm("$2");
        wrapped_current = current_value + wrap;
        *current_ptr = wrapped_current - step_work;
    } else {
        *current_ptr = current + step_value;
    }

    return distance;
}
