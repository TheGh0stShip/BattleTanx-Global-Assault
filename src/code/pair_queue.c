#include "types.h"

typedef struct {
    s32 state;
    u16 pad_4;
    u16 count;
    struct {
        s32 first;
        s32 second;
    } entries[1];
} PairQueue;

void func_80082D60(void) {
}

void func_80082D68(PairQueue* queue, s32 first, s32 second) {
    s32 offset;
    s32* entry;

    queue->state = 2;
    offset = queue->count * 8 + 8;
    entry = (s32*)((u8*)queue + offset);
    entry[0] = first;
    entry[1] = second;
    queue->count++;
}
