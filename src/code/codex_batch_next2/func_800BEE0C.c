#include "types.h"

extern s8 D_803A57F5[];
extern u8 D_803A57F0[];

void func_800BEE0C(u16 index) {
    register u16 i asm("$17");
    s8 *children;
    register s8 *child asm("$16");

    i = 0;
    children = D_803A57F5 + index * 16;
    do {
        child = children + i;
        if (*child > 0) {
            func_800BEE0C((u16)*child);
            *child = -1;
        }
        i++;
    } while (i < 4);
    *(s16 *)(D_803A57F0 + index * 16) = 0;
}
