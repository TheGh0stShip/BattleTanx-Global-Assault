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

void osViSetSpecialFeatures(u32 func)
{
    register u32 saveMask;

    saveMask = __osDisableInt();

    if (func & 0x01) {
        __osViNext->features |= 0x08;
    }
    if (func & 0x02) {
        __osViNext->features &= ~0x08;
    }
    if (func & 0x04) {
        __osViNext->features |= 0x04;
    }
    if (func & 0x08) {
        __osViNext->features &= ~0x04;
    }
    if (func & 0x10) {
        __osViNext->features |= 0x10;
    }
    if (func & 0x20) {
        __osViNext->features &= ~0x10;
    }
    if (func & 0x40) {
        __osViNext->features |= 0x10000;
        __osViNext->features &= ~0x300;
    }
    if (func & 0x80) {
        __osViNext->features &= ~0x10000;
        __osViNext->features |= __osViNext->mode->control & 0x300;
    }

    __osViNext->state |= 0x08;
    __osRestoreInt(saveMask);
}
