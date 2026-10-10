#include "types.h"

/*
 * libmus scheduler callback table (musSched in the N64 libmus API:
 * install, waitframe, dotask). func_80097560 hands it to the libmus
 * scheduler setter (func_800FBDBC) before MusInitialize. Entries are kept
 * as u32 N64 address tokens, not host function pointers.
 */
typedef struct MusSchedTokens {
    u32 install;   /* func_80097794: creates the audio message queues */
    u32 waitframe; /* func_800977DC: drains the retrace queue */
    u32 dotask;    /* func_80097844: builds and submits the audio RSP task */
} MusSchedTokens;

void func_80097794(void);
void func_800977DC(void);
void func_80097844(void);

u8 gAudioFrameCounter = 0;   /* D_801147E0: sb/lbu in 80097D14, 800979F4, 80097A6C, 80097DF8 */
s32 gAudioUnusedWord = 0;    /* D_801147E4: no consumer found in code */
s32 gAudioPendingTask = 0;   /* D_801147E8: pending audio RSP task (Task97844 *) */
/* D_801147EC */
MusSchedTokens gMusSchedCallbacks = {
    (u32)func_80097794,
    (u32)func_800977DC,
    (u32)func_80097844,
};
/* 0x801147F8-0x80114800: KMC as pads this unit's .data to 16 bytes. */
