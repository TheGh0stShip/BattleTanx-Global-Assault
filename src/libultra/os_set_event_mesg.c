#include "ultra.h"

typedef struct {
    OSMesgQueue *queue;
    OSMesg msg;
} OSEventState;

extern OSEventState D_803B0470[];

void osSetEventMesg(s32 event, OSMesgQueue *mq, OSMesg msg)
{
    register u32 saveMask;
    OSEventState *state;

    saveMask = __osDisableInt();
    state = &D_803B0470[event];
    state->queue = mq;
    state->msg = msg;
    __osRestoreInt(saveMask);
}
