#include "ultra.h"

extern OSTimer *__osTimerList;
extern OSTime __osInsertTimer(OSTimer *timer);
extern void __osSetTimerIntr(OSTime time);

s32 osSetTimer(OSTimer *timer, OSTime countdown, OSTime interval,
               OSMesgQueue *mq, OSMesg msg)
{
    OSTime time;

    timer->next = 0;
    timer->prev = 0;
    timer->interval = interval;
    if (countdown != 0)
        timer->value = countdown;
    else
        timer->value = interval;
    timer->mq = mq;
    timer->msg = msg;
    time = __osInsertTimer(timer);
    if (__osTimerList->next == timer)
        __osSetTimerIntr(time);
    return 0;
}
