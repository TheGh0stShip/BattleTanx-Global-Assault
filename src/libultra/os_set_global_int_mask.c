#include "ultra.h"

extern u32 __OSGlobalIntMask;

void __osSetGlobalIntMask(u32 mask)
{
    register OSIntMask saveMask = __osDisableInt();

    __OSGlobalIntMask |= mask;
    __osRestoreInt(saveMask);
}
