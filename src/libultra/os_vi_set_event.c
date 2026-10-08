#include "ultra.h"

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framebuffer;
    void *mode;
    u32 features;
    OSMesgQueue *messageQueue;
    OSMesg message;
} OSViContext;

extern OSViContext *__osViNext;

void osViSetEvent(OSMesgQueue *mq, OSMesg msg, u32 retraceCount)
{
    register u32 saveMask;

    saveMask = __osDisableInt();
    __osViNext->messageQueue = mq;
    __osViNext->message = msg;
    __osViNext->retraceCount = retraceCount;
    __osRestoreInt(saveMask);
}
