/* Source shape adapted from the MIT-licensed Dr. Mario 64 libultra 2.0I. */
#include "audio.h"
#include "abi.h"

#define RANGE 2.0
#define AL_FILTER_SET_SOURCE 1
#define AL_AUX_L_OUT 1728
#define AL_AUX_R_OUT 2048
#define AL_TEMP_0 0
#define AL_TEMP_1 320
#define AL_TEMP_2 640
#define UNITY_PITCH 0x8000
#define SWAP(in, out) \
    { \
        s16 t = out; \
        out = in; \
        in = t; \
    }

typedef struct {
    u8 pad[0x44];
    s32 outputRate;
} ReverbSynth;

typedef struct {
    ReverbSynth drvr;
} ReverbGlobals;

extern ReverbGlobals *alGlobals_80126ED0;
#define alGlobals alGlobals_80126ED0
extern u32 osVirtualToPhysical(void *);
extern void init_lpfilter(ALLowPass *lp);
extern f32 _doModFunc(ALDelay *d, s32 count);

Acmd *_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_filterBuffer(ALLowPass *lp, s32 buff, s32 count, Acmd *p);
Acmd *_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p);

Acmd *alFxPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset,
               Acmd *p)
{
    Acmd *ptr = p;
    ALFx *r = (ALFx *)filter;
    ALFilter *source = r->filter.source;
    s16 i, buff1, buff2, input, output;
    s16 *in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
    ALDelay *d, *pd;

    ((void)0);
    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);
    input = AL_AUX_L_OUT;
    output = AL_AUX_R_OUT;
    buff1 = AL_TEMP_0;
    buff2 = AL_TEMP_1;

    aSetBuffer(ptr++, 0, 0, 0, outCount << 1);
    aMix(ptr++, 0, 0xda83, AL_AUX_L_OUT, input);
    aMix(ptr++, 0, 0x5a82, AL_AUX_R_OUT, input);
    ptr = _saveBuffer(r, r->input, input, outCount, ptr);
    aClearBuffer(ptr++, output, outCount << 1);

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];
        if (in_ptr == prev_out_ptr) {
            SWAP(buff1, buff2);
        } else {
            ptr = _loadBuffer(r, in_ptr, buff1, outCount, ptr);
        }
        ptr = _loadOutputBuffer(r, d, buff2, outCount, ptr);
        if (d->ffcoef) {
            aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
            if (!d->rs && !d->lp)
                ptr = _saveBuffer(r, out_ptr, buff2, outCount, ptr);
        }
        if (d->fbcoef) {
            aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
            ptr = _saveBuffer(r, in_ptr, buff1, outCount, ptr);
        }
        if (d->lp)
            ptr = _filterBuffer(d->lp, buff2, outCount, ptr);
        if (!d->rs)
            ptr = _saveBuffer(r, out_ptr, buff2, outCount, ptr);
        if (d->gain)
            aMix(ptr++, 0, (u16)d->gain, buff2, output);
        prev_out_ptr = &r->input[d->output];
    }

    r->input += outCount;
    if (r->input > &r->base[r->length])
        r->input -= r->length;
    aDMEMMove(ptr++, output, AL_AUX_L_OUT, outCount << 1);
    return ptr;
}
s32 alFxParam(void *filter, s32 paramID, void *param)
{
    if (paramID == AL_FILTER_SET_SOURCE) {
        ALFilter *f = (ALFilter *)filter;
        f->source = (ALFilter *)param;
    }
    return 0;
}

s32 alFxParamHdl(void *filter, s32 paramID, void *param)
{
    ALFx *f = (ALFx *)filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;

    switch (p) {
    case 0:
        f->delay[s].input = (u32)val & 0xFFFFFFF8;
        break;
    case 1:
        f->delay[s].output = (u32)val & 0xFFFFFFF8;
        break;
    case 3:
        f->delay[s].ffcoef = (s16)val;
        break;
    case 2:
        f->delay[s].fbcoef = (s16)val;
        break;
    case 4:
        f->delay[s].gain = (s16)val;
        break;
    case 5:
        f->delay[s].rsinc = ((((f32)val) / 1000) * RANGE) /
                            alGlobals->drvr.outputRate;
        break;
    case 6:
        f->delay[s].rsgain = (((f32)val) / 173123.404906676) *
                             (f->delay[s].output - f->delay[s].input);
        break;
    case 7:
        if (f->delay[s].lp) {
            f->delay[s].lp->fc = (s16)val;
            init_lpfilter(f->delay[s].lp);
        }
        break;
    }
    return 0;
}

Acmd *_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p)
{
    Acmd *ptr = p;
    s32 ratio;
    s32 count;
    s32 rbuff = AL_TEMP_2;
    s16 *out_ptr;
    f32 fincount;
    f32 fratio;
    f32 delta;
    s32 ramalign = 0;
    s32 length;

    if (d->rs) {
        length = d->output - d->input;
        delta = _doModFunc(d, incount);
        delta /= length;
        delta = (s32)(delta * UNITY_PITCH);
        delta = delta / UNITY_PITCH;
        fratio = 1.0 - delta;
        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;
        out_ptr = &r->input[-(d->output - d->rsdelta)];
        ramalign = ((s32)out_ptr & 7) >> 1;
        ptr = _loadBuffer(r, out_ptr - ramalign, rbuff,
                          count + ramalign, ptr);
        ratio = (s32)(fratio * UNITY_PITCH);
        aSetBuffer(ptr++, 0, rbuff + (ramalign << 1), buff, incount << 1);
        aResample(ptr++, d->rs->first, ratio,
                  osVirtualToPhysical(d->rs->state));
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        out_ptr = &r->input[-d->output];
        ptr = _loadBuffer(r, out_ptr, buff, incount, ptr);
    }
    return ptr;
}

Acmd *_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    Acmd *ptr = p;
    s32 after_end;
    s32 before_end;
    s16 *updated_ptr;
    s16 *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)
        curr_ptr += r->length;
    updated_ptr = curr_ptr + count;

    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        aSetBuffer(ptr++, 0, buff, 0, before_end << 1);
        aLoadBuffer(ptr++, osVirtualToPhysical(curr_ptr));
        aSetBuffer(ptr++, 0, buff + (before_end << 1), 0, after_end << 1);
        aLoadBuffer(ptr++, osVirtualToPhysical(r->base));
    } else {
        aSetBuffer(ptr++, 0, buff, 0, count << 1);
        aLoadBuffer(ptr++, osVirtualToPhysical(curr_ptr));
    }
    aSetBuffer(ptr++, 0, 0, 0, count << 1);
    return ptr;
}

Acmd *_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    Acmd *ptr = p;
    s32 after_end;
    s32 before_end;
    s16 *updated_ptr;
    s16 *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)
        curr_ptr += r->length;
    updated_ptr = curr_ptr + count;

    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        aSetBuffer(ptr++, 0, 0, buff, before_end << 1);
        aSaveBuffer(ptr++, osVirtualToPhysical(curr_ptr));
        aSetBuffer(ptr++, 0, 0, buff + (before_end << 1), after_end << 1);
        aSaveBuffer(ptr++, osVirtualToPhysical(r->base));
        aSetBuffer(ptr++, 0, 0, 0, count << 1);
    } else {
        aSetBuffer(ptr++, 0, 0, buff, count << 1);
        aSaveBuffer(ptr++, osVirtualToPhysical(curr_ptr));
    }
    return ptr;
}

Acmd *_filterBuffer(ALLowPass *lp, s32 buff, s32 count, Acmd *p)
{
    Acmd *ptr = p;

    aSetBuffer(ptr++, 0, buff, buff, count << 1);
    aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
    aPoleFilter(ptr++, lp->first, lp->fgain,
                osVirtualToPhysical(lp->fstate));
    lp->first = 0;
    return ptr;
}

f32 _doModFunc(ALDelay *d, s32 count)
{
    f32 val;

    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > RANGE) ? d->rsval - (RANGE * 2) : d->rsval;
    val = d->rsval;
    val = (val < 0) ? -val : val;
    val -= RANGE / 2;
    return(d->rsgain * val);
}

