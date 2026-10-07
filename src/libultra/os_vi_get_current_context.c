#include "types.h"

extern void* __osViCurr;

void* __osViGetCurrentContext(void) {
    return __osViCurr;
}
