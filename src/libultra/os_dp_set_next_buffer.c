#include "ultra.h"

#define DPC_START_REG 0xA4100000
#define DPC_END_REG 0xA4100004
#define DPC_STATUS_REG 0xA410000C
#define DPC_STATUS_START_VALID 0x1
#define DPC_CLR_XBUS_DMEM_DMA 0x1

extern s32 __osDpDeviceBusy(void);
extern u32 osVirtualToPhysical(void *addr);

s32 osDpSetNextBuffer(void *bufPtr, u64 size)
{
    register u32 status;

    if (__osDpDeviceBusy())
        return -1;
    IO_WRITE(DPC_STATUS_REG, DPC_CLR_XBUS_DMEM_DMA);
    do {
        status = IO_READ(DPC_STATUS_REG);
    } while (status & DPC_STATUS_START_VALID);
    IO_WRITE(DPC_START_REG, osVirtualToPhysical(bufPtr));
    IO_WRITE(DPC_END_REG, osVirtualToPhysical(bufPtr) + size);
    return 0;
}
