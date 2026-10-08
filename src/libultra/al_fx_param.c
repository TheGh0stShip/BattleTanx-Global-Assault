#include "types.h"

s32 alFxParam(void **filter, s32 paramID, void *param)
{
    if (paramID == 1) {
        *filter = param;
    }
    return 0;
}
