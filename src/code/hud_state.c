#include "types.h"

typedef struct {
    u8 pad_0[8];
    u8* state;
} HudOwner;

extern s16 D_803A5970;
extern s16 D_803A5972;

s32 func_800BFD40(s32 arg0, HudOwner* owner) {
    owner->state[7] = 0xFF;
    D_803A5970 = 0;
    D_803A5972 = 0;
    return 1;
}

s32 func_800BFD64(s32 arg0, HudOwner* owner) {
    owner->state[7] = 0;
    D_803A5970 = 0;
    D_803A5972 = 0;
    return 1;
}

s32 func_800BFD84(void) {
    return D_803A5970;
}

s32 func_800BFD94(void) {
    return D_803A5972;
}
