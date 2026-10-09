/* Source adapted from the MIT-licensed Dr. Mario 64 libultra 2.0I. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef void *OSMesg;

typedef struct OSThread OSThread;
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
    s32 errStatus;
    void *dramAddr;
    u32 c2Addr;
    u32 sectorSize;
    u32 c1ErrNum;
    u32 c1ErrSector[4];
} OSBlockInfo;

typedef struct {
    s32 cmdType;
    u16 transferMode;
    u16 blockNum;
    s32 sectorNum;
    u32 devAddr;
    u32 bmCtlShadow;
    u32 seqCtlShadow;
    OSBlockInfo block[2];
} OSTranxInfo;

struct OSPiHandle {
    OSPiHandle *next;
    u8 type;
    u8 latency;
    u8 pageSize;
    u8 relDuration;
    u8 pulse;
    u8 domain;
    u16 pad;
    u32 baseAddress;
    u32 speed;
    OSTranxInfo transferInfo;
};

typedef struct {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
} OSIoMesgHdr;

typedef struct {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    OSPiHandle *piHandle;
} OSIoMesg;

typedef struct {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    s32 (*edma)(OSPiHandle *, s32, u32, void *, u32);
} OSDevMgr;

extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern void __osResetGlobalIntMask(u32);
extern void __osSetGlobalIntMask(u32);
extern s32 osEPiRawWriteIo(OSPiHandle *, u32, u32);
extern s32 osEPiRawReadIo(OSPiHandle *, u32, u32 *);
extern void osYieldThread(void);

#define IO_WRITE(addr, data) (*(volatile u32 *)(addr) = (u32)(data))

void __osDevMgrMain(void *args)
{
    OSIoMesg *mb;
    OSMesg em;
    OSMesg dummy;
    s32 ret;
    OSDevMgr *dm;
    s32 messageSend = 0;

    dm = (OSDevMgr *)args;
    mb = 0;
    ret = 0;

    while (1) {
        osRecvMesg(dm->cmdQueue, (OSMesg *)&mb, 1);

        if (mb->piHandle != 0 && mb->piHandle->type == 2 &&
            (mb->piHandle->transferInfo.cmdType == 0 ||
             mb->piHandle->transferInfo.cmdType == 1)) {
            OSBlockInfo *blockInfo;
            OSTranxInfo *info;
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];
            info->sectorNum = -1;

            if (info->transferMode != 3) {
                blockInfo->dramAddr =
                    (void *)((u32)blockInfo->dramAddr - blockInfo->sectorSize);
            }

            if (info->transferMode == 2 &&
                mb->piHandle->transferInfo.cmdType == 0) {
                messageSend = 1;
            } else {
                messageSend = 0;
            }

            osRecvMesg(dm->acsQueue, &dummy, 1);
            __osResetGlobalIntMask(0x00100401);
            osEPiRawWriteIo(mb->piHandle, 0x05000510,
                            info->bmCtlShadow | 0x80000000);

readblock1:
            osRecvMesg(dm->evtQueue, &em, 1);
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];

            if (blockInfo->errStatus == 29) {
                u32 stat;
                osEPiRawWriteIo(mb->piHandle, 0x05000510,
                                info->bmCtlShadow | 0x10000000);
                osEPiRawWriteIo(mb->piHandle, 0x05000510,
                                info->bmCtlShadow);
                osEPiRawReadIo(mb->piHandle, 0x05000508, &stat);

                if (stat & 0x02000000) {
                    osEPiRawWriteIo(mb->piHandle, 0x05000510,
                                    info->bmCtlShadow | 0x01000000);
                }

                blockInfo->errStatus = 4;
                IO_WRITE(0xA4600010, 2);
                __osSetGlobalIntMask(0x00100C01);
            }

            osSendMesg(mb->hdr.retQueue, mb, 0);

            if (messageSend == 1 &&
                mb->piHandle->transferInfo.block[0].errStatus == 0) {
                messageSend = 0;
                goto readblock1;
            }

            osSendMesg(dm->acsQueue, 0, 0);
            if (mb->piHandle->transferInfo.blockNum == 1) {
                osYieldThread();
            }
        } else {
            switch (mb->hdr.type) {
            case 11:
                osRecvMesg(dm->acsQueue, &dummy, 1);
                ret = dm->dma(0, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case 12:
                osRecvMesg(dm->acsQueue, &dummy, 1);
                ret = dm->dma(1, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case 15:
                osRecvMesg(dm->acsQueue, &dummy, 1);
                ret = dm->edma(mb->piHandle, 0, mb->devAddr, mb->dramAddr,
                               mb->size);
                break;
            case 16:
                osRecvMesg(dm->acsQueue, &dummy, 1);
                ret = dm->edma(mb->piHandle, 1, mb->devAddr, mb->dramAddr,
                               mb->size);
                break;
            case 10:
                osSendMesg(mb->hdr.retQueue, mb, 0);
                ret = -1;
                break;
            default:
                ret = -1;
                break;
            }

            if (ret == 0) {
                osRecvMesg(dm->evtQueue, &em, 1);
                osSendMesg(mb->hdr.retQueue, mb, 0);
                osSendMesg(dm->acsQueue, 0, 0);
            }
        }
    }
}
