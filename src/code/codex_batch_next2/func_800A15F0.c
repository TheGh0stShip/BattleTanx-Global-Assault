#include "types.h"

typedef struct {
    u8 pad_0[0x1F4];
    u16 flags;
    u8 pad_1F6[6];
    s32 field_1FC;
    void *current_task;
    void *yielded_task;
} SchedulerState;

extern s32 osSpTaskYielded(void *task);
extern void osWritebackDCacheAll(void);
extern void osSpTaskLoad(void *task);
extern void osSpTaskStartGo(void *task);
extern void func_800976AC(void);
extern void func_800A17F0(SchedulerState *state);

void func_800A15F0(SchedulerState *state) {
    if (state->flags & 2) {
        state->flags &= ~2;
        if (osSpTaskYielded(state->yielded_task)) {
            state->flags |= 4;
        }
        osWritebackDCacheAll();
        osSpTaskLoad(state->current_task);
        osSpTaskStartGo(state->current_task);
        state->flags |= 1;
    } else {
        if (state->current_task == 0) {
            state->flags = 0;
            return;
        }
        func_800976AC();
        state->current_task = 0;
        if (state->flags & 4) {
            if (state->yielded_task == 0) {
                state->flags = 0;
                return;
            }
            osSpTaskLoad(state->yielded_task);
            osSpTaskStartGo(state->yielded_task);
            if (state->flags & 8) {
                state->yielded_task = 0;
            }
            state->flags = 1;
        } else {
            if (state->yielded_task != 0) {
                state->flags = 0;
                return;
            }
            if (state->field_1FC == 1) {
                func_800A17F0(state);
                return;
            }
            state->flags = 0;
        }
    }
}
