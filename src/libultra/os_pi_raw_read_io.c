#include "ultra.h"

#define PI_STATUS_REG 0xA4600010
#define PI_STATUS_IO_BUSY 0x2
#define PI_STATUS_DMA_BUSY 0x1

extern u32 D_80000308;

s32 osPiRawReadIo(u32 devAddr, u32 *data)
{
    register u32 stat;

    while ((stat = IO_READ(PI_STATUS_REG)) & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) {
    }
    *data = IO_READ(PHYS_TO_K1(D_80000308 | devAddr));
    return 0;
}
