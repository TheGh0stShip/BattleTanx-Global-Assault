#include "ultra.h"

extern u32 D_803B0578;
extern OSTime D_803B0570;
extern u32 osGetCount(void);

OSTime osGetTime(void)
{
    u32 tmptime;
    u32 elapseCount;
    OSTime currentCount;
    register u32 saveMask;

    saveMask = __osDisableInt();
    tmptime = osGetCount();
    elapseCount = tmptime - D_803B0578;
    currentCount = D_803B0570;
    __osRestoreInt(saveMask);
    return currentCount + elapseCount;
}
