#include "types.h"

s32 __osSiDeviceBusy(void);

s32 __osSiRawReadIo(u32 address, u32* value) {
    if (__osSiDeviceBusy()) {
        return -1;
    }
    *value = *(volatile u32*)(address | 0xA0000000);
    return 0;
}
