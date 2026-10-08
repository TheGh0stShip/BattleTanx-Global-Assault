#include "n_audio_private.h"

void func_801022D0(N_ALVoice *v)
{
    ALParam *update;

    if (v->pvoice) {
        update = __allocParam();
        if (update) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = 15;
            update->next = 0;
            n_alEnvmixerParam(v->pvoice, 3, update);
        }
    }
}
