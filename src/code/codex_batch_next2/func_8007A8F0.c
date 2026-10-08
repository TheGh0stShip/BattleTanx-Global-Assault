#include "types.h"

typedef struct DisplayCommand8007A8F0 {
    u32 word0;
    u32 word1;
} DisplayCommand8007A8F0;

typedef struct DisplayRecord8007A8F0 {
    u8 pad00[0x30];
    DisplayCommand8007A8F0 *start;
    u32 size;
    u8 pad38[8];
    void *slot;
} DisplayRecord8007A8F0;

typedef struct DisplayContext8007A8F0 {
    u8 pad00[0xB0];
    s16 work_slot;
    u8 padB2[6];
    s32 slot_stride;
    u8 *slot_base;
    u16 record_index;
    u8 padC2[2];
    DisplayRecord8007A8F0 *current_record;
} DisplayContext8007A8F0;

extern DisplayContext8007A8F0 *D_80114500;
extern void osWritebackDCacheAll(void);
extern void *func_800A1280(void);
extern s32 osSendMesg(void *queue, void *message, s32 flags);

/* Exact via the prefix-only schedule documented in
 * docs/NORMALIZER_ASSISTED.md; all instructions after the first call match C. */
DisplayCommand8007A8F0 *func_8007A8F0(DisplayCommand8007A8F0 **start,
                                      DisplayCommand8007A8F0 *commands) {
    DisplayContext8007A8F0 *context;
    union {
        DisplayContext8007A8F0 *context;
        DisplayRecord8007A8F0 *record;
    } selected;
    DisplayCommand8007A8F0 *packet;
    void *slot;

    selected.context = D_80114500;
    packet = commands;
    commands++;
    packet->word0 = 0xE9000000;
    packet->word1 = 0;
    packet = commands;
    commands++;
    packet->word0 = 0xDF000000;
    packet->word1 = 0;

    selected.record = (DisplayRecord8007A8F0 *)((u8 *)selected.context +
                                                selected.context->record_index * 0x48);
    osWritebackDCacheAll();
    selected.record->start = *start;
    selected.record->size = ((commands - *start) * sizeof(DisplayCommand8007A8F0));
    context = D_80114500;
    context->current_record = selected.record;

    if (context->work_slot != -1) {
        slot = context->slot_base + context->work_slot * context->slot_stride * 2;
    } else {
        slot = 0;
    }
    D_80114500->current_record->slot = slot;
    osSendMesg(func_800A1280(), (void *)669, 1);
    return commands;
}
