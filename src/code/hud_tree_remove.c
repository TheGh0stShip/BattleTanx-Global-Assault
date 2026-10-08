#include "types.h"

typedef struct {
    u16 flags;
    u16 field2;
    s8 parent;
    s8 children[4];
    u8 pad9[3];
    void* object;
} HudNode;

extern HudNode D_803A57F0[];
extern void func_800BEE0C(u16 index);

void func_800BF000(void* object) {
    u16 i;
    u16 k;
    u16 m;
    u16 child;

    for (i = 0; i < 20; i++) {
        if (D_803A57F0[i].object == object) {
            for (k = 0; k < 4; k++) {
                if (D_803A57F0[i].children[k] > 0) {
                    child = D_803A57F0[i].children[k];
                    for (m = 0; m < 4; m++) {
                        if (D_803A57F0[child].children[m] > 0) {
                            func_800BEE0C(D_803A57F0[child].children[m]);
                            D_803A57F0[child].children[m] = -1;
                        }
                    }
                    D_803A57F0[child].flags = 0;
                    D_803A57F0[i].children[k] = -1;
                }
            }
            return;
        }
    }
}
