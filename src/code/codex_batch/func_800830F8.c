#include "types.h"
#include "m2c_macros.h"

void func_80086208(void *, s32);
void func_80088854(void);

void func_800830F8(void *arg0) {
    s32 flags;
    s32 offset;

    if (M2C_FIELD(arg0, s32 *, 0xA0) != 0) {
        func_80086208(arg0, 0x13);
        return;
    }
    if (M2C_FIELD(arg0, s32 *, 0x1D8) <= M2C_FIELD(arg0, s32 *, 0x1C8)) {
        offset = M2C_FIELD(arg0, s32 *, 0x98) * 0xD0;
        __asm__("lui $1,%%hi(D_80122EB8)\n"
                "addu $1,$1,%1\n"
                "lw %0,%%lo(D_80122EB8)($1)"
                : "=r"(flags) : "r"(offset));
        if (flags & 1) {
            func_80086208(arg0, 0x15);
            return;
        }
    }
    func_80088854();
}
