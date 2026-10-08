#include "types.h"
#include "m2c_macros.h"

extern u8 D_80216E00[];
extern M2C_UNK D_80217010;
extern M2C_UNK D_80219200;
extern s32 func_8010CA40(void *);
extern M2C_UNK osRecvMesg(M2C_UNK *, M2C_UNK *, M2C_UNK);
extern M2C_UNK osSendMesg(M2C_UNK *, M2C_UNK *, M2C_UNK);

s32 func_800996C4(s32 arg0) {
    s32 temp_s0;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    temp_s0 = func_8010CA40(((arg0 - 1) * 0x68) + D_80216E00);
    osSendMesg(&D_80217010, &D_80219200, 0);
    return temp_s0;
}
