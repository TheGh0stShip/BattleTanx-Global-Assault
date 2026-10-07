#include "types.h"

void __osSpSetStatus(u32 status);

void osSpTaskYield(void) {
    __osSpSetStatus(0x400);
}
