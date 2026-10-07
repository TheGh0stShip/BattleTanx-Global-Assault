#include "types.h"

typedef struct OSThread {
    u32 next;
    s32 priority;
} OSThread;

extern OSThread* __osRunningThread;

s32 osGetThreadPri(OSThread* thread) {
    if (thread == 0) {
        thread = __osRunningThread;
    }
    return thread->priority;
}
