#include "types.h"

extern u8 D_80222930[];
extern u8 D_80224B40[];
extern s16 D_80222B1C;
extern u8 osViModeTable[];

extern void osCreateMesgQueue(void *queue, void *messages, s32 count);
extern void osCreateViManager(s32 priority);
extern void osViSetMode(void *mode);
extern void osViSetSpecialFeatures(u32 features);
extern void osViBlack(s32 active);
extern void osSetEventMesg(s32 event, void *queue, s32 message);
extern void osViSetEvent(void *queue, s32 message, u8 retrace_count);
extern void func_800A134C(void *state);
extern void func_800A1290(void *queue);
extern void osCreateThread(void *thread, s32 id, void (*entry)(void *),
                           void *arg, void *stack, s32 priority);
extern void osStartThread(void *thread);

void func_800A1150(s32 thread_id, s32 priority, u8 video_mode,
                   u8 retrace_count, s32 field_21C) {
    void *scheduler_thread;

    osCreateMesgQueue(D_80222930, D_80222930 + 0x18, 8);
    osCreateViManager(0xFE);
    osViSetMode(osViModeTable + video_mode * 0x50);
    osViSetSpecialFeatures(0x42);
    osViBlack(1);
    osSetEventMesg(4, D_80222930, 0x29B);
    osSetEventMesg(9, D_80222930, 0x29C);
    osSetEventMesg(14, D_80222930, 0x29E);
    osViSetEvent(D_80222930, 0x29A, retrace_count);
    D_80222B1C = field_21C;
    func_800A134C(D_80222930);

    scheduler_thread = D_80222930 + 0x38;
    osCreateThread(scheduler_thread, thread_id, func_800A1290, D_80222930,
                   D_80224B40, priority);
    osStartThread(scheduler_thread);
}
