#include "os_thread.h"

#define MQ_IS_EMPTY(mq) ((mq)->validCount == 0)

extern void __osEnqueueAndYield(OSThread **queue);
extern OSThread *__osPopThread(OSThread **queue);
extern void osStartThread(OSThread *t);

s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag)
{
    register u32 saveMask;

    saveMask = __osDisableInt();
    while (MQ_IS_EMPTY(mq)) {
        if (flag == OS_MESG_NOBLOCK) {
            __osRestoreInt(saveMask);
            return -1;
        }
        __osRunningThread->state = OS_STATE_WAITING;
        __osEnqueueAndYield((OSThread **)&mq->mtqueue);
    }
    if (msg != 0)
        *msg = mq->msg[mq->first];
    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;
    if (((OSThread *)mq->fullqueue)->next != 0)
        osStartThread(__osPopThread((OSThread **)&mq->fullqueue));
    __osRestoreInt(saveMask);
    return 0;
}
