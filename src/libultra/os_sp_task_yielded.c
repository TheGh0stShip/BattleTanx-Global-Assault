#include "ultra.h"

#define SP_STATUS_YIELDED 0x100
#define SP_STATUS_TASKDONE 0x80
#define OS_TASK_YIELD 0x1
#define OS_TASK_DP_WAIT 0x2

typedef struct {
    u32 type;
    u32 flags;
} OSTaskHeader;

extern u32 __osSpGetStatus(void);

s32 osSpTaskYielded(OSTaskHeader *task)
{
    u32 status;
    s32 yielded;

    status = __osSpGetStatus();
    if (status & SP_STATUS_YIELDED)
        yielded = OS_TASK_YIELD;
    else
        yielded = 0;
    if (status & SP_STATUS_TASKDONE) {
        task->flags |= yielded;
        task->flags &= ~OS_TASK_DP_WAIT;
    }
    return yielded;
}
