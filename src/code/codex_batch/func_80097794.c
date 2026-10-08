#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
M2C_UNK osCreateMesgQueue(M2C_UNK *, M2C_UNK *, M2C_UNK); /* extern */
extern M2C_UNK D_801B4570;
extern M2C_UNK D_801B4590;
extern M2C_UNK D_801B45B8;
extern M2C_UNK D_80216D98;

void func_80097794(void) {
    osCreateMesgQueue(&D_80216D98, &D_801B45B8, 8);
    osCreateMesgQueue(&D_801B4590, &D_801B4570, 8);
}
