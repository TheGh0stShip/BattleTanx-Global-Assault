#include "types.h"

typedef struct {
    u16 field0;
    u16 field2;
    u8 pad4[8];
    void *fieldC;
} Entry800BF1A4;

extern Entry800BF1A4 D_803A57F0[];

void func_800BF1A4(void *value) {
    u16 i = 0;

    while (i < 20) {
        if (D_803A57F0[i].fieldC == value) {
            D_803A57F0[i].field0 = (D_803A57F0[i].field2 & ~2) | 0x40;
            break;
        }
        i++;
    }
}
