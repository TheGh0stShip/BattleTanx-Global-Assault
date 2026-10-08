/* Private n_audio declarations for the 0x800FFB30-0x801029D0 units. */
#ifndef N_AUDIO_PRIVATE_H
#define N_AUDIO_PRIVATE_H
#include "n_synth_types.h"

typedef struct ALPlayer_s {
    struct ALPlayer_s *next;   /* 0x00 */
    void *clientData;          /* 0x04 */
    void *handler;             /* 0x08 */
    s32 callTime;              /* 0x0C */
    s32 samplesLeft;           /* 0x10 */
} ALPlayer;

typedef struct N_ALVoice_s {
    ALLink node;               /* 0x00 */
    N_PVoice *pvoice;          /* 0x08 */
    void *table;               /* 0x0C */
    void *clientPrivate;       /* 0x10 */
    s16 state;                 /* 0x14 */
    s16 priority;              /* 0x16 */
    s16 fxBus;                 /* 0x18 */
    s16 unityPitch;            /* 0x1A */
} N_ALVoice;

typedef struct {
    s16 priority;              /* 0x00 */
    s16 fxBus;                 /* 0x02 */
    u8 unityPitch;             /* 0x04 */
} ALVoiceConfig;

extern N_ALGlobals *n_alGlobals;
extern N_ALSynth *alGlobals;
#define n_syn alGlobals

extern ALParam *__allocParam(void);
extern void __freeParam(ALParam *param);
extern s32 n_alEnvmixerParam(void *filter, s32 paramID, void *param);
extern void alUnlink(ALLink *ln);
extern u32 osSetIntMask(u32 mask);
extern u32 osVirtualToPhysical(void *addr);
#endif
