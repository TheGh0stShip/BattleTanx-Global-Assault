#include "ultra.h"

typedef struct {
    u32 origin;
    u32 yScale;
    u32 vStart;
    u32 vBurst;
    u32 vIntr;
} OSViFieldRegsRaw;

typedef struct {
    u32 type;
    u32 control;
    u32 width;
    u32 burst;
    u32 vSync;
    u32 hSync;
    u32 leap;
    u32 hStart;
    u32 xScale;
    u32 vCurrent;
    OSViFieldRegsRaw field[2];
} OSViModeRaw;

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framebuffer;
    OSViModeRaw *mode;
    u32 features;
    OSMesgQueue *messageQueue;
    OSMesg message;
    u32 unk18;
    u32 unk1C;
    u32 xScale;
    f32 yScaleFactor;
    u16 yOffset;
    u16 pad2A;
    u32 yScale;
} OSViContextRaw;

extern OSViContextRaw *__osViNext;
extern OSViContextRaw *__osViCurr;
extern u32 osVirtualToPhysical(void *address);

void __osViSwapContext(void)
{
    register OSViModeRaw *mode;
    register OSViContextRaw *context;
    u32 origin;
    u32 hStart;
    u32 scale;
    u32 field;

    field = 0;
    context = __osViNext;
    mode = context->mode;
    field = IO_READ(0xA4400010) & 1;
    origin = osVirtualToPhysical(context->framebuffer) +
             mode->field[field].origin;
    if (context->state & 2) {
        context->xScale |= mode->xScale & ~0xFFF;
    } else {
        context->xScale = mode->xScale;
    }
    if (context->state & 4) {
        scale = mode->field[field].yScale & 0xFFF;
        context->yScale = context->yScaleFactor * scale;
        context->yScale |= mode->field[field].yScale & ~0xFFF;
    } else {
        context->yScale = mode->field[field].yScale;
    }
    hStart = mode->hStart;
    if (context->state & 0x20) {
        hStart = 0;
    }
    if (context->state & 0x40) {
        context->yScale = 0;
        origin = osVirtualToPhysical(context->framebuffer);
    }
    if (context->state & 0x80) {
        context->yScale = (context->yOffset << 16) & 0x03FF0000;
        origin = osVirtualToPhysical(context->framebuffer);
    }
    IO_WRITE(0xA4400004, origin);
    IO_WRITE(0xA4400008, mode->width);
    IO_WRITE(0xA4400014, mode->burst);
    IO_WRITE(0xA4400018, mode->vSync);
    IO_WRITE(0xA440001C, mode->hSync);
    IO_WRITE(0xA4400020, mode->leap);
    IO_WRITE(0xA4400024, hStart);
    IO_WRITE(0xA4400028, mode->field[field].vStart);
    IO_WRITE(0xA440002C, mode->field[field].vBurst);
    IO_WRITE(0xA440000C, mode->field[field].vIntr);
    IO_WRITE(0xA4400030, context->xScale);
    IO_WRITE(0xA4400034, context->yScale);
    IO_WRITE(0xA4400000, context->features);
    __osViNext = __osViCurr;
    __osViCurr = context;
    *__osViNext = *__osViCurr;
}
