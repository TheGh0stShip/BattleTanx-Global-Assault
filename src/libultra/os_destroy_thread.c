#include "os_thread.h"

extern void __osDequeueThread(OSThread **queue, OSThread *t);
extern void __osDispatchThread(void);

void osDestroyThread(OSThread *t)
{
    register u32 saveMask;
    register OSThread *pred, *succ;

    saveMask = __osDisableInt();
    if (t == 0)
        t = __osRunningThread;
    else if (t->state != OS_STATE_STOPPED)
        __osDequeueThread(t->queue, t);
    if (__osActiveQueue == t) {
        __osActiveQueue = __osActiveQueue->tlnext;
    } else {
        pred = __osActiveQueue;
        while ((succ = pred->tlnext) != 0) {
            if (succ == t) {
                pred->tlnext = t->tlnext;
                break;
            }
            pred = succ;
        }
    }
    if (t == __osRunningThread)
        __osDispatchThread();
    __osRestoreInt(saveMask);
}
