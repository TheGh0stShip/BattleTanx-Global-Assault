/* Data reconstruction: decompals/ultralib e24c836796df4bf520ff8b11a5c9d2cea3a66cbd src/os/timerintr.c: OSTimer* __osTimerList = &__osBaseTimer; */
#include "ultra.h"

typedef union {
    OSTime time;
    u32 words[2];
} OSTimeValue;

/* BSS_VRAM 0x803B0570 */
OSTimeValue D_803B0570;
u32 D_803B0578;
u32 D_803B057C;
u32 D_803B0580;

extern OSTimer D_803B0550; /* __osBaseTimer, this unit's bss: LDSYM D_803B0550=0x803B0550 */
OSTimer *__osTimerList = &D_803B0550;
extern u32 osGetCount(void);
extern void __osSetCompare(u32 value);
extern void __osSetTimerIntr(OSTime time);
extern OSTime __osInsertTimer(OSTimer *timer);

void __osTimerServicesInit(void)
{
    D_803B0570.time = 0;
    D_803B0578 = 0;
    D_803B057C = 0;
    __osTimerList->prev = __osTimerList;
    __osTimerList->next = __osTimerList->prev;
    __osTimerList->value = 0;
    __osTimerList->interval = __osTimerList->value;
    __osTimerList->mq = 0;
    __osTimerList->msg = 0;
}

void __osTimerInterrupt(void)
{
    OSTimer *timer;
    u32 count;
    u32 elapsed;

    if (__osTimerList->next == __osTimerList) {
        return;
    }

    for (;;) {
        timer = __osTimerList->next;
        if (timer == __osTimerList) {
            __osSetCompare(0);
            D_803B0580 = 0;
            break;
        }
        count = osGetCount();
        elapsed = count - D_803B0580;
        D_803B0580 = count;
        if (elapsed < timer->value) {
            timer->value -= elapsed;
            __osSetTimerIntr(timer->value);
            break;
        }
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        timer->next = 0;
        timer->prev = 0;
        if (timer->mq != 0) {
            osSendMesg(timer->mq, timer->msg, OS_MESG_NOBLOCK);
        }
        if (timer->interval != 0) {
            timer->value = timer->interval;
            __osInsertTimer(timer);
        }
    }
}

void __osSetTimerIntr(OSTime time)
{
    OSTime compare;
    s32 saveMask;

    saveMask = __osDisableInt();
    D_803B0580 = osGetCount();
    compare = D_803B0580 + time;
    __osSetCompare(compare);
    __osRestoreInt(saveMask);
}

OSTime __osInsertTimer(OSTimer *timer)
{
    OSTimer *current;
    OSTime time;
    s32 saveMask;

    saveMask = __osDisableInt();
    for (current = __osTimerList->next, time = timer->value;
         current != __osTimerList && time > current->value;
         time -= current->value, current = current->next) {
    }
    timer->value = time;
    if (current != __osTimerList) {
        current->value -= time;
    }
    timer->next = current;
    timer->prev = current->prev;
    current->prev->next = timer;
    current->prev = timer;
    __osRestoreInt(saveMask);
    return time;
}
