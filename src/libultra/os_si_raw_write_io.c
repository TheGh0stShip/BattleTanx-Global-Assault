#include "types.h"

s32 __osSiDeviceBusy(void);

s32 __osSiRawWriteIo(u32 address, u32 value) {
    if (__osSiDeviceBusy()) {
        return -1;
    }
    *(volatile u32*)(address | 0xA0000000) = value;
    return 0;
}
