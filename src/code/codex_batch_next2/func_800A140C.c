#include "types.h"

typedef struct {
    u8 pad_0[0x1E8];
    s32 frame_count;
    u16 task_interval;
    u16 task_ticks;
    u16 retraces;
    u16 field_1F2;
    u16 flags;
    u8 pad_1F6[2];
    s32 swap_state;
    s32 field_1FC;
    void *current_task;
    void *yielded_task;
} SchedulerState;

extern s32 D_80224B40;
extern u16 D_80000400[];

extern void func_80098AFC(void);
extern void func_80098BF8(void);
extern void __osSpSetPc(u32 pc);
extern void *func_80097660(void);
extern void osSpTaskYield(void);
extern void osWritebackDCacheAll(void);
extern void osSpTaskLoad(void *task);
extern void osSpTaskStartGo(void *task);
extern void func_800A1858(SchedulerState *state);
extern void *func_8007AF84(s32 index);
extern void *osViGetCurrentFramebuffer(void);
extern void func_8007ADF0(s32 index, void *framebuffer);
extern void func_800A17F0(SchedulerState *state);

void func_800A140C(SchedulerState *state) {
    s32 fade_counter;
    register u32 i __asm__("$4");
    register u32 limit __asm__("$5");
    register u16 *pixel __asm__("$3");
    register s32 saved __asm__("$17");
    register void *current_framebuffer __asm__("$2");
    void *second_framebuffer;

    func_80098AFC();
    fade_counter = D_80224B40;
    if (fade_counter != 0) {
        D_80224B40 = fade_counter + 1;
        if (fade_counter & 0x10) {
            i = 0;
            limit = 0x383FF;
            pixel = D_80000400;
            do {
                *pixel = (*pixel >> 1) & 0x7BDE;
                pixel++;
                i++;
            } while (i <= limit);
        }
        if ((D_80224B40 & 7) == 7) {
            func_80098BF8();
            __osSpSetPc(0);
        }
        return;
    }

    {
        u16 ticks;
        s32 frames;
        u32 interval;

        ticks = state->task_ticks;
        frames = state->frame_count;
        interval = state->task_interval;
        ticks++;
        frames++;
        state->task_ticks = ticks;
        state->frame_count = frames;
        if (ticks >= interval && state->current_task == 0) {
        state->current_task = func_80097660();
        if (state->current_task != 0) {
            if (state->flags != 0) {
                osSpTaskYield();
                state->flags |= 2;
            } else {
                osWritebackDCacheAll();
                osSpTaskLoad(state->current_task);
                osSpTaskStartGo(state->current_task);
                state->flags = 1;
            }
            state->task_ticks = 0;
        }
        }
    }

    state->retraces++;
    func_800A1858(state);
    saved = (s32)func_8007AF84(1);
    if (saved == 0) {
        return;
    }
    current_framebuffer = osViGetCurrentFramebuffer();
    if ((s32)current_framebuffer != saved) {
        return;
    }
    func_8007ADF0(0, current_framebuffer);
    saved = state->swap_state;
    if (saved == 1) {
        register s32 buffer_index __asm__("$4");

        __asm__ volatile("" : "=r"(saved) : "0"(saved));
        second_framebuffer = func_8007AF84(2);
        buffer_index = 1;
        func_8007ADF0(buffer_index, second_framebuffer);
        if (state->flags != 0) {
            state->yielded_task = 0;
            state->swap_state = 0;
            return;
        }
        if (state->field_1FC == saved) {
            func_800A17F0(state);
            state->swap_state = 0;
            return;
        }
        state->yielded_task = 0;
        state->swap_state = 0;
    } else {
        func_8007ADF0(1, 0);
    }
}
