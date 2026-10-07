#include "types.h"

s32 __osDpDeviceBusy(void) {
    register u32 status = *(volatile u32*)0xA410000C;
    if (status & 0x100) {
        return 1;
    }
    return 0;
}
