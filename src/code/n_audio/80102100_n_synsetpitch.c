#include "n_audio_private.h"

void func_80102100(N_ALVoice *v, f32 pitch)
{
    ALParam *update;

    if (v->pvoice) {
        update = __allocParam();
        if (update) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = 7;
            update->data.f = pitch;
            update->next = 0;
            n_alEnvmixerParam(v->pvoice, 3, update);
        }
    }
}
