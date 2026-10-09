#include "types.h"

void func_800AAAE0(void);
void func_800AF978(s32, s32);
extern u8 D_802194A5;

void func_800B0268(void) {
    volatile s32 frame_pad[2];
    s32 index;
    s32 offset;
    s32 count;
    s32 value;

    func_800AAAE0();
    __asm__("lbu %0,D_802194A5" : "=r"(count));
    if (count > 0) {
        index = 0;
        offset = 0;
        do {
            __asm__("lui $1,%%hi(D_802194D9)\n"
                    "addu $1,$1,%1\n"
                    "lbu %0,%%lo(D_802194D9)($1)"
                    : "=r"(value) : "r"(offset), "r"(index));
            func_800AF978(value, index);
            index++;
            offset += 0x28;
        } while (index < (s32)D_802194A5);
    }
    (void)frame_pad;
}
