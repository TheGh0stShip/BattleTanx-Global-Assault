/* Minimal libultra 2.0I declarations required to reproduce sched.o. */
#ifndef SCHED_COMPAT_H
#define SCHED_COMPAT_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef signed long s32;
typedef void *OSMesg;
typedef s32 OSPri;
typedef u32 OSIntMask;

typedef struct OSThread { u8 opaque[0x1B0]; } OSThread;
typedef struct OSMesgQueue {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct {
    u32 type;
    u32 flags;
    u64 *ucode_boot;
    u32 ucode_boot_size;
    u64 *ucode;
    u32 ucode_size;
    u64 *ucode_data;
    u32 ucode_data_size;
    u64 *dram_stack;
    u32 dram_stack_size;
    u64 *output_buff;
    u64 *output_buff_size;
    u64 *data_ptr;
    u32 data_size;
    u64 *yield_data_ptr;
    u32 yield_data_size;
} OSTask_t;
typedef union { OSTask_t t; long long force_structure_alignment; } OSTask;
typedef struct { short type; char misc[30]; } OSScMsg;
typedef struct OSScTask_s {
    struct OSScTask_s *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    OSTask list;
    OSMesgQueue *msgQ;
    OSMesg msg;
} OSScTask;
typedef struct SCClient_s { struct SCClient_s *next; OSMesgQueue *msgQ; } OSScClient;
typedef struct {
    OSScMsg retraceMsg;
    OSScMsg prenmiMsg;
    OSMesgQueue interruptQ;
    OSMesg intBuf[8];
    OSMesgQueue cmdQ;
    OSMesg cmdMsgBuf[8];
    OSThread thread;
    OSScClient *clientList;
    OSScTask *audioListHead;
    OSScTask *gfxListHead;
    OSScTask *audioListTail;
    OSScTask *gfxListTail;
    OSScTask *curRSPTask;
    OSScTask *curRDPTask;
    u32 frameCount;
    s32 doAudio;
} OSSched;
typedef struct { u8 opaque[0x50]; } OSViMode;
extern OSViMode osViModeTable[];
#define OS_SC_RETRACE_MSG 1
#define OS_SC_PRE_NMI_MSG 4
#define OS_SC_MAX_MESGS 8
#define OS_SC_NEEDS_RDP 0x0001
#define OS_SC_NEEDS_RSP 0x0002
#define OS_SC_DRAM_DLIST 0x0004
#define OS_SC_PARALLEL_TASK 0x0010
#define OS_SC_LAST_TASK 0x0020
#define OS_SC_SWAPBUFFER 0x0040
#define OS_SC_RCP_MASK 0x0003
#define OS_SC_TYPE_MASK 0x0007
#define OS_PRIORITY_VIMGR 254
#define OS_EVENT_SP 4
#define OS_EVENT_DP 9
#define OS_EVENT_PRENMI 14
#define OS_IM_NONE 1
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define M_GFXTASK 1
#define M_AUDTASK 2
#define TRUE 1
#define FALSE 0
#define NULL 0
#define assert(x) ((void)0)
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern void osCreateViManager(OSPri);
extern void osViSetMode(OSViMode *);
extern void osViBlack(s32);
extern void osSetEventMesg(s32, OSMesgQueue *, OSMesg);
extern void osViSetEvent(OSMesgQueue *, OSMesg, u32);
extern void osCreateThread(OSThread *, s32, void (*)(void *), void *, void *, OSPri);
extern void osStartThread(OSThread *);
extern OSIntMask osSetIntMask(OSIntMask);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osSpTaskYielded(OSTask *);
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetNextFramebuffer(void);
extern void osViSwapBuffer(void *);
extern void osWritebackDCacheAll(void);
extern void osSpTaskLoad(OSTask *);
extern void osSpTaskStartGo(OSTask *);
extern s32 osDpSetNextBuffer(void *, u64);
extern void osSpTaskYield(void);
#endif
