#include "n_audio_private.h"

extern s32 func_80102854(s32 micros);

void func_80102190(N_ALVoice *v, s16 volume, s32 t)
{
    ALParam *update;

    if (v->pvoice) {
        update = __allocParam();
        if (update) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = 11;
            update->data.i = volume;
            update->moredata.i = func_80102854(t);
            update->next = 0;
            n_alEnvmixerParam(v->pvoice, 3, update);
        }
    }
}
