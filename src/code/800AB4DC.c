/* SPAN 0x800AB58C */
#include "types.h"
#ifndef NULL
#define NULL 0
#endif
s32 func_800AAC14(s32, s32, s32, s32, s32, s32, s32); /* extern */
s32 func_800AAEA0(s16, s16, s16, s16, s32, s32 *, s32 *, s32 *, s32 *); /* extern */

void func_800AB4DC(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;

    if (func_800AAEA0(arg0, arg1, arg2, arg3, arg6, &sp28, &sp2C, &sp30, &sp34) != 0) {
        func_800AAC14(sp28, sp2C, sp30, sp34, arg4, arg5, arg6);
    }
}
