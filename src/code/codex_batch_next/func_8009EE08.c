#include "types.h"

extern u8 D_8021C060;
extern u8 D_8021BD40;
extern u8 D_8021BB90;
extern u8 D_8021B9E0;
extern void func_8009EEA0(void *argument);

extern void osCreatePiManager(s32 priority, void *queue, void *messages,
                              s32 count);
extern void osCreateThread(void *thread, s32 id, void (*entry)(void *),
                           void *argument, void *stack, s32 priority);
extern void osStartThread(void *thread);
extern void osSetThreadPri(void *thread, s32 priority);

void func_8009EE08(void *argument) {
    osCreatePiManager(0x96, &D_8021C060, &D_8021BD40, 0xC8);
    osCreateThread(&D_8021BB90, 2, func_8009EEA0, argument,
                   &D_8021B9E0, 10);
    osStartThread(&D_8021BB90);
    osSetThreadPri(0, 0);
    for (;;) {
    }
}
