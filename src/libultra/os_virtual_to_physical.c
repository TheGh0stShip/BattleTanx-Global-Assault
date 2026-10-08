#include "ultra.h"

#define K0BASE 0x80000000
#define K1BASE 0xA0000000
#define K2BASE 0xC0000000
#define PHYS_MASK 0x1FFFFFFF

extern u32 __osProbeTLB(void *address);

u32 osVirtualToPhysical(void *address)
{
    if ((u32)address >= K0BASE && (u32)address < K1BASE)
        return (u32)address & PHYS_MASK;
    if ((u32)address >= K1BASE && (u32)address < K2BASE)
        return (u32)address & PHYS_MASK;
    return __osProbeTLB(address);
}
