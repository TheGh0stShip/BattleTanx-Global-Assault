#include "os_thread.h"

#define MQ_IS_FULL(mq) ((mq)->validCount >= (mq)->msgCount)

extern void __osEnqueueAndYield(OSThread **queue);
extern OSThread *__osPopThread(OSThread **queue);
extern void osStartThread(OSThread *t);

s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag)
{
    register u32 saveMask;
    register s32 index;

    saveMask = __osDisableInt();
    while (MQ_IS_FULL(mq)) {
        if (flag == OS_MESG_BLOCK) {
            __osRunningThread->state = OS_STATE_WAITING;
            __osEnqueueAndYield((OSThread **)&mq->fullqueue);
        } else {
            __osRestoreInt(saveMask);
            return -1;
        }
    }
    index = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[index] = msg;
    mq->validCount++;
    if (((OSThread *)mq->mtqueue)->next != 0)
        osStartThread(__osPopThread((OSThread **)&mq->mtqueue));
    __osRestoreInt(saveMask);
    return 0;
}
