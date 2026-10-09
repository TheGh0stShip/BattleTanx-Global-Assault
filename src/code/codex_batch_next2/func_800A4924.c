#include "types.h"

typedef struct EventNode4924 {
    s32 type;
    u8 pad04[0x20];
    s32 delay;
    u16 scale;
    u8 pad2A[6];
    struct EventNode4924 *next;
} EventNode4924;

typedef struct {
    u8 pad00[0xC];
    EventNode4924 *event;
    s32 timestamp;
    u8 position[0xC];
    u16 value20;
    u16 value22;
    u8 value24;
} EventState4924;

extern s32 D_8021945C;
extern void func_800A5BD8(void *position, u16 value, u8 mode, f32 scale,
                         EventNode4924 *event, s32 arg5);

void func_800A4924(EventState4924 *state, s32 *done) {
    EventNode4924 *event;
    s32 elapsed;

    event = state->event;
    elapsed = D_8021945C - state->timestamp;
    if (elapsed < event->delay) {
        return;
    }
    if (event->next != 0) {
        if (event->next->type == 2) {
            *(volatile u16 *)&state->value22 = state->value22 + elapsed * event->scale;
            state->event = event->next;
            state->timestamp = D_8021945C;
            return;
        }
        func_800A5BD8(state->position, state->value20, state->value24, 1.0f,
                      event->next, 0);
    }
    *done = 1;
}
