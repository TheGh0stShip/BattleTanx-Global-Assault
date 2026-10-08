/* IDOFLAGS: -O1 -mips2 */
#include "ultra.h"

extern s32 __osAiDeviceBusy(void);
extern u32 osVirtualToPhysical(void *addr);
extern u8 aisetnextbuf_data_0000;

s32 osAiSetNextBuffer(void *bufPtr, u32 size)
{
    char *bptr = bufPtr;

    if (aisetnextbuf_data_0000 != 0)
        bptr -= 0x2000;

    if ((((u32)bufPtr + size) & 0x3FFF) == 0x2000)
        aisetnextbuf_data_0000 = 1;
    else
        aisetnextbuf_data_0000 = 0;

    if (__osAiDeviceBusy())
        return -1;

    IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(bptr));
    IO_WRITE(AI_LEN_REG, size);
    return 0;
}
