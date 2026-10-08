#include "ultra.h"

#define PI_DRAM_ADDR_REG 0xA4600000
#define PI_CART_ADDR_REG 0xA4600004
#define PI_RD_LEN_REG 0xA4600008
#define PI_WR_LEN_REG 0xA460000C
#define PI_STATUS_REG 0xA4600010
#define PI_BSD_DOM1_LAT_REG 0xA4600014
#define PI_BSD_DOM1_PWD_REG 0xA4600018
#define PI_BSD_DOM1_PGS_REG 0xA460001C
#define PI_BSD_DOM1_RLS_REG 0xA4600020
#define PI_BSD_DOM2_LAT_REG 0xA4600024
#define PI_BSD_DOM2_PWD_REG 0xA4600028
#define PI_BSD_DOM2_PGS_REG 0xA460002C
#define PI_BSD_DOM2_RLS_REG 0xA4600030
#define PI_STATUS_DMA_BUSY 0x01
#define PI_STATUS_IO_BUSY 0x02
#define PI_DOMAIN1 0
#define K1_TO_PHYS(x) ((u32)(x) & 0x1FFFFFFF)

extern OSPiHandle *__osCurrentHandle[2];
extern u32 osVirtualToPhysical(void *addr);

s32 osEPiRawStartDma(OSPiHandle *pihandle, s32 direction, u32 devAddr,
                     void *dramAddr, u32 size)
{
    u32 stat;
    u32 domain;

    stat = IO_READ(PI_STATUS_REG);
    while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY))
        stat = IO_READ(PI_STATUS_REG);
    domain = pihandle->domain;
    if (__osCurrentHandle[domain] != pihandle) {
        OSPiHandle *cHandle = __osCurrentHandle[domain];

        if (domain == PI_DOMAIN1) {
            if (cHandle->latency != pihandle->latency)
                IO_WRITE(PI_BSD_DOM1_LAT_REG, pihandle->latency);
            if (cHandle->pageSize != pihandle->pageSize)
                IO_WRITE(PI_BSD_DOM1_PGS_REG, pihandle->pageSize);
            if (cHandle->relDuration != pihandle->relDuration)
                IO_WRITE(PI_BSD_DOM1_RLS_REG, pihandle->relDuration);
            if (cHandle->pulse != pihandle->pulse)
                IO_WRITE(PI_BSD_DOM1_PWD_REG, pihandle->pulse);
        } else {
            if (cHandle->latency != pihandle->latency)
                IO_WRITE(PI_BSD_DOM2_LAT_REG, pihandle->latency);
            if (cHandle->pageSize != pihandle->pageSize)
                IO_WRITE(PI_BSD_DOM2_PGS_REG, pihandle->pageSize);
            if (cHandle->relDuration != pihandle->relDuration)
                IO_WRITE(PI_BSD_DOM2_RLS_REG, pihandle->relDuration);
            if (cHandle->pulse != pihandle->pulse)
                IO_WRITE(PI_BSD_DOM2_PWD_REG, pihandle->pulse);
        }
        __osCurrentHandle[domain] = pihandle;
    }
    IO_WRITE(PI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS(pihandle->baseAddress | devAddr));
    switch (direction) {
    case OS_READ:
        IO_WRITE(PI_WR_LEN_REG, size - 1);
        break;
    case OS_WRITE:
        IO_WRITE(PI_RD_LEN_REG, size - 1);
        break;
    default:
        return -1;
    }
    return 0;
}
