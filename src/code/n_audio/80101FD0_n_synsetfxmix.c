#include "n_audio_private.h"

void func_80101FD0(N_ALVoice *v, u8 fxmix)
{
    ALParam *update;

    if (v->pvoice) {
        update = __allocParam();
        if (update) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = 16;
            if (fxmix > 127)
                fxmix = 127;
            update->data.i = fxmix;
            update->next = 0;
            n_alEnvmixerParam(v->pvoice, 3, update);
        }
    }
}
