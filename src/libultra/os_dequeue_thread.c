/* Data reconstruction: decompals/ultralib e24c836796df4bf520ff8b11a5c9d2cea3a66cbd src/os/thread.c */
#include "os_thread.h"

struct __osThreadTail {
    OSThread *next;
    OSPri priority;
};

struct __osThreadTail __osThreadTail = { 0, -1 }; /* D_80126EE0 */
OSThread *__osRunQueue = (OSThread *)&__osThreadTail;
OSThread *__osActiveQueue = (OSThread *)&__osThreadTail;
OSThread *__osRunningThread = 0;
OSThread *__osFaultedThread = 0;

void __osDequeueThread(OSThread **queue, OSThread *thread)
{
    register OSThread **link = queue;
    register OSThread *current = *link;

    while (current != 0) {
        if (current == thread) {
            *link = thread->next;
            return;
        }
        link = &current->next;
        current = *link;
    }
}
