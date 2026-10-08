#include "types.h"
#include "m2c_macros.h"

extern s32 D_8021945C;

void func_8008C5BC(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_a1;
    s32 temp_v0;

    temp_v0 = D_8021945C;
    temp_a1 = (arg1 * 4) + arg0;
    M2C_FIELD(temp_a1, s32 *, 0x238) = arg2;
    M2C_FIELD(temp_a1, s32 *, 0x230) = temp_v0;
}
