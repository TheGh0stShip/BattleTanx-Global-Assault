#include "os_thread.h"

extern void __osEnqueueAndYield(OSThread **queue);

void osYieldThread(void)
{
    register u32 saveMask;

    saveMask = __osDisableInt();
    __osRunningThread->state = OS_STATE_RUNNABLE;
    __osEnqueueAndYield(&__osRunQueue);
    __osRestoreInt(saveMask);
}
