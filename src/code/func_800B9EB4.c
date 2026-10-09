#include "types.h"

typedef struct QueueEntry {
    s32 key;
    s32 value;
    s32 unused;
} QueueEntry;

typedef struct TextureQueue {
    u8 pad0[8];
    s32 count;
    QueueEntry entries[0x100];
} TextureQueue;

s32 func_800B9EB4(TextureQueue *queue, s32 key, s32 value) {
    s32 index;

    for (index = 0; index < queue->count; index++) {
        if (key == queue->entries[index].key) {
            return 0;
        }
    }
    if (queue->count >= 0x100) {
        return -1;
    }
    queue->entries[queue->count].key = key;
    index = queue->count++;
    queue->entries[index].value = value;
    return 0;
}
