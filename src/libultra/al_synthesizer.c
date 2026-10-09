/* Source shape adapted from the MIT-licensed Dr. Mario 64 libultra 2.0I. */
#include "al_synth.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static s32 __nextSampleTime(ALSynth *driver, ALPlayer **client);
static s32 _timeToSamplesNoRound(ALSynth *synth, s32 micros);

void alSynNew(ALSynth *driver, ALSynConfig *config)
{
    s32 i;
    ALVoice *virtualVoice;
    PVoice *physicalVoice;
    ALVoice *virtualVoices;
    PVoice *physicalVoices;
    ALHeap *heap = config->heap;
    ALSave *save;
    ALFilter **sources;
    ALParam *params;
    ALParam *param;

    driver->head = 0;
    driver->numPVoices = config->maxPVoices;
    driver->curSamples = 0;
    driver->paramSamples = 0;
    driver->outputRate = config->outputRate;
    driver->maxOutSamples = AL_MAX_RSP_SAMPLES;
    driver->dma = (ALDMANew)config->dmaproc;

    save = alHeapAlloc(heap, 1, sizeof(ALSave));
    alSaveNew(save);
    driver->outputFilter = (ALFilter *)save;

    driver->auxBus = alHeapAlloc(heap, 1, sizeof(SynthAuxBus));
    driver->maxAuxBusses = 1;
    sources = alHeapAlloc(heap, config->maxPVoices, sizeof(ALFilter *));
    alAuxBusNew(driver->auxBus, sources, config->maxPVoices);

    driver->mainBus = alHeapAlloc(heap, 1, sizeof(ALMainBus));
    sources = alHeapAlloc(heap, config->maxPVoices, sizeof(ALFilter *));
    alMainBusNew(driver->mainBus, sources, config->maxPVoices);

    if (config->fxType != AL_FX_NONE) {
        alSynAllocFX(driver, 0, config, heap);
    } else {
        alMainBusParam(
            driver->mainBus, AL_FILTER_ADD_SOURCE, &driver->auxBus[0]);
    }

    driver->pFreeList.next = 0;
    driver->pFreeList.prev = 0;
    driver->pLameList.next = 0;
    driver->pLameList.prev = 0;
    driver->pAllocList.next = 0;
    driver->pAllocList.prev = 0;

    physicalVoices = alHeapAlloc(
        heap, config->maxPVoices, sizeof(PVoice));
    for (i = 0; i < config->maxPVoices; i++) {
        physicalVoice = &physicalVoices[i];
        alLink((ALLink *)physicalVoice, &driver->pFreeList);
        physicalVoice->vvoice = 0;

        alLoadNew(&physicalVoice->decoder, driver->dma, heap);
        alLoadParam(&physicalVoice->decoder, AL_FILTER_SET_SOURCE, 0);
        alResampleNew(&physicalVoice->resampler, heap);
        alResampleParam(
            &physicalVoice->resampler, AL_FILTER_SET_SOURCE,
            &physicalVoice->decoder);
        alEnvmixerNew(&physicalVoice->envmixer, heap);
        alEnvmixerParam(
            &physicalVoice->envmixer, AL_FILTER_SET_SOURCE,
            &physicalVoice->resampler);
        alAuxBusParam(
            driver->auxBus, AL_FILTER_ADD_SOURCE,
            &physicalVoice->envmixer);
        physicalVoice->channelKnob = (ALFilter *)&physicalVoice->envmixer;
    }

    alSaveParam(save, AL_FILTER_SET_SOURCE, driver->mainBus);

    params = alHeapAlloc(heap, config->maxUpdates, sizeof(ALParam));
    driver->paramList = 0;
    for (i = 0; i < config->maxUpdates; i++) {
        param = &params[i];
        param->next = driver->paramList;
        driver->paramList = param;
    }
    driver->heap = heap;
}

Acmd *alAudioFrame(Acmd *commands, s32 *commandLength, s16 *outputBuffer,
                   s32 outputLength)
{
    ALPlayer *client;
    ALFilter *output;
    ALSynth *driver = &alGlobals_80126ED0->drvr;
    s16 temporary = 0;
    Acmd *commandEnd = commands;
    Acmd *command;
    s32 outputCount;
    s16 *localOutput = outputBuffer;

    if (driver->head == 0) {
        *commandLength = 0;
        return commands;
    }

    for (driver->paramSamples = __nextSampleTime(driver, &client);
         driver->paramSamples - driver->curSamples < outputLength;
         driver->paramSamples = __nextSampleTime(driver, &client)) {
        driver->paramSamples &= ~0xf;
        client->samplesLeft +=
            _timeToSamplesNoRound(driver, (*client->handler)(client));
    }
    driver->paramSamples &= ~0xf;

    while (outputLength > 0) {
        outputCount = MIN(driver->maxOutSamples, outputLength);
        command = commandEnd;
        aSegment(command++, 0, 0);
        output = driver->outputFilter;
        (*output->setParam)(output, AL_FILTER_SET_DRAM, localOutput);
        commandEnd = (Acmd *)(*output->handler)(
            output, &temporary, outputCount, driver->curSamples, command);
        outputLength -= outputCount;
        localOutput += outputCount << 1;
        driver->curSamples += outputCount;
    }
    *commandLength = (s32)(commandEnd - commands);
    _collectPVoices(driver);
    return commandEnd;
}

ALParam *__allocParam_80110870(void)
{
    ALParam *update = 0;
    ALSynth *driver = &alGlobals_80126ED0->drvr;

    if (driver->paramList) {
        update = driver->paramList;
        driver->paramList = driver->paramList->next;
        update->next = 0;
    }
    return update;
}

void __freeParam_80110858(ALParam *param)
{
    ALSynth *driver = &alGlobals_80126ED0->drvr;
    param->next = driver->paramList;
    driver->paramList = param;
}

void _collectPVoices(ALSynth *driver)
{
    ALLink *link;
    PVoice *voice;

    while ((link = driver->pLameList.next) != 0) {
        voice = (PVoice *)link;
        alUnlink(link);
        alLink(link, &driver->pFreeList);
    }
}

void _freePVoice(ALSynth *driver, PVoice *voice)
{
    alUnlink((ALLink *)voice);
    alLink((ALLink *)voice, &driver->pLameList);
}

static s32 _timeToSamplesNoRound(ALSynth *synth, s32 micros)
{
    f32 result = ((f32)micros) * synth->outputRate / 1000000.0 + 0.5;
    return (s32)result;
}

s32 _timeToSamples(ALSynth *synth, s32 micros)
{
    return _timeToSamplesNoRound(synth, micros) & ~0xf;
}

static s32 __nextSampleTime(ALSynth *driver, ALPlayer **client)
{
    ALMicroTime delta = 0x7fffffff;
    ALPlayer *current;

    *client = 0;
    for (current = driver->head; current != 0; current = current->next) {
        if ((current->samplesLeft - driver->curSamples) < delta) {
            *client = current;
            delta = current->samplesLeft - driver->curSamples;
        }
    }
    return (*client)->samplesLeft;
}
