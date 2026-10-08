#include "n_synth_types.h"

typedef union {
    struct {
        u32 w0;
        u32 w1;
    } words;
    long long force_structure_alignment;
} Acmd;

#define A_CLEARBUFF 2
#define N_AL_AUX_L_OUT 0x7C0
#define N_AL_DIVIDED 0x170
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define aClearBuffer(pkt, d, c)                                         \
    {                                                                   \
        Acmd *_a = (Acmd *)pkt;                                         \
        _a->words.w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24); \
        _a->words.w1 = (unsigned int)(c);                               \
    }

extern N_ALSynth *alGlobals;
extern Acmd *n_alEnvmixerPull(N_PVoice *filter, s32 sampleOffset, Acmd *p);

Acmd *func_800FFA90(s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    N_ALAuxBus *m = alGlobals->auxBus;
    N_PVoice **sources = m->sources;
    s32 i;

    aClearBuffer(ptr++, N_AL_AUX_L_OUT, N_AL_DIVIDED << 1);

    for (i = 0; i < m->sourceCount; i++)
        ptr = n_alEnvmixerPull(sources[i], sampleOffset, ptr);
    return ptr;
}
