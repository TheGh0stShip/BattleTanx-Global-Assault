/* Data reconstruction: decompals/ultralib e24c836796df4bf520ff8b11a5c9d2cea3a66cbd src/io/vi.c: __OSViContext vi[2] = {0}; __osViCurr = &vi[0]; __osViNext = &vi[1]; */
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
    u8 unused[0x20];
} OSViContext;

OSViContext vi_data_0000[2] = { 0 }; /* vi[2] */
OSViContext *__osViCurr = &vi_data_0000[0];
OSViContext *__osViNext = &vi_data_0000[1];
extern OSViMode osViModePalLan1;
extern OSViMode osViModeMpalLan1;
extern OSViMode osViModeNtscLan1;
extern u32 osTvType;
extern void __osViSwapContext(void);

void __osViInit(void)
{
    _bzero(vi_data_0000, sizeof(vi_data_0000));
    __osViCurr = &vi_data_0000[0];
    __osViNext = &vi_data_0000[1];
    __osViNext->retraceCount = 1;
    __osViCurr->retraceCount = 1;
    __osViNext->framebuffer = (void *)0x80000000;
    __osViCurr->framebuffer = (void *)0x80000000;

    if (osTvType == 0) {
        __osViNext->mode = &osViModePalLan1;
    } else if (osTvType == 2) {
        __osViNext->mode = &osViModeMpalLan1;
    } else {
        __osViNext->mode = &osViModeNtscLan1;
    }

    __osViNext->state = 0x20;
    __osViNext->features = __osViNext->mode->control;

    while (IO_READ(0xA4400010) > 10) {
    }
    IO_WRITE(0xA4400000, 0);
    __osViSwapContext();
}
