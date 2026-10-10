#include "types.h"

extern s32 D_80216FA0[];
extern s32 D_80216FD0[];

void func_80098BC8(s32 controller) {
    s32 index = D_80216FA0[controller];

    if (index >= 0) {
        D_80216FD0[index] = 1;
    }
}
