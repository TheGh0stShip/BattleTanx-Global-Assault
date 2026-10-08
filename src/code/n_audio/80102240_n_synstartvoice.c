#include "n_audio_private.h"

typedef struct {
    ALParam *next;             /* 0x00 */
    s32 delta;                 /* 0x04 */
    s16 type;                  /* 0x08 */
    s16 unity;                 /* 0x0A */
    void *wave;                /* 0x0C */
} ALStartParamAlt;

void func_80102240(N_ALVoice *v, void *table)
{
    ALStartParamAlt *update;

    if (v->pvoice) {
        update = (ALStartParamAlt *)__allocParam();
        if (update) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = 14;
            update->wave = table;
            update->next = 0;
            update->unity = v->unityPitch;
            n_alEnvmixerParam(v->pvoice, 3, update);
        }
    }
}
