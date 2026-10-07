#include "types.h"

extern s32 D_8021945C;

void func_80082BD4(u8* object) {
    s32 mode = *(s32*)(object + 0x158);

    if (mode != 1) {
        s32 deadline = *(s32*)(object + 0x15C);
        s32 now = D_8021945C;

        if (now >= deadline) {
            if (mode == 2) {
                *(s32*)(object + 0x158) = 3;
                *(s32*)(object + 0x15C) = now + 30;
            } else {
                *(s32*)(object + 0x158) = 1;
            }
        }
    }
}
