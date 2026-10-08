#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK MusPtrBankSetCurrent(M2C_UNK *);            /* extern */
M2C_UNK func_800FBB34(M2C_UNK *, M2C_UNK *);        /* extern */
M2C_UNK func_80097BC4(M2C_UNK, M2C_UNK *, s32);     /* static */
extern s32 D_8011472C;
extern M2C_UNK D_801F3D98;
extern M2C_UNK D_B0624008;

void func_800979A0(void) {
    func_80097BC4(3, &D_801F3D98, D_8011472C);
    func_800FBB34(&D_801F3D98, &D_B0624008);
    MusPtrBankSetCurrent(&D_801F3D98);
}
