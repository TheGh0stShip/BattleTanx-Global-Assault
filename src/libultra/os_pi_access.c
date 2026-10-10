#include "types.h"

typedef void* OSMesg;
typedef struct OSMesgQueue {
    u8 opaque[0x18];
} OSMesgQueue;

/* libultra 2.0I io/piacs.c: initialized flag owned by this translation unit. */
u32 __osPiAccessQueueEnabled = 0;
extern OSMesgQueue D_803B0448;
extern OSMesg D_803B0440[1];

extern void osCreateMesgQueue(OSMesgQueue* queue, OSMesg* messages, s32 count);
extern s32 osSendMesg(OSMesgQueue* queue, OSMesg message, s32 flags);
extern s32 osRecvMesg(OSMesgQueue* queue, OSMesg* message, s32 flags);

void __osPiCreateAccessQueue(void) {
    __osPiAccessQueueEnabled = 1;
    osCreateMesgQueue(&D_803B0448, D_803B0440, 1);
    osSendMesg(&D_803B0448, 0, 0);
}

void __osPiGetAccess(void) {
    OSMesg message;

    if (__osPiAccessQueueEnabled == 0) {
        __osPiCreateAccessQueue();
    }
    osRecvMesg(&D_803B0448, &message, 1);
}

void __osPiRelAccess(void) {
    osSendMesg(&D_803B0448, 0, 0);
}
