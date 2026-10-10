#include "types.h"

typedef struct EmbeddedState {
    u16 id;
    u8 pad002[6];
    u16 value;
    u8 pad00A[2];
    f32 height;
    f32 x;
    f32 z;
} EmbeddedState;

typedef struct Object82070 {
    s32 kind;
    u8 pad004[4];
    f32 x;
    f32 z;
    u8 pad010[0x20 - 0x10];
    u16 value_a;
    u8 pad022[0x30 - 0x22];
    f32 scale;
    u8 pad034[0x48 - 0x34];
    u16 value_b;
    u8 pad04A[0x94 - 0x4A];
    u8 owner;
    u8 pad095[0x134 - 0x95];
    EmbeddedState state;
} Object82070;


u16 func_800B1898(s32, s16, s16, void *, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32);

void func_80082070(Object82070 *object) {
    f32 scale;
    EmbeddedState *state;
    EmbeddedState *output;

    scale = object->scale;
    state = &object->state;
    output = state;
    if (scale <= 0.0f) {
        output->height = 0.0f;
        output->value = object->value_a;
    } else {
        output->height = ((scale * 600.0f) / 60.0f) + 100.0f;
        output->value = object->value_b;
    }
    output->x = object->x;
    output->z = object->z;
    if (state->id == 0xFFFF) {
        state->id = func_800B1898(object->kind, (s16)state->x, (s16)state->z,
                                  0, -150, 150, -100,
                                  (s16)(state->height + 100.0f), 0, 0,
                                  state->value, 0xF8000000, object->owner);
    }
}
