#include "types.h"

typedef struct {
    u16 flags;
    u16 field2;
    s8 field4;
    s8 children[4];
    u8 pad9[3];
    void *object;
} Entry800BF3EC;

extern Entry800BF3EC D_803A57F0[];

void func_800BC9F4(void *object);
void func_800BF3EC(u16 index);

void func_800BF3EC(u16 index) {
    u16 i;

    if (D_803A57F0[index].flags & 1) {
        func_800BC9F4(D_803A57F0[index].object);
    }
    for (i = 0; i < 4; i++) {
        if (D_803A57F0[index].children[i] > 0) {
            func_800BF3EC(D_803A57F0[index].children[i]);
        }
    }
}
