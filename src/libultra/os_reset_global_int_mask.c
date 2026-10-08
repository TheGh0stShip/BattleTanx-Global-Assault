#include "ultra.h"

extern u32 __OSGlobalIntMask;

void __osResetGlobalIntMask(u32 mask)
{
    register OSIntMask saveMask = __osDisableInt();

    __OSGlobalIntMask &= ~(mask & ~0x401);
    __osRestoreInt(saveMask);
}
