/* Source adapted from the MIT-licensed Dr. Mario 64 libultra 2.0I. */
#include "types.h"
#include "abi.h"

typedef struct ALFilter_s ALFilter;
typedef Acmd *(*ALFilterHandler)(ALFilter *, s16 *, s32, s32, Acmd *);
typedef s32 (*ALSetParam)(ALFilter *, s32, void *);

struct ALFilter_s {
    ALFilter *source;
    ALFilterHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
};

typedef struct ALParam_s ALParam;

typedef struct {
    ALFilter filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    s32 motion;
} ALResampler;

enum {
    AL_FILTER_FREE_VOICE,
    AL_FILTER_SET_SOURCE,
    AL_FILTER_ADD_SOURCE,
    AL_FILTER_ADD_UPDATE,
    AL_FILTER_RESET,
    AL_FILTER_SET_WAVETABLE,
    AL_FILTER_SET_DRAM,
    AL_FILTER_SET_PITCH,
    AL_FILTER_SET_UNITY_PITCH,
    AL_FILTER_START
};

#define AL_DECODER_OUT 320
#define AL_STOPPED 0
#define AL_PLAYING 1
#define UNITY_PITCH 0x8000
#define MAX_RATIO 1.99996

extern u32 osVirtualToPhysical(void *);

Acmd *alResamplePull(void *filter, s16 *outp, s32 outCnt, s32 sampleOffset,
                     Acmd *p)
{
    ALResampler *f = (ALResampler *)filter;
    Acmd *ptr = p;
    s16 inp;
    s32 inCount;
    ALFilter *source = f->filter.source;
    s32 incr;
    f32 finCount;

    inp = AL_DECODER_OUT;

    if (!outCnt)
        return ptr;

    if (f->upitch) {
        ptr = (*source->handler)(source, &inp, outCnt, sampleOffset, p);
        aDMEMMove(ptr++, inp, *outp, outCnt << 1);
    } else {
        if (f->ratio > MAX_RATIO)
            f->ratio = MAX_RATIO;

        f->ratio = (s32)(f->ratio * UNITY_PITCH);
        f->ratio = f->ratio / UNITY_PITCH;

        finCount = f->delta + (f->ratio * (f32)outCnt);
        inCount = (s32)finCount;
        f->delta = finCount - (f32)inCount;

        ptr = (*source->handler)(source, &inp, inCount, sampleOffset, p);

        incr = (s32)(f->ratio * UNITY_PITCH);
        aSetBuffer(ptr++, 0, inp, *outp, outCnt << 1);
        aResample(ptr++, f->first, incr, osVirtualToPhysical(f->state));
        f->first = 0;
    }

    return ptr;
}

s32 alResampleParam(void *filter, s32 paramID, void *param)
{
    ALFilter *f = (ALFilter *)filter;
    ALResampler *r = (ALResampler *)filter;
    union {
        f32 f;
        s32 i;
    } data;

    switch (paramID) {
    case AL_FILTER_SET_SOURCE:
        f->source = (ALFilter *)param;
        break;

    case AL_FILTER_RESET:
        r->delta = 0.0;
        r->first = 1;
        r->motion = AL_STOPPED;
        r->upitch = 0;
        if (f->source)
            (*f->source->setParam)(f->source, AL_FILTER_RESET, 0);
        break;

    case AL_FILTER_START:
        r->motion = AL_PLAYING;
        if (f->source)
            (*f->source->setParam)(f->source, AL_FILTER_START, 0);
        break;

    case AL_FILTER_SET_PITCH:
        data.i = (s32)param;
        r->ratio = data.f;
        break;

    case AL_FILTER_SET_UNITY_PITCH:
        r->upitch = 1;
        break;

    default:
        if (f->source)
            (*f->source->setParam)(f->source, paramID, param);
        break;
    }
    return 0;
}
