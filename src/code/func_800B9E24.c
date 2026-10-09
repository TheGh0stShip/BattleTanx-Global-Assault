#include "types.h"

typedef struct QueueEntry {
    s32 key;
    s32 value;
    s32 unused;
} QueueEntry;

typedef struct VertexQueue {
    u8 pad0[0x1818];
    s32 count;
    QueueEntry entries[0xAF0];
} VertexQueue;

s32 func_800B9E24(VertexQueue *queue, s32 key, s32 value) {
    s32 index;

    for (index = 0; index < queue->count; index++) {
        if (key == queue->entries[index].key) {
            return 0;
        }
    }
    if (queue->count >= 0xAF0) {
        return -1;
    }
    queue->entries[queue->count].key = key;
    index = queue->count++;
    queue->entries[index].value = value;
    return 0;
}
