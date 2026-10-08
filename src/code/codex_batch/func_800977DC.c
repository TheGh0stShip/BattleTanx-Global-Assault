#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK osRecvMesg(M2C_UNK *, s32 *, M2C_UNK);      /* extern */
M2C_UNK func_80097DF8();                            /* static */
extern M2C_UNK D_80216D98;
extern s32 D_80224B40;

void func_800977DC(void) {
    s32 sp10;

    sp10 = 0;
    osRecvMesg(&D_80216D98, NULL, 1);
    do {
        osRecvMesg(&D_80216D98, &sp10, 0);
    } while (sp10 != 0);
    if (D_80224B40 == 0) {
        func_80097DF8();
    }
}
