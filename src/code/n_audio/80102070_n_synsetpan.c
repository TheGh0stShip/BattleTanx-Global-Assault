#include "n_audio_private.h"

void func_80102070(N_ALVoice *v, u8 pan)
{
    ALParam *update;

    if (v->pvoice) {
        update = __allocParam();
        if (update) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = 12;
            update->data.i = pan;
            update->next = 0;
            n_alEnvmixerParam(v->pvoice, 3, update);
        }
    }
}
