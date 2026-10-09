/* Source shape adapted from the MIT-licensed Dr. Mario 64 libultra 2.0I. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
typedef void *OSMesg;
typedef unsigned long long u64;

typedef struct OSThread {
    u8 opaque[0x1B0];
} OSThread;
typedef struct OSPiHandle OSPiHandle;

typedef struct {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

typedef struct {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    s32 (*edma)(OSPiHandle *, s32, u32, void *, u32);
} OSDevMgr;

static OSThread piThread;
static u64 piThreadStack[0x200];
static OSMesgQueue piEventQueue;
static OSMesg piEventBuf[1];

/*
 * Keep these definitions local to this reconstruction unit.  The unsplit
 * data assembly still exports the SDK names for the other libultra units;
 * unit_data.tsv places this unit's identical bytes over that ROM range.
 */
static OSDevMgr piDevMgr = { 0 };
static OSPiHandle *piTable = 0;
extern OSPiHandle CartRomHandle;
extern OSPiHandle LeoDiskHandle;
static OSPiHandle *currentHandle[2] = { &CartRomHandle, &LeoDiskHandle };

extern s32 __osPiAccessQueueEnabled;
extern OSMesgQueue __osPiAccessQueue;

extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern void __osPiCreateAccessQueue(void);
extern void osSetEventMesg(u32, OSMesgQueue *, OSMesg);
extern s32 osGetThreadPri(OSThread *);
extern void osSetThreadPri(OSThread *, s32);
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern s32 osPiRawStartDma(s32, u32, void *, u32);
extern s32 osEPiRawStartDma(OSPiHandle *, s32, u32, void *, u32);
extern void __osDevMgrMain(void *);
extern void osCreateThread(OSThread *, s32, void (*)(void *), void *, void *, s32);
extern void osStartThread(OSThread *);

void osCreatePiManager(s32 pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt)
{
    u32 savedMask;
    s32 oldPri;
    s32 myPri;

    if (piDevMgr.active)
        return;
    osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
    osCreateMesgQueue(&piEventQueue, piEventBuf, 1);

    if (!__osPiAccessQueueEnabled)
        __osPiCreateAccessQueue();

    osSetEventMesg(8, &piEventQueue, (OSMesg)0x22222222);
    oldPri = -1;
    myPri = osGetThreadPri(0);

    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(0, pri);
    }

    savedMask = __osDisableInt();
    piDevMgr.active = 1;
    piDevMgr.thread = &piThread;
    piDevMgr.cmdQueue = cmdQ;
    piDevMgr.evtQueue = &piEventQueue;
    piDevMgr.acsQueue = &__osPiAccessQueue;
    piDevMgr.dma = osPiRawStartDma;
    piDevMgr.edma = osEPiRawStartDma;
    osCreateThread(&piThread, 0, __osDevMgrMain, &piDevMgr,
                   (u8 *)piThreadStack + sizeof(piThreadStack), pri);
    osStartThread(&piThread);
    __osRestoreInt(savedMask);

    if (oldPri != -1)
        osSetThreadPri(0, oldPri);
}
