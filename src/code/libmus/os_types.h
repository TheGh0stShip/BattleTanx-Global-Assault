/* Minimal libultra OS layouts (N64 o32, big-endian) for the libmus units. */
#ifndef OS_TYPES_H
#define OS_TYPES_H

#include "ultra_basic_types.h"
typedef void *OSMesg;

typedef struct {
    void *mtqueue;             /* 0x00 */
    void *fullqueue;           /* 0x04 */
    s32 validCount;            /* 0x08 */
    s32 first;                 /* 0x0C */
    s32 msgCount;              /* 0x10 */
    OSMesg *msg;               /* 0x14 */
} OSMesgQueue;

typedef struct {
    u16 type;                  /* 0x00 */
    u8 pri;                    /* 0x02 */
    u8 status;                 /* 0x03 */
    OSMesgQueue *retQueue;     /* 0x04 */
} OSIoMesgHdr;

typedef struct {
    OSIoMesgHdr hdr;           /* 0x00 */
    void *dramAddr;            /* 0x08 */
    u32 devAddr;               /* 0x0C */
    u32 size;                  /* 0x10 */
    void *piHandle;            /* 0x14 */
} OSIoMesg;

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

typedef union {
    OSTask_t t;
    long long force_structure_alignment;
} OSTask;

typedef struct OSScTask_s {
    struct OSScTask_s *next;   /* 0x00 */
    u32 state;                 /* 0x04 */
    u32 flags;                 /* 0x08 */
    void *framebuffer;         /* 0x0C */
    OSTask list;               /* 0x10 */
    OSMesgQueue *msgQ;         /* 0x50 */
    OSMesg msg;                /* 0x54 */
    u64 startTime;             /* 0x58 */
    u64 totalTime;             /* 0x60 */
} OSScTask;

typedef struct {
    short type;
    char misc[30];
} OSScMsg;

typedef struct OSScClient_s {
    struct OSScClient_s *next;
    OSMesgQueue *msgQ;
} OSScClient;

extern void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
extern u32 osVirtualToPhysical(void *addr);

#endif
