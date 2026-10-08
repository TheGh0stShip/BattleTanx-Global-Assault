#include "types.h"

extern s32 D_80114804;
extern s32 D_80219208[];
extern u8 D_80219218[];

void func_80098534(void) {
    s32 count = D_80114804;
    s32 index = 0;
    s32 *base;
    s32 offset;
    u16 value;

    if (count > 0) {
        base = D_80219208;
        do {
            offset = base[(u16)index] * 6;
            __asm__("lui $1,%%hi(D_80219238)\n"
                    "addu $1,$1,%1\n"
                    "lhu %0,%%lo(D_80219238)($1)"
                    : "=r"(value) : "r"(offset), "r"(index));
            *(u16 *)(D_80219218 + offset) = value;
            index += 1;
        } while ((u16)index < count);
    }
}
