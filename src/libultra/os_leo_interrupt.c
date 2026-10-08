/* IDOFLAGS: -O1 -mips2 -Xcpluscomm
   BSS_VRAM 0x803ADF70 */
#include "ultra.h"

#define OS_PIM_STACKSIZE 4096
#define NULL ((void *)0)
#define OS_EVENT_PI 8
#define PI_STATUS_REG 0xA4600010
#define PI_STATUS_DMA_BUSY 1
#define PI_STATUS_IO_BUSY 2
#define PI_STATUS_CLR_INTR 2
#define SR_IBIT4 0x00000800
#define OS_IM_PI 0x00100401
#define LEO_BASE_REG 0x05000000
#define LEO_STATUS (LEO_BASE_REG + 0x508)
#define LEO_BM_CTL (LEO_BASE_REG + 0x510)
#define LEO_BM_STATUS (LEO_BASE_REG + 0x510)
#define LEO_C2_BUFF (LEO_BASE_REG + 0x000)
#define LEO_SECTOR_BUFF (LEO_BASE_REG + 0x400)
#define LEO_STATUS_DATA_REQUEST 0x40000000
#define LEO_STATUS_C2_TRANSFER 0x10000000
#define LEO_STATUS_BUFFER_MANAGER_ERROR 0x08000000
#define LEO_STATUS_MECHANIC_INTERRUPT 0x02000000
#define LEO_BM_STATUS_MICRO 0x02000000
#define LEO_BM_STATUS_C1DOUBLE 0x00400000
#define LEO_BM_STATUS_C1SINGLE 0x00200000
#define LEO_BM_CTL_RESET 0x10000000
#define LEO_BM_CTL_CLR_MECHANIC_INTR 0x01000000
#define LEO_CMD_TYPE_0 0
#define LEO_CMD_TYPE_1 1
#define LEO_CMD_TYPE_2 2
#define LEO_ERROR_GOOD 0
#define LEO_ERROR_4 4
#define LEO_ERROR_22 22
#define LEO_ERROR_23 23
#define LEO_ERROR_24 24
#define LEO_ERROR_29 29
#define LEO_TRACK_MODE 2
#define LEO_SECTOR_MODE 3
#define ALIGNED(x)
#define MQ_IS_FULL(mq) ((mq)->validCount >= (mq)->msgCount)

#undef IO_READ
#undef IO_WRITE
#define IO_READ(addr) (*(volatile u32 *)((u32)(addr) | 0xA0000000))
#define IO_WRITE(addr, data) (*(volatile u32 *)((u32)(addr) | 0xA0000000) = (u32)(data))
#define WAIT_ON_IOBUSY(stat) { \
    stat = IO_READ(PI_STATUS_REG); \
    while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) \
        stat = IO_READ(PI_STATUS_REG); \
} (void)0

typedef struct {
    OSMesgQueue *messageQueue;
    OSMesg message;
} __OSEventState;

typedef struct LeoThread_s {
    struct LeoThread_s *next;
} LeoThread;

extern u32 __OSGlobalIntMask;
extern __OSEventState D_803B0470[];
extern void *__osRunQueue;
extern s32 osEPiRawStartDma(OSPiHandle *, s32, u32, void *, u32);
extern void __osEnqueueThread(void **, void *);
extern void *__osPopThread(void **);

#define __osEventStateTab D_803B0470
#define __osEPiRawStartDma osEPiRawStartDma

extern OSPiHandle *__osDiskHandle;

u8 leoDiskStack[OS_PIM_STACKSIZE] ALIGNED(0x10);

void __osLeoAbnormalResume(void);
void __osLeoResume(void);

s32 __osLeoInterrupt(void) {
    u32 stat = 0;
    volatile u32 pi_stat;
    u32 bm_stat;
    __OSTranxInfo *info = &__osDiskHandle->transferInfo;
    __OSBlockInfo *blockInfo = &info->block[info->blockNum];

    pi_stat = IO_READ(PI_STATUS_REG);
    if (pi_stat & PI_STATUS_DMA_BUSY) {
        __OSGlobalIntMask = __OSGlobalIntMask & ~SR_IBIT4; //cart interrupt
        blockInfo->errStatus = LEO_ERROR_29;
        __osLeoResume();
        return 1;
    }

    WAIT_ON_IOBUSY(pi_stat);
    stat = IO_READ(LEO_STATUS);
    if (stat & LEO_STATUS_MECHANIC_INTERRUPT) {
        WAIT_ON_IOBUSY(pi_stat);
        IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_CLR_MECHANIC_INTR);
        blockInfo->errStatus = LEO_ERROR_GOOD;
        return 0;
    }

    if (info->cmdType == LEO_CMD_TYPE_2) {
        return 1;
    }

    if (stat & LEO_STATUS_BUFFER_MANAGER_ERROR) {
        WAIT_ON_IOBUSY(pi_stat);
        stat = IO_READ(LEO_STATUS);
        blockInfo->errStatus = LEO_ERROR_22;
        __osLeoResume();
        IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
        __OSGlobalIntMask |= OS_IM_PI;
        return 1;
    }

    if (info->cmdType == LEO_CMD_TYPE_1) {
        if ((stat & LEO_STATUS_DATA_REQUEST) == 0) {
            if (info->sectorNum + 1 != info->transferMode * 85) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
                return 1;
            }

            IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
            __OSGlobalIntMask |= OS_IM_PI;
            blockInfo->errStatus = LEO_ERROR_GOOD;
            __osLeoResume();
            return 1;
        } else {
            blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr + blockInfo->sectorSize);
            info->sectorNum++;
            __osEPiRawStartDma(__osDiskHandle, OS_WRITE, LEO_SECTOR_BUFF, blockInfo->dramAddr, blockInfo->sectorSize);
            return 1;
        }
    } else if (info->cmdType == LEO_CMD_TYPE_0) {
        if (info->transferMode == LEO_SECTOR_MODE) {
            if (info->sectorNum > (s32)blockInfo->C1ErrNum + 17) {
                blockInfo->errStatus = LEO_ERROR_GOOD;
                __osLeoAbnormalResume();
                return 1;
            }

            if ((stat & LEO_STATUS_DATA_REQUEST) == 0) {
                blockInfo->errStatus = LEO_ERROR_23;
                __osLeoAbnormalResume();
                return 1;
            }
        } else {
            blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr + blockInfo->sectorSize);
        }

        bm_stat = IO_READ(LEO_BM_STATUS);
        if ((bm_stat & LEO_BM_STATUS_C1SINGLE && bm_stat & LEO_BM_STATUS_C1DOUBLE) || bm_stat & LEO_BM_STATUS_MICRO) {
            if (blockInfo->C1ErrNum > 3) {
                if (info->transferMode != LEO_SECTOR_MODE || info->sectorNum > 0x52) {
                    blockInfo->errStatus = LEO_ERROR_23;
                    __osLeoAbnormalResume();
                    return 1;
                }
            } else {
                int errNum = blockInfo->C1ErrNum;
                blockInfo->C1ErrSector[errNum] = info->sectorNum + 1;
            }

            blockInfo->C1ErrNum++;
        }

        if (stat & LEO_STATUS_C2_TRANSFER) {
            if (info->sectorNum + 1 != 88) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
            }

            if (info->transferMode == LEO_TRACK_MODE && info->blockNum == 0) {
                info->blockNum = 1;
                info->sectorNum = -1;
                info->block[1].dramAddr = (void *)((u32)info->block[1].dramAddr - info->block[1].sectorSize);

                blockInfo->errStatus = LEO_ERROR_22;
            } else {
                IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
                __OSGlobalIntMask |= OS_IM_PI;
                info->cmdType = LEO_CMD_TYPE_2;
                blockInfo->errStatus = LEO_ERROR_GOOD;
            }

            __osEPiRawStartDma(__osDiskHandle, OS_READ, LEO_C2_BUFF, blockInfo->C2Addr, blockInfo->sectorSize * 4);
            return 1;
        }

        if (info->sectorNum == -1 && info->transferMode == LEO_TRACK_MODE && info->blockNum == 1) {
            __OSBlockInfo *bptr = &info->block[0];
            if (bptr->C1ErrNum == 0) {
                if (((u32 *)bptr->C2Addr)[0] | ((u32 *)bptr->C2Addr)[1] | ((u32 *)bptr->C2Addr)[2] | ((u32 *)bptr->C2Addr)[3]) {
                    bptr->errStatus = LEO_ERROR_24;
                    __osLeoAbnormalResume();
                    return 1;
                }
            }

            bptr->errStatus = 0;
            __osLeoResume();
        }
        info->sectorNum++;
        if (stat & LEO_STATUS_DATA_REQUEST) {
            if (info->sectorNum > 0x54) {
                blockInfo->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
                return 1;
            }

            __osEPiRawStartDma(__osDiskHandle, 0, LEO_SECTOR_BUFF, blockInfo->dramAddr, blockInfo->sectorSize);
            blockInfo->errStatus = LEO_ERROR_GOOD;
            return 1;
        } else if (info->sectorNum <= 0x54) {
            blockInfo->errStatus = LEO_ERROR_24;
            __osLeoAbnormalResume();
            return 1;
        }

        return 1;
    } else {
        blockInfo->errStatus = LEO_ERROR_4;
        __osLeoAbnormalResume();
        return 1;
    }
}

void __osLeoAbnormalResume(void) {
    __OSTranxInfo *info = &__osDiskHandle->transferInfo;
    u32 pi_stat;

    WAIT_ON_IOBUSY(pi_stat);
    IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_RESET);
    WAIT_ON_IOBUSY(pi_stat);
    IO_WRITE(LEO_BM_CTL, info->bmCtlShadow);
    __osLeoResume();
    IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
    __OSGlobalIntMask |= OS_IM_PI;
}

void __osLeoResume(void) {
    __OSEventState *es = &__osEventStateTab[OS_EVENT_PI];
    OSMesgQueue *mq = es->messageQueue;
    s32 last;

    if (mq == NULL || MQ_IS_FULL(mq)) {
        return;
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = es->message;
    mq->validCount++;

    if (((LeoThread *)mq->mtqueue)->next != NULL) {
        __osEnqueueThread(&__osRunQueue, __osPopThread(&mq->mtqueue));
    }
}
