#include "types.h"

typedef struct EventNode49D0 {
    s32 type;
    s32 delay;
    u8 pad08[0x24];
    struct EventNode49D0 *next;
} EventNode49D0;

typedef struct {
    u8 pad00[0xC];
    EventNode49D0 *event;
    s32 timestamp;
    u8 position[8];
    f32 velocity;
    u8 pad20[2];
    u8 mode22;
    u8 pad23[9];
} EventState49D0;

extern s32 D_8021945C;
extern f32 D_80219488;
extern void func_800A5BD8(void *position, u16 value, u8 mode, f32 scale,
                         EventNode49D0 *event, s32 arg5);

void func_800A49D0(EventState49D0 *state, s32 *done) {
    EventNode49D0 *event;
    s32 now;

    state->velocity += D_80219488 * *(f32 *)((u8 *)state + 0x2C);
    event = state->event;
    now = D_8021945C;
    if ((now - state->timestamp) < event->delay) {
        return;
    }
    if (event->next != 0) {
        if (event->next->type == 10) {
            state->event = event->next;
            state->timestamp = now;
            return;
        }
        func_800A5BD8(state->position, 0, state->mode22, 1.0f,
                      event->next, 0);
    }
    *done = 1;
}
