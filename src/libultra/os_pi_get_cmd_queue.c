#include "types.h"

extern void* __osPiDevMgr;
extern void* player_bss_003C;

void* osPiGetCmdQueue(void) {
    if (__osPiDevMgr == 0) {
        return 0;
    }
    return player_bss_003C;
}
