#include "ultra.h"

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framebuffer;
} OSViContext;

extern OSViContext *__osViNext;

void *osViGetNextFramebuffer(void)
{
    register u32 saveMask;
    void *framebuffer;

    saveMask = __osDisableInt();
    framebuffer = __osViNext->framebuffer;
    __osRestoreInt(saveMask);
    return framebuffer;
}
