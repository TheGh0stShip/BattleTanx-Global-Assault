#include "types.h"

typedef struct {
    s32 value;
    u8 flag;
} SmallState;

void func_80086160(SmallState *state) {
    state->value = 0;
    state->flag = 0;
}
