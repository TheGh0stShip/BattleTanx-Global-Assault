#include "ultra.h"

#define VI_STATE_BLACK 0x20

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framebuffer;
} OSViContext;

extern OSViContext *__osViNext;

void osViBlack(u8 active)
{
    register u32 saveMask;

    saveMask = __osDisableInt();
    if (active)
        __osViNext->state |= VI_STATE_BLACK;
    else
        __osViNext->state &= ~VI_STATE_BLACK;
    __osRestoreInt(saveMask);
}
