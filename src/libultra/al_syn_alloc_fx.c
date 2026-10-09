/* Source shape adapted from the MIT-licensed Dr. Mario 64 libultra 2.0I. */
#include "al_synth.h"

ALFxRef *alSynAllocFX(
    ALSynth *synth, s16 bus, ALSynConfig *config, ALHeap *heap)
{
    alFxNew(&synth->auxBus[bus].fx[0], config, heap);
    alFxParam(
        &synth->auxBus[bus].fx[0], AL_FILTER_SET_SOURCE,
        &synth->auxBus[bus]);
    alMainBusParam(
        synth->mainBus, AL_FILTER_ADD_SOURCE, &synth->auxBus[bus].fx[0]);
    return (ALFxRef)(&synth->auxBus[bus].fx[0]);
}
