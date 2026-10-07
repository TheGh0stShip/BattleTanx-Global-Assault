#include "types.h"

typedef struct {
    u8 pad_0[0x1E8];
    s32 field_1E8;
    u8 pad_1EC[2];
    u16 field_1EE;
    u16 field_1F0;
    u16 field_1F2;
    u16 field_1F4;
    u8 pad_1F6[2];
    s32 field_1F8;
    s32 field_1FC;
    s32 field_200;
    s32 field_204;
} SchedulerState;

extern s32 D_80224B40;

void func_800A134C(SchedulerState* state) {
    state->field_1E8 = 0;
    state->field_1EE = 0;
    state->field_1F0 = 0;
    state->field_1F2 = 1;
    state->field_1F4 = 0;
    state->field_1F8 = 0;
    state->field_1FC = 0;
    state->field_200 = 0;
    state->field_204 = 0;
    D_80224B40 = 0;
}
