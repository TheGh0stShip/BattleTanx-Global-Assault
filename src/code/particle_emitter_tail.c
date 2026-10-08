/* ---- 0x800ED800/tu/object_release_tail_800EFBFC.c ---- */
#include "types.h"
#define NULL ((void *)0)
void func_80097BA4(void *, s32);
void func_800EFBFC(u8 *a) { void *p = *(void **)(a + 0x40); if (p != NULL) func_80097BA4(p, 0); }
void *func_800EFC28(u8 *o) {
    u8 *p, *q;
    if (*(s16 *)(o + 0x3E) > 0) {
        p = *(u8 **)(o + 0x30);
        q = *(u8 **)(p + 0x14);
        while (q != NULL) { p = q; q = *(u8 **)(p + 0x14); }
        return p;
    }
    return *(u8 **)(o + 0x30);
}

