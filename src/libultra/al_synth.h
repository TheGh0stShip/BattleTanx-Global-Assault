#ifndef AL_SYNTH_H
#define AL_SYNTH_H

#include "audio.h"
#include "abi.h"

typedef s32 ALMicroTime;
typedef void *ALFxRef;

typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;

typedef ALMicroTime (*ALVoiceHandler)(void *);

typedef struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    ALVoiceHandler handler;
    ALMicroTime callTime;
    s32 samplesLeft;
} ALPlayer;

typedef struct ALVoice_s {
    ALLink node;
    struct PVoice_s *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
} ALVoice;

typedef struct SynthAuxBus_s {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
    ALFx fx[1];
} SynthAuxBus;

typedef struct ALSynth_s {
    ALPlayer *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    ALDMANew dma;
    ALHeap *heap;
    ALParam *paramList;
    ALMainBus *mainBus;
    SynthAuxBus *auxBus;
    ALFilter *outputFilter;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
} ALSynth;

typedef struct PVoice_s {
    ALLink node;
    ALVoice *vvoice;
    ALFilter *channelKnob;
    ALLoadFilter decoder;
    ALResampler resampler;
    ALEnvMixer envmixer;
    s32 offset;
} PVoice;

typedef struct {
    ALSynth drvr;
} ALGlobals;

#define AL_FILTER_SET_SOURCE 1
#define AL_FILTER_ADD_SOURCE 2
#define AL_FILTER_SET_DRAM 6
#define AL_MAX_RSP_SAMPLES 160
#define AL_FX_NONE 0

extern ALGlobals *alGlobals_80126ED0;

extern void _collectPVoices(ALSynth *);
extern void _freePVoice(ALSynth *, PVoice *);

extern void alLink(ALLink *, ALLink *);
extern void alUnlink(ALLink *);
extern void alSaveNew(ALSave *);
extern void alAuxBusNew(SynthAuxBus *, ALFilter **, s32);
extern void alMainBusNew(ALMainBus *, ALFilter **, s32);
extern s32 alMainBusParam(void *, s32, void *);
extern s32 alAuxBusParam(void *, s32, void *);
extern void alLoadNew(ALLoadFilter *, ALDMANew, ALHeap *);
extern s32 alLoadParam(void *, s32, void *);
extern void alResampleNew(ALResampler *, ALHeap *);
extern s32 alResampleParam(void *, s32, void *);
extern void alEnvmixerNew(ALEnvMixer *, ALHeap *);
extern s32 alEnvmixerParam(void *, s32, void *);
extern s32 alSaveParam(void *, s32, void *);
extern void alFxNew(ALFx *, ALSynConfig *, ALHeap *);
extern s32 alFxParam(void *, s32, void *);
extern ALFxRef *alSynAllocFX(ALSynth *, s16, ALSynConfig *, ALHeap *);

#endif
