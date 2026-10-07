#include "types.h"

extern s32 D_8021945C;

typedef struct {
    s32 mode;
    s32 resource;
    s32 value;
    u16 state;
} ModeOwner;

void func_80082B40(ModeOwner* owner) {
    owner->mode = 1;
    owner->value = 0;
    owner->state = 0;
    owner->resource = D_8021945C;
}

void func_80082B60(void) {
}
