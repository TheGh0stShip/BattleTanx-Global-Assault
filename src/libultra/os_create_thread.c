#include "os_thread.h"

#define OS_IM_ALL 0x003FFF01
#define RCP_IMASK 0x003F0000
#define RCP_IMASKSHIFT 16
#define SR_IMASK 0x0000FF00
#define SR_EXL 0x00000002
#define SR_IE 0x00000001
#define FPCSR_FS 0x01000000
#define FPCSR_EV 0x00000800

extern void __osCleanupThread_801058A0(void);

void osCreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg,
                    void *sp, OSPri p)
{
    register u32 saveMask;
    OSIntMask mask;

    t->id = id;
    t->priority = p;
    t->next = 0;
    t->queue = 0;
    t->context.pc = (u32)entry;
    t->context.a0 = (s32)arg;
    t->context.sp = (s64)(s32)sp - 16;
    t->context.ra = (s32)__osCleanupThread_801058A0;
    mask = OS_IM_ALL;
    t->context.sr = SR_IMASK | SR_EXL | SR_IE;
    t->context.rcp = (mask & RCP_IMASK) >> RCP_IMASKSHIFT;
    t->context.fpcsr = (u32)(FPCSR_FS | FPCSR_EV);
    t->fp = 0;
    t->state = OS_STATE_STOPPED;
    t->flags = 0;
    saveMask = __osDisableInt();
    t->tlnext = __osActiveQueue;
    __osActiveQueue = t;
    __osRestoreInt(saveMask);
}
