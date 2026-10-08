#include "os_thread.h"

typedef struct {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
    u8 rest[0x10];
} OSIoMesgRaw;

typedef struct {
    u32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *eventQueue;
    u32 unk10;
    u32 unk14;
    u32 unk18;
} OSViManagerRaw;

/* DATA_VRAM 0x80126F80 */
/* BSS_VRAM 0x803B0590 */
OSViManagerRaw D_80126F80 = { 0 };
OSThread D_803B0590;
u64 D_803B0740[0x200];
OSMesgQueue D_803B1740;
OSMesg D_803B1758[5];
u32 viEventPadding;
OSIoMesgRaw D_803B1770;
OSIoMesgRaw D_803B1788;
u16 D_803B17A0;

extern void __osTimerServicesInit(void);
extern OSPri osGetThreadPri(OSThread *thread);
extern void osSetThreadPri(OSThread *thread, OSPri pri);
extern void osStartThread(OSThread *thread);
extern void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *),
                           void *arg, void *sp, OSPri pri);
extern void osSetEventMesg(u32 event, OSMesgQueue *mq, OSMesg msg);
extern void __osViInit(void);
extern void viMgrMain(void *arg);
extern void *__osViGetCurrentContext(void);
extern void __osViSwapContext(void);
extern void __osTimerInterrupt(void);
extern u32 osGetCount(void);

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framebuffer;
    void *mode;
    u32 features;
    OSMesgQueue *messageQueue;
    OSMesg message;
} OSViContextRaw;

extern u64 D_803B0570;
extern u32 D_803B0578;
extern u32 D_803B057C;

void osCreateViManager(OSPri pri)
{
    u32 saveMask;
    OSPri oldPri;
    OSPri myPri;

    if (D_80126F80.active != 0) {
        return;
    }

    __osTimerServicesInit();
    osCreateMesgQueue(&D_803B1740, D_803B1758, 5);

    D_803B1770.type = 13;
    D_803B1770.pri = 0;
    D_803B1770.retQueue = 0;
    D_803B1788.type = 14;
    D_803B1788.pri = 0;
    D_803B1788.retQueue = 0;

    osSetEventMesg(7, &D_803B1740, &D_803B1770);
    osSetEventMesg(3, &D_803B1740, &D_803B1788);

    oldPri = -1;
    myPri = osGetThreadPri(0);
    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(0, pri);
    }

    saveMask = __osDisableInt();
    D_80126F80.active = 1;
    D_80126F80.thread = &D_803B0590;
    D_80126F80.cmdQueue = &D_803B1740;
    D_80126F80.eventQueue = &D_803B1740;
    D_80126F80.unk10 = 0;
    D_80126F80.unk14 = 0;
    D_80126F80.unk18 = 0;
    osCreateThread(&D_803B0590, 0, viMgrMain, &D_80126F80,
                   (u8 *)D_803B0740 + 0x1000, pri);
    __osViInit();
    osStartThread(&D_803B0590);
    __osRestoreInt(saveMask);

    if (oldPri != -1) {
        osSetThreadPri(0, oldPri);
    }
}

void viMgrMain(void *arg)
{
    OSViContextRaw *context;
    OSViManagerRaw *manager;
    OSMesg msg;
    u32 resetTime;
    u32 count;

    msg = 0;
    resetTime = 0;
    context = __osViGetCurrentContext();
    D_803B17A0 = context->retraceCount;
    if (D_803B17A0 == 0) {
        D_803B17A0 = 1;
    }
    manager = arg;

    while (1) {
        osRecvMesg(manager->eventQueue, &msg, OS_MESG_BLOCK);
        switch (((OSIoMesgRaw *)msg)->type) {
        case 13:
            __osViSwapContext();
            if (--D_803B17A0 == 0) {
                context = __osViGetCurrentContext();
                if (context->messageQueue != 0) {
                    osSendMesg(context->messageQueue, context->message,
                               OS_MESG_NOBLOCK);
                }
                D_803B17A0 = context->retraceCount;
            }

            D_803B057C++;
            if (resetTime != 0) {
                count = osGetCount();
                D_803B0570 = count;
                resetTime = 0;
            }
            count = D_803B0578;
            D_803B0578 = osGetCount();
            count = D_803B0578 - count;
            D_803B0570 = D_803B0570 + count;
            break;
        case 14:
            __osTimerInterrupt();
            break;
        }
    }
}
