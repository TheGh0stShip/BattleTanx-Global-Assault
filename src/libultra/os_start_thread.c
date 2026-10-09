/* Source shape adapted from the MIT-licensed Dr. Mario 64 libultra 2.0I. */
#include "os_thread.h"

extern void __osEnqueueThread(OSThread **, OSThread *);
extern OSThread *__osPopThread(OSThread **);
extern void __osDispatchThread(void);
extern void __osEnqueueAndYield(OSThread **);

void osStartThread(OSThread *thread)
{
    register u32 saveMask = __osDisableInt();

    switch (thread->state) {
    case OS_STATE_WAITING:
        thread->state = OS_STATE_RUNNABLE;
        __osEnqueueThread(&__osRunQueue, thread);
        break;
    case OS_STATE_STOPPED:
        if (thread->queue == 0 || thread->queue == &__osRunQueue) {
            thread->state = OS_STATE_RUNNABLE;
            __osEnqueueThread(&__osRunQueue, thread);
        } else {
            thread->state = OS_STATE_WAITING;
            __osEnqueueThread(thread->queue, thread);
            __osEnqueueThread(
                &__osRunQueue, __osPopThread(thread->queue));
        }
        break;
    }

    if (__osRunningThread == 0) {
        __osDispatchThread();
    } else if (__osRunningThread->priority < __osRunQueue->priority) {
        __osRunningThread->state = OS_STATE_RUNNABLE;
        __osEnqueueAndYield(&__osRunQueue);
    }

    __osRestoreInt(saveMask);
}
