#include "types.h"

typedef struct Pair {
    s32 first;
    s32 second;
} Pair;

typedef struct Group {
    s8 enabled;
    s8 mode;
    s16 count;
    Pair entries[5];
} Group;

typedef struct RuntimePool {
    u8 pad0000[0xC00C];
    Group first;
    Group second;
    s32 active;
    s32 pending;
    s8 flag;
    u8 padC06D;
    s16 index;
    s32 tail;
} RuntimePool;

extern RuntimePool *D_80114680;

void *func_800ACEB4(s32);
s32 func_800A18D0(s32, s32);
void func_8007D998(RuntimePool *);

void func_8008A8C4(void) {
    void *allocated;
    RuntimePool *pool;
    Group *group;
    u8 *entry;
    s32 handle;
    u16 i;

    allocated = func_800ACEB4(0xC074);
    D_80114680 = allocated;
    if (allocated != 0) {
        handle = func_800A18D0(5, 12);
        if (handle != 0) {
            pool = D_80114680;
            func_8007D998(pool);
            i = 0;
            group = &pool->first;
            group->enabled = 1;
            group->mode = 0;
            group->count = 0;
            do {
                entry = (u8 *)((unsigned long)(i * 8) + (unsigned long)group);
                entry += 4;
                i++;
                ((Pair *)entry)->first = 0;
                ((Pair *)entry)->second = 0;
            } while (i < 5);
            i = 0;
            group = &pool->second;
            group->enabled = 1;
            group->mode = 0;
            group->count = 0;
            do {
                entry = (u8 *)((unsigned long)(i * 8) + (unsigned long)group);
                entry += 4;
                i++;
                ((Pair *)entry)->first = 0;
                ((Pair *)entry)->second = 0;
            } while (i < 5);
            pool->active = 0;
            pool->pending = 0;
            pool->active = handle;
            pool->flag = 0;
            pool->index = 0;
            pool->tail = 0;
        }
    }
}
