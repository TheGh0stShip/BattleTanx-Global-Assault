#include "types.h"

typedef void* OSMesg;
typedef u32 OSIntMask;

typedef struct OSMesgQueue {
    u8 opaque[0x18];
} OSMesgQueue;

typedef struct OSThread {
    u8 opaque[0x1B0];
} OSThread;

typedef struct OSScClient {
    struct OSScClient* next;
    OSMesgQueue* msg_queue;
} OSScClient;

typedef struct OSScMsg {
    s16 type;
    u8 payload[0x1E];
} OSScMsg;

typedef struct OSSched {
    OSScMsg retrace_msg;
    OSScMsg prenmi_msg;
    OSMesgQueue interrupt_queue;
    OSMesg interrupt_messages[8];
    OSMesgQueue command_queue;
    OSMesg command_messages[8];
    OSThread thread;
    OSScClient* client_list;
    void* audio_list_head;
    void* graphics_list_head;
    void* audio_list_tail;
    void* graphics_list_tail;
    void* current_rsp_task;
    void* current_rdp_task;
    u32 frame_count;
    s32 do_audio;
} OSSched;

typedef struct OSViMode {
    u8 opaque[0x50];
} OSViMode;

extern OSViMode osViModeTable[];

extern OSIntMask osSetIntMask(OSIntMask mask);
extern void osCreateMesgQueue(OSMesgQueue* queue, OSMesg* messages, s32 count);
extern void osCreateViManager(s32 priority);
extern void osViSetMode(OSViMode* mode);
extern void osViBlack(s32 active);
extern void osSetEventMesg(s32 event, OSMesgQueue* queue, OSMesg message);
extern void osViSetEvent(OSMesgQueue* queue, OSMesg message, u32 retrace_count);
extern void osCreateThread(OSThread* thread, s32 id, void (*entry)(void*),
                           void* argument, void* stack, s32 priority);
extern void osStartThread(OSThread* thread);
extern void sched_text_0554(void* argument);

OSMesgQueue* osScGetCmdQ(OSSched* scheduler) {
    return &scheduler->command_queue;
}

void osScRemoveClient(OSSched* scheduler, OSScClient* target) {
    OSScClient* client = scheduler->client_list;
    OSScClient* previous = 0;
    OSIntMask mask;

    mask = osSetIntMask(1);
    while (client != 0) {
        if (client == target) {
            if (previous != 0) {
                previous->next = target->next;
            } else {
                scheduler->client_list = target->next;
            }
            break;
        }
        previous = client;
        client = client->next;
    }
    osSetIntMask(mask);
}

void osScAddClient(OSSched* scheduler, OSScClient* client,
                   OSMesgQueue* message_queue) {
    OSIntMask mask;

    mask = osSetIntMask(1);
    client->msg_queue = message_queue;
    client->next = scheduler->client_list;
    scheduler->client_list = client;
    osSetIntMask(mask);
}

void osCreateScheduler(OSSched* scheduler, void* stack, s32 priority,
                       u8 mode, u8 retrace_count) {
    scheduler->current_rsp_task = 0;
    scheduler->current_rdp_task = 0;
    scheduler->client_list = 0;
    scheduler->frame_count = 0;
    scheduler->audio_list_head = 0;
    scheduler->graphics_list_head = 0;
    scheduler->audio_list_tail = 0;
    scheduler->graphics_list_tail = 0;
    scheduler->retrace_msg.type = 1;
    scheduler->prenmi_msg.type = 4;

    osCreateMesgQueue(&scheduler->interrupt_queue,
                      scheduler->interrupt_messages, 8);
    osCreateMesgQueue(&scheduler->command_queue,
                      scheduler->command_messages, 8);

    osCreateViManager(254);
    osViSetMode(&osViModeTable[mode]);
    osViBlack(1);
    osSetEventMesg(4, &scheduler->interrupt_queue, (OSMesg)667);
    osSetEventMesg(9, &scheduler->interrupt_queue, (OSMesg)668);
    osSetEventMesg(14, &scheduler->interrupt_queue, (OSMesg)669);
    osViSetEvent(&scheduler->interrupt_queue, (OSMesg)666, retrace_count);

    osCreateThread(&scheduler->thread, 4, sched_text_0554, scheduler, stack,
                   priority);
    osStartThread(&scheduler->thread);
}
