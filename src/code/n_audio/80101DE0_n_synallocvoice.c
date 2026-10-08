#include "n_audio_private.h"

s32 func_80101EF8(N_PVoice **pvoice, s16 priority);

s32 func_80101DE0(N_ALVoice *voice, ALVoiceConfig *vc)
{
    N_PVoice *pvoice = 0;
    ALParam *update;
    s32 stolen;

    voice->priority = vc->priority;
    voice->unityPitch = vc->unityPitch;
    voice->table = 0;
    voice->fxBus = vc->fxBus;
    voice->state = 0;
    voice->pvoice = 0;

    stolen = func_80101EF8(&pvoice, vc->priority);

    if (pvoice) {
        if (stolen) {
            pvoice->offset = 552;
            pvoice->vvoice->pvoice = 0;
            pvoice->vvoice = voice;
            voice->pvoice = pvoice;

            update = __allocParam();
            update->delta = n_syn->paramSamples;
            update->type = 11;
            update->data.i = 0;
            update->moredata.i = 368;
            n_alEnvmixerParam(voice->pvoice, 3, update);

            update = __allocParam();
            if (update) {
                update->delta = n_syn->paramSamples + pvoice->offset;
                update->type = 15;
                update->next = 0;
                n_alEnvmixerParam(voice->pvoice, 3, update);
            }
        } else {
            pvoice->offset = 0;
            pvoice->vvoice = voice;
            voice->pvoice = pvoice;
        }
    }
    return pvoice != 0;
}

s32 func_80101EF8(N_PVoice **pvoice, s16 priority)
{
    ALLink *dl;
    N_PVoice *pv;
    s32 stolen = 0;

    if ((dl = n_syn->pLameList.next) != 0) {
        *pvoice = (N_PVoice *)dl;
        alUnlink(dl);
        alLink(dl, &n_syn->pAllocList);
    } else if ((dl = n_syn->pFreeList.next) != 0) {
        *pvoice = (N_PVoice *)dl;
        alUnlink(dl);
        alLink(dl, &n_syn->pAllocList);
    } else {
        for (dl = n_syn->pAllocList.next; dl != 0; dl = dl->next) {
            pv = (N_PVoice *)dl;
            if (pv->vvoice->priority <= priority && pv->offset == 0) {
                *pvoice = pv;
                priority = pv->vvoice->priority;
                stolen = 1;
            }
        }
    }
    return stolen;
}
