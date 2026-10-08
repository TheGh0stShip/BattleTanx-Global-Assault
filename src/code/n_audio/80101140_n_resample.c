/* RODATA_VRAM 0x80077790: this unit's literal pool is linked at its retail address. */
#include "n_audio_private.h"
#include "n_abi.h"

#define A_DMEMMOVE 10
#define A_RESAMPLE 5
#define N_AL_DECODER_OUT 0x170
#define UNITY_PITCH 0x8000
#define MAX_RATIO 1.99996

extern Acmd *n_alAdpcmPull(N_PVoice *e, s16 *outp, s32 outCount, Acmd *p);
extern void n_alLoadParam(N_PVoice *filter, s32 paramID, void *param);

Acmd *n_alResamplePull(N_PVoice *e, s16 *outp, Acmd *p)
{
    Acmd *ptr = p;
    s16 inp;
    s32 inCount;
    s32 incr;
    f32 finCount;

    inp = N_AL_DECODER_OUT;

    if (e->rs_upitch) {
        ptr = n_alAdpcmPull(e, &inp, FIXED_SAMPLE, p);
        {
            Acmd *_a = ptr++;
            _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(inp, 0, 24);
            _a->words.w1 = _SHIFTL(*outp, 16, 16) | _SHIFTL(FIXED_SAMPLE << 1, 0, 16);
        }
    } else {
        if (e->rs_ratio > MAX_RATIO)
            e->rs_ratio = MAX_RATIO;
        e->rs_ratio = (s32)(e->rs_ratio * UNITY_PITCH) / (f32)UNITY_PITCH;
        finCount = e->rs_delta + (e->rs_ratio * (f32)FIXED_SAMPLE);
        inCount = (s32)finCount;
        e->rs_delta = finCount - (f32)inCount;
        ptr = n_alAdpcmPull(e, &inp, inCount, p);
        incr = (s32)(e->rs_ratio * UNITY_PITCH);
        {
            Acmd *_a = ptr++;
            _a->words.w0 = _SHIFTL(A_RESAMPLE, 24, 8) | _SHIFTL(osVirtualToPhysical(e->rs_state), 0, 24);
            _a->words.w1 = _SHIFTL(e->rs_first, 30, 2) | _SHIFTL(incr, 14, 16) | _SHIFTL(inp, 2, 12);
        }
        e->rs_first = 0;
    }
    return ptr;
}

s32 n_alResampleParam(N_PVoice *filter, s32 paramID, void *param)
{
    n_alLoadParam(filter, paramID, param);
    return 0;
}
