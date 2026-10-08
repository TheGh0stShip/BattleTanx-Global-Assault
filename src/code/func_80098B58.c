#include "types.h"

extern s32 D_80216FA0[];
extern s32 D_80216FD0[];
extern s32 D_80216FE0[];
extern s32 D_80216FF0[];
extern s32 D_80217000[];

void func_80098B58(s32 index, s32 value, s32 amount, s32 duration) {
    register s32 slot asm("$3") = D_80216FA0[index];
    s32 *amountPtr;
    s32 *durationPtr;

    if (slot >= 0) {
        amountPtr = &D_80216FE0[slot];
        if (amount == 0) {
            amount = 1;
        }
        *amountPtr = amount;
        durationPtr = &D_80216FF0[slot];
        if (duration <= 0) {
            duration = 1;
        }
        *durationPtr = -duration;
        D_80217000[slot] = -1;
        D_80216FD0[slot] = value;
    }
}
