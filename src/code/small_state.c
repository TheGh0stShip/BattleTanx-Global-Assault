#include "types.h"

typedef struct {
    u32 value;
    u16 type;
    u8 state;
} SmallState;

void func_8007D470(SmallState* state) {
    state->value = 0;
    state->type = 0xFFFF;
    state->state = 0;
}

s32 func_8007D484(u16 value) {
    return value != 0 && value < 5;
}

s32 func_8007D498(void) {
    return 1;
}

void func_8007D4A0(SmallState* source, SmallState* destination) {
    u32 value = source->value;
    u16 type = source->type;
    u8 state = source->state;

    destination->value = value;
    destination->type = type;
    destination->state = state;
}
