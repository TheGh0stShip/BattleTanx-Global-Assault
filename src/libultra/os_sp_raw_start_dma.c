#include "ultra.h"

#define SP_MEM_ADDR_REG 0xA4040000
#define SP_DRAM_ADDR_REG 0xA4040004
#define SP_RD_LEN_REG 0xA4040008
#define SP_WR_LEN_REG 0xA404000C

extern s32 __osSpDeviceBusy(void);
extern u32 osVirtualToPhysical(void *addr);

s32 __osSpRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size)
{
    if (__osSpDeviceBusy())
        return -1;
    IO_WRITE(SP_MEM_ADDR_REG, devAddr);
    IO_WRITE(SP_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));
    if (direction == OS_READ)
        IO_WRITE(SP_WR_LEN_REG, size - 1);
    else
        IO_WRITE(SP_RD_LEN_REG, size - 1);
    return 0;
}
