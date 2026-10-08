/* IDO 5.3: -O1 -mips2 -non_shared -G 0 */
#include "ultra.h"

#define PI_DOM1_ADDR2 0x10000000
#define DEVICE_TYPE_CART 0
#define PI_DOMAIN1 0

OSPiHandle CartRomHandle;
extern OSPiHandle *__osPiTable;
extern s32 osPiRawReadIo(u32 devAddr, u32 *data);

OSPiHandle *osCartRomInit(void)
{
    u32 domain = 0;
    u32 saveMask;

    if (CartRomHandle.baseAddress == PHYS_TO_K1(PI_DOM1_ADDR2))
        return &CartRomHandle;

    CartRomHandle.type = DEVICE_TYPE_CART;
    CartRomHandle.baseAddress = PHYS_TO_K1(PI_DOM1_ADDR2);
    osPiRawReadIo(0, &domain);
    CartRomHandle.latency = domain & 0xFF;
    CartRomHandle.pulse = (domain >> 8) & 0xFF;
    CartRomHandle.pageSize = domain >> 16 & 0xF;
    CartRomHandle.relDuration = domain >> 20 & 0xF;
    CartRomHandle.domain = PI_DOMAIN1;
    CartRomHandle.speed = 0;

    _bzero(&CartRomHandle.transferInfo, sizeof(__OSTranxInfo));

    saveMask = __osDisableInt();
    CartRomHandle.next = __osPiTable;
    __osPiTable = &CartRomHandle;
    __osRestoreInt(saveMask);

    return &CartRomHandle;
}
