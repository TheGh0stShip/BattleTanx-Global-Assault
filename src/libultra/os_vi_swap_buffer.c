#include "ultra.h"

#define VI_STATE_BUFFER 0x10

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framebuffer;
} OSViContext;

extern OSViContext *__osViNext;

void osViSwapBuffer(void *framebuffer)
{
    u32 saveMask;

    saveMask = __osDisableInt();
    __osViNext->framebuffer = framebuffer;
    __osViNext->state |= VI_STATE_BUFFER;
    __osRestoreInt(saveMask);
}
