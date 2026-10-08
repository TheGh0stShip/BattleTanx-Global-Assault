#include "ultra.h"

typedef struct {
    u32 type;
    u32 control;
} OSViMode;

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framebuffer;
    OSViMode *mode;
    u32 features;
} OSViContext;

extern OSViContext *__osViNext;

void osViSetMode(OSViMode *mode)
{
    register u32 saveMask;

    saveMask = __osDisableInt();
    __osViNext->mode = mode;
    __osViNext->state = 1;
    __osViNext->features = __osViNext->mode->control;
    __osRestoreInt(saveMask);
}
