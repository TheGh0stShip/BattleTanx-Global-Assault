#include "types.h"
#include "m2c_macros.h"

extern M2C_UNK func_8009EEE0(void *);
extern M2C_UNK func_8009EFD4(void *, s32, M2C_UNK, s32, s32);

void func_800A702C(void *arg0, s32 arg1, s32 arg2, s32 arg3, void *arg4) {
    if (arg2 == 1) {
        func_8009EEE0(arg4);
        func_8009EFD4(arg4, M2C_FIELD(arg0, s32 *, 0x10), 0,
                      M2C_FIELD(arg0, s32 *, 0x14),
                      M2C_FIELD(arg0, u16 *, 0xC));
        M2C_FIELD(arg4, u8 *, 0x40) = M2C_FIELD(arg0, u8 *, 0x19);
    }
}
