#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK MusPtrBankSetCurrent(M2C_UNK *);            /* extern */
M2C_UNK func_800FBB34(M2C_UNK *, M2C_UNK *);        /* extern */
M2C_UNK func_800FBCFC(M2C_UNK *);                   /* extern */
M2C_UNK osViExtendVStart(M2C_UNK *);                /* extern */
M2C_UNK func_80097BC4(M2C_UNK, M2C_UNK *, s32);     /* static */
extern s32 D_80114714;
extern s32 D_8011471C;
extern M2C_UNK D_801F3198;
extern M2C_UNK D_801FE598;
extern M2C_UNK D_B0593A28;

void func_8009790C(void) {
    s32 temp_s2;

    temp_s2 = D_8011471C;
    func_80097BC4(0, &D_801FE598, D_80114714);
    func_80097BC4(1, &D_801F3198, temp_s2);
    func_800FBB34(&D_801FE598, &D_B0593A28);
    func_800FBCFC(&D_801F3198);
    MusPtrBankSetCurrent(&D_801FE598);
    osViExtendVStart(&D_801F3198);
}
