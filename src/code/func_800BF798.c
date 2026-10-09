#include "types.h"

u64 osGetTime(void);
extern u64 D_803A5938;
extern u8 D_803A57F0[];
extern u16 D_80116840;
extern u16 D_803A5972;
extern u16 D_803A5970;

void func_800BF798(void) {
    u64 now = osGetTime();
    register s32 index __asm__("$3");

    D_803A5938 = now;
    index = 0;
    do {
        register u32 offset __asm__("$2") = (index & 0xFFFF) * 0x10;
        __asm__("lui $1,%%hi(D_803A57F0)\n"
                "addu $1,$1,%0\n"
                "sh $0,%%lo(D_803A57F0)($1)"
                :
                : "r"(offset));
        index++;
    } while ((u32)(index & 0xFFFF) < 0x14);
    D_80116840 = 0;
    D_803A5972 = 0;
    D_803A5970 = 0;
}
