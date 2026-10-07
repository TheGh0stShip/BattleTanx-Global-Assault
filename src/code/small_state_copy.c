#include "types.h"

typedef struct {
    u32 value;
    u16 type;
    u8 state;
} SmallState;

typedef struct {
    s32* value;
    u16 type;
    u8 pad_6[0x1A];
    u16 state;
} SmallStateSource;

void func_8007D558(SmallState* destination, SmallStateSource* source) {
    if (source->value == 0) {
        destination->value = 0;
    } else {
        destination->value = *source->value;
    }
    destination->type = source->type;
    destination->state = source->state;
}

s32 func_8007D588(SmallState* first, SmallState* second) {
    s32 equal = 0;

    if (first->value == second->value && first->type == second->type) {
        equal = 1;
    }
    return equal;
}
