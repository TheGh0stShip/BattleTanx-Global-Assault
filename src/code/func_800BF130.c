#include "types.h"

typedef struct {
    u16 flags;
    u16 savedFlags;
    u8 pad[8];
    void *object;
} HudEntry;

extern HudEntry D_803A57F0[];

/* Clears the active flag for the first matching entry in the 20-slot table. */
void func_800BF130(void *object) {
    u16 i;

    for (i = 0; i < 20; i++) {
        if (D_803A57F0[i].object == object) {
            D_803A57F0[i].savedFlags = D_803A57F0[i].flags;
            D_803A57F0[i].flags &= ~2;
            break;
        }
    }
}
