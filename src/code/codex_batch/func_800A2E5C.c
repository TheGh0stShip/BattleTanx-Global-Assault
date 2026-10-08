#include "types.h"
#include "m2c_macros.h"

void func_800CA620(s32, s32, s32);

void func_800A2E5C(void *arg0, s32 *arg1) {
    s32 previous;
    s32 counter;
    s32 index;
    s32 value;

    previous = M2C_FIELD(arg0, s32 *, 0xC);
    counter = previous + 1;
    M2C_FIELD(arg0, s32 *, 0xC) = counter;
    if (!(counter & 0xF)) {
        index = counter;
        if (index < 0) {
            index = previous + 0x10;
        }
        index = (index >> 4) * 4;
        __asm__("lui $1,%%hi(D_80114D3C)\n"
                "addu $1,$1,%1\n"
                "lw %0,%%lo(D_80114D3C)($1)"
                : "=r"(value) : "r"(index));
        func_800CA620(0, value, 0x2D);
        if (((s32)M2C_FIELD(arg0, s32 *, 0xC) / 16) == 0x3A) {
            *arg1 = 1;
        }
    }
}
