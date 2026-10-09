#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK osCreateThread(M2C_UNK *, M2C_UNK, M2C_UNK (*)(), s32, M2C_UNK *, s32); /* extern */
M2C_UNK osInitialize();                             /* extern */
M2C_UNK osStartThread(M2C_UNK *);                   /* extern */
M2C_UNK func_8009EE08();                            /* static */
extern M2C_UNK D_8021B5E0;
extern M2C_UNK D_8021B9E0;

void func_8009ED9C(s32 arg0) {
    osInitialize();
    osCreateThread(&D_8021B9E0, 1, func_8009EE08, arg0, &D_8021B5E0, 0xF);
    osStartThread(&D_8021B9E0);
}
