#include "os_types.h"

typedef struct dma_buf_s {
    struct dma_buf_s *prev;    /* 0x00 */
    struct dma_buf_s *next;    /* 0x04 */
    s32 frames;                /* 0x08 */
    u32 startAddr;             /* 0x0C */
    u8 *ptr;                   /* 0x10 */
} dma_buf_t;

typedef struct {
    OSScClient client;         /* 0x00 */
    OSMesgQueue frameQ;        /* 0x08 */
    OSMesg frameMsgs[4];       /* 0x20 */
    OSMesgQueue taskQ;         /* 0x30 */
    OSMesg taskMsgs[4];        /* 0x48 */
} sched_client_t;

extern dma_buf_t *D_803ADA10;
extern dma_buf_t *D_803ADA14;
extern dma_buf_t *D_803ADA18;
extern OSIoMesg *D_803ADA1C;
extern OSMesg *D_803ADA20;
extern s32 D_803ADA24;
extern s32 D_803ADA28;
extern OSMesgQueue D_803ADA30;
extern void *D_803ADA48;
extern void *D_803ADA50;
extern void *D_803ADA60;
extern sched_client_t *D_803ADA64;
extern u32 D_803AD9D0;
extern u64 rspbootTextStart[];
extern u64 D_800F8E80[];

extern void *osCartRomInit(void);
extern void *__MusIntMemMalloc(s32 size);
extern void func_800FF9F0(void *p, s32 value, s32 len);
extern s32 osEPiStartDma(void *pihandle, OSIoMesg *mb, s32 direction);
extern OSMesgQueue *osScGetCmdQ(void *sc);
extern void osScAddClient(void *sc, OSScClient *c, OSMesgQueue *msgQ);

void *func_800FF1A4(void *state);
u32 func_800FF1B0(u32 addr, s32 len, void *state);
dma_buf_t *func_800FF21C(u32 addr, s32 len);

void *func_800FEF60(s32 count, s32 size)
{
    s32 i;
    dma_buf_t *dma;

    D_803ADA48 = osCartRomInit();
    D_803ADA1C = __MusIntMemMalloc(count * 2 * sizeof(OSIoMesg));
    D_803ADA20 = __MusIntMemMalloc(count * 2 * sizeof(OSMesg));
    D_803ADA18 = __MusIntMemMalloc(count * sizeof(dma_buf_t));
    func_800FF9F0(D_803ADA18, 0, count * sizeof(dma_buf_t));
    for (i = 0; i < count - 1; i++) {
        D_803ADA18[i].next = &D_803ADA18[i] + 1;
        (&D_803ADA18[i] + 1)->prev = &D_803ADA18[i];
        D_803ADA18[i].ptr = __MusIntMemMalloc(size);
        D_803ADA18[i].startAddr = -1;
    }
    D_803ADA18[i].ptr = __MusIntMemMalloc(size);
    D_803ADA18[i].startAddr = -1;
    D_803ADA24 = size;
    D_803ADA28 = 0;
    D_803ADA10 = 0;
    D_803ADA14 = D_803ADA18;
    osCreateMesgQueue(&D_803ADA30, D_803ADA20, count * 2);
    return func_800FF1A4;
}

void func_800FF0D4(void)
{
    OSMesg msg;
    dma_buf_t *dma;
    dma_buf_t *next;

    while (D_803ADA28) {
        osRecvMesg(&D_803ADA30, &msg, 0);
        D_803ADA28--;
    }
    dma = D_803ADA10;
    while (dma) {
        if (--dma->frames == 0) {
            next = dma->next;
            if (next)
                next->prev = dma->prev;
            if (dma->prev)
                dma->prev->next = dma->next;
            else
                D_803ADA10 = dma->next;
            dma->prev = 0;
            dma->next = D_803ADA14;
            D_803ADA14 = dma;
            dma = next;
        } else {
            dma = dma->next;
        }
    }
}

void *func_800FF1A4(void *state)
{
    return func_800FF1B0;
}

u32 func_800FF1B0(u32 addr, s32 len, void *state)
{
    dma_buf_t *dma;

    dma = func_800FF21C(addr, len);
    if (!dma)
        return osVirtualToPhysical((void *)addr);
    if ((addr & 0xFF000000) == 0xFF000000) {
        addr &= 0xFFFFFF;
        addr += 0x140000;
    }
    return osVirtualToPhysical(dma->ptr + addr - dma->startAddr);
}

dma_buf_t *func_800FF21C(u32 addr, s32 len)
{
    void *handle;
    dma_buf_t *dma;
    dma_buf_t *last;
    dma_buf_t *buf;
    u32 end;
    OSIoMesg *mb;

    if ((addr & 0xFF000000) == 0xFF000000) {
        handle = D_803ADA50;
        addr &= 0xFFFFFF;
        addr += 0x140000;
    } else {
        if (D_803AD9D0 & 1)
            return 0;
        handle = D_803ADA48;
    }
    dma = D_803ADA10;
    last = 0;
    end = addr + len;
    while (dma) {
        if (addr < dma->startAddr)
            break;
        if (end <= dma->startAddr + D_803ADA24)
            return dma;
        last = dma;
        dma = dma->next;
    }
    buf = D_803ADA14;
    if (!buf)
        return D_803ADA10;
    D_803ADA14 = buf->next;
    if (last) {
        buf->next = last->next;
        if (buf->next)
            buf->next->prev = buf;
        buf->prev = last;
        last->next = buf;
    } else {
        buf->prev = 0;
        buf->next = D_803ADA10;
        if (buf->next)
            buf->next->prev = buf;
        D_803ADA10 = buf;
    }
    buf->startAddr = addr & ~1;
    buf->frames = 2;
    mb = &D_803ADA1C[D_803ADA28++];
    mb->hdr.pri = 0;
    mb->hdr.retQueue = &D_803ADA30;
    mb->dramAddr = buf->ptr;
    mb->devAddr = buf->startAddr;
    mb->size = D_803ADA24;
    osEPiStartDma(handle, mb, 0);
    return buf;
}

void func_800FF3B0(void *sc)
{
    D_803ADA60 = sc;
}

void __OsSchedInstall(void)
{
    D_803ADA64 = __MusIntMemMalloc(sizeof(sched_client_t));
    osCreateMesgQueue(&D_803ADA64->frameQ, D_803ADA64->frameMsgs, 4);
    osCreateMesgQueue(&D_803ADA64->taskQ, D_803ADA64->taskMsgs, 4);
    osScAddClient(D_803ADA60, &D_803ADA64->client, &D_803ADA64->frameQ);
}

void func_800FF420(void)
{
    OSScMsg *msg;

    do {
        osRecvMesg(&D_803ADA64->frameQ, (OSMesg *)&msg, 1);
        osRecvMesg(&D_803ADA64->frameQ, 0, 0);
    } while (msg->type != 1);
}

void func_800FF480(u32 *info)
{
    OSScTask t;
    OSScMsg msg;

    t.next = 0;
    t.msgQ = &D_803ADA64->taskQ;
    t.msg = &msg;
    t.flags = 2;
    t.list.t.data_ptr = (u64 *)info[0];
    t.list.t.data_size = info[1];
    t.list.t.type = 2;
    t.list.t.ucode_boot = rspbootTextStart;
    t.list.t.ucode_boot_size = (u8 *)D_800F8E80 - (u8 *)rspbootTextStart;
    t.list.t.flags = 0;
    t.list.t.ucode = (u64 *)info[2];
    t.list.t.ucode_data = (u64 *)info[3];
    t.list.t.ucode_size = 0x1000;
    t.list.t.ucode_data_size = 0x800;
    t.list.t.dram_stack = 0;
    t.list.t.dram_stack_size = 0;
    t.list.t.output_buff = 0;
    t.list.t.output_buff_size = 0;
    t.list.t.yield_data_ptr = 0;
    t.list.t.yield_data_size = 0;
    osSendMesg(osScGetCmdQ(D_803ADA60), (OSMesg)&t, 1);
    osRecvMesg(&D_803ADA64->taskQ, 0, 1);
}
