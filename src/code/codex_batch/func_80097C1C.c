#include "types.h"

void func_800FB7D4(s32, s32);
void MusSetMasterVolume(s32, s32);

typedef struct {
    s16 state;
    u8 pad[0xE];
    s32 volume;
} AudioState;

extern AudioState D_801B4540;
extern s32 D_801B4544;

void func_80097C1C(s32 arg0) {
    register s32 volume __asm__("$16") = arg0;
    register AudioState *state __asm__("$17") = &D_801B4540;

    if ((volume == 0) && (D_801B4544 != 0)) {
        func_800FB7D4(D_801B4544, 5);
        state->state = 2;
    }
    state->volume = volume;
    MusSetMasterVolume(2, volume);
}
