#include "types.h"

typedef struct {
    u32 value;
    u16 type;
    u8 state;
} SmallState;

typedef struct {
    u8 mode;
    u8 pad_1;
    u16 field_2;
    f32 field_4;
    f32 field_8;
    SmallState state;
    u8 field_14;
    u8 pad_15;
    u16 field_16;
} ModeState;

typedef struct {
    u8 mode;
    u8 pad_1;
    u16 field_2;
    f32 field_4;
    f32 field_8;
    u16 field_C;
    u16 field_E;
    u16 field_10;
    u8 field_12;
} ModeTwoState;

extern void func_8007D470(SmallState* state);

void Steps_GetNextLegPtr(ModeState* state) {
    state->mode = 1;
    state->field_2 = 0;
    state->field_4 = 0.0f;
    *(u16*)&state->field_8 = 0;
}

void Steps_FreeStep(ModeTwoState* state) {
    state->mode = 2;
    state->field_2 = 0;
    state->field_4 = 0.0f;
    state->field_8 = 0.0f;
    state->field_C = 0;
    state->field_E = 0;
    state->field_10 = 0;
    state->field_12 = 0;
}

void Steps_PruneFork(ModeState* state) {
    f32 zero = 0.0f;

    state->mode = 3;
    state->field_2 = 0;
    state->field_8 = zero;
    state->field_4 = zero;
    func_8007D470(&state->state);
    state->field_14 = 0;
    state->field_16 = 0;
}

void Steps_CropLinearBranch(ModeState* state) {
    state->mode = 4;
    state->field_2 = 0;
    state->field_4 = 0.0f;
    state->field_8 = 0.0f;
}
