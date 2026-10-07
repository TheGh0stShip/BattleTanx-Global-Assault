#include "types.h"

typedef struct {
    u8 pad_0[0x74];
    s32 state;
    u8 pad_78[0x1D8];
} PlayerState;

extern u8 D_802194A4;
extern u8 D_802194A6;
extern PlayerState D_80235F00[];

void func_8009C284(void) {
    u16 index = 0;

    while (index < D_802194A6) {
        index++;
    }
}

s32 func_8009C2B4(void) {
    s32 index;
    PlayerState* player;

    for (index = 0; index < D_802194A4; index++) {
        if (index != 0x7F) {
            goto nonnull;
        }
        player = 0;
        goto selected;
nonnull:
        player = &D_80235F00[index];
selected:
        if (player->state == 1) {
            return 1;
        }
    }
    return 0;
}
