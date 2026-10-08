#include "ultra.h"

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framebuffer;
} OSViContext;

extern OSViContext *__osViCurr;

void *osViGetCurrentFramebuffer(void)
{
    register u32 saveMask;
    void *framebuffer;

    saveMask = __osDisableInt();
    framebuffer = __osViCurr->framebuffer;
    __osRestoreInt(saveMask);
    return framebuffer;
}
