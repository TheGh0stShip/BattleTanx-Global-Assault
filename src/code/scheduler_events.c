#include "types.h"

typedef struct {
    u8 pad_0[0x1EC];
    u16 interval;
    u16 field_1EE;
    u16 counter;
    u16 pending;
    u16 flags;
    u8 pad_1F6[6];
    s32 state_1FC;
    s32 task_200;
    s32 task_204;
} SchedulerState;

extern s32 D_80224B40;

extern void func_800A17F0(SchedulerState* state);
extern void* func_8007ADC0(void);
extern void func_8007ADF0(s32 slot, void* task);
extern void func_801101FC(void* task);
extern void func_8011038C(void* task);
extern void* func_8007AF84(s32 slot);
extern void func_80111330(s32 arg0);
extern void func_80111CB0(void* task);

void func_800A179C(SchedulerState* state) {
    if ((state->task_204 != 0) || (state->flags != 0)) {
        state->state_1FC = 1;
    } else {
        func_800A17F0(state);
    }
}

void func_800A17DC(void) {
    D_80224B40 = 1;
}

void func_800A17F0(SchedulerState* state) {
    state->task_204 = (s32)func_8007ADC0();
    func_8007ADF0(2, *(void**)((u8*)state->task_204 + 0x40));
    func_8007ADF0(3, 0);
    func_801101FC((void*)state->task_204);
    func_8011038C((void*)state->task_204);
    state->state_1FC = 0;
    state->flags = 1;
}

void func_800A1858(SchedulerState* state) {
    void* task;

    if (state->counter >= state->interval) {
        task = func_8007AF84(1);
        if (task != 0) {
            if (state->pending != 0) {
                func_80111330(0);
                state->pending = 0;
            }
            func_80111CB0(task);
            state->counter = 0;
        }
    }
}
