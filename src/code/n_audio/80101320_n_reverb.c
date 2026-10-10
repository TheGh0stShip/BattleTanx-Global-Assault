/* Data reconstruction: n_audio n_reverb.c _n_loadOutputBuffer statics (val/lastval/blob), as in monde-lointain/mariogolf64 5014056 src/libnaudio/n_reverb.c:243-245 (MIT); unused by the code, emitted as .data */
/* RODATA_VRAM 0x800777A0: this unit's switch table and literal pool are linked at their retail address. */
#include "n_audio_private.h"
#include "n_abi.h"

#define N_AL_TEMP_0 0x000
#define N_AL_TEMP_1 0x170
#define N_AL_TEMP_2 0x2E0
#define UNITY_PITCH 0x8000
#define RANGE 2.0
#define CONVERT 173123.404906676
#define LENGTH (f->delay[s].output - f->delay[s].input)
#define SWAP(in, out) \
    {                 \
        s16 t = out;  \
        out = in;     \
        in = t;       \
    }

extern Acmd *func_800FFA90(s32 sampleOffset, Acmd *p);
extern f32 _doModFunc(ALDelay *d, s32 count);
extern void init_lpfilter(ALLowPass *lp);

Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, Acmd *p);
Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p);
Acmd *_n_filterBuffer(ALLowPass *lp, s32 buff, Acmd *p);

Acmd *func_80101320(s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALFx *r = n_syn->auxBus->fx;
    s16 i, buff1, buff2, input, output;
    s16 *in_ptr, *out_ptr, *prev_out_ptr = 0;
    ALDelay *d;

    ptr = func_800FFA90(sampleOffset, p);

    input = N_AL_AUX_L_OUT;
    output = N_AL_AUX_R_OUT;
    buff1 = N_AL_TEMP_0;
    buff2 = N_AL_TEMP_1;

    n_aMix(ptr++, 0, 0xDA83, N_AL_AUX_L_OUT, input);
    n_aMix(ptr++, 0, 0x5A82, N_AL_AUX_R_OUT, input);

    ptr = _n_saveBuffer(r, r->input, input, ptr);

    aClearBuffer(ptr++, output, N_AL_DIVIDED);

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];

        if (in_ptr == prev_out_ptr) {
            SWAP(buff1, buff2);
        } else {
            ptr = _n_loadBuffer(r, in_ptr, buff1, FIXED_SAMPLE, ptr);
        }
        ptr = _n_loadOutputBuffer(r, d, buff2, ptr);

        if (d->ffcoef) {
            n_aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
            if (!d->rs && !d->lp)
                ptr = _n_saveBuffer(r, out_ptr, buff2, ptr);
        }

        if (d->fbcoef) {
            n_aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
            ptr = _n_saveBuffer(r, in_ptr, buff1, ptr);
        }

        if (d->lp)
            ptr = _n_filterBuffer(d->lp, buff2, ptr);

        if (!d->rs)
            ptr = _n_saveBuffer(r, out_ptr, buff2, ptr);

        if (d->gain)
            n_aMix(ptr++, 0, (u16)d->gain, buff2, output);

        prev_out_ptr = &r->input[d->output];
    }

    r->input += FIXED_SAMPLE;
    if (r->input > &r->base[r->length])
        r->input -= r->length;

    n_aDMEMMove(ptr++, output, N_AL_AUX_L_OUT, N_AL_DIVIDED);
    return ptr;
}

s32 func_80101630(void *filter, s32 paramID, void *param)
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
        f->delay[s].rsinc = ((((f32)val) / 1000) * RANGE) / n_syn->outputRate;
        break;
    case 6:
        f->delay[s].rsgain = (((f32)val) / CONVERT) * LENGTH;
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

Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, Acmd *p)
{
    Acmd *ptr = p;
    s32 ratio, count, rbuff = N_AL_TEMP_2;
    s16 *out_ptr;
    f32 fincount, fratio, delta;
    s32 ramalign = 0, length;
    s32 incount = FIXED_SAMPLE;
    static f32 val = 0.0;
    static f32 lastval = -10.0;
    static f32 blob = 0;

    if (d->rs) {
        length = d->output - d->input;
        delta = _doModFunc(d, FIXED_SAMPLE);
        delta /= length;
        delta = (s32)(delta * UNITY_PITCH);
        delta = delta / UNITY_PITCH;
        fratio = 1.0 - delta;

        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;

        out_ptr = &r->input[-(d->output - d->rsdelta)];
        ramalign = ((s32)out_ptr & 0x7) >> 1;
        ptr = _n_loadBuffer(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);
        ratio = (s32)(fratio * UNITY_PITCH);
        {
            Acmd *_a = ptr++;
            u32 outBits;

            _a->words.w0 = _SHIFTL(A_RESAMPLE, 24, 8) |
                           _SHIFTL(osVirtualToPhysical(d->rs->state), 0, 24);
            outBits = buff >> 8;
            _a->words.w1 = _SHIFTL(d->rs->first, 30, 2) |
                           _SHIFTL(ratio, 14, 16) |
                           _SHIFTL(rbuff + (ramalign << 1), 2, 12) |
                           _SHIFTL(outBits, 0, 2);
        }
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        out_ptr = &r->input[-d->output];
        ptr = _n_loadBuffer(r, out_ptr, buff, FIXED_SAMPLE, ptr);
    }
    return ptr;
}

Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    Acmd *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];

    if (curr_ptr < r->base)
        curr_ptr += r->length;
    updated_ptr = curr_ptr + count;

    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;

        n_aLoadBuffer(ptr++, before_end << 1, buff, osVirtualToPhysical(curr_ptr));
        n_aLoadBuffer(ptr++, after_end << 1, buff + (before_end << 1), osVirtualToPhysical(r->base));
    } else {
        n_aLoadBuffer(ptr++, count << 1, buff, osVirtualToPhysical(curr_ptr));
    }
    return ptr;
}

Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p)
{
    Acmd *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)
        curr_ptr += r->length;
    updated_ptr = curr_ptr + FIXED_SAMPLE;

    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;

        n_aSaveBuffer(ptr++, before_end << 1, buff, osVirtualToPhysical(curr_ptr));
        n_aSaveBuffer(ptr++, after_end << 1, buff + (before_end << 1), osVirtualToPhysical(r->base));
    } else {
        n_aSaveBuffer(ptr++, FIXED_SAMPLE << 1, buff, osVirtualToPhysical(curr_ptr));
    }
    return ptr;
}

Acmd *_n_filterBuffer(ALLowPass *lp, s32 buff, Acmd *p)
{
    Acmd *ptr = p;
    s32 t = buff >> 8;

    n_aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
    {
        Acmd *_a = ptr++;
        _a->words.w0 = (_SHIFTL(A_POLEF, 24, 8) | _SHIFTL(lp->first, 16, 8) | _SHIFTL(lp->fgain, 0, 16));
        _a->words.w1 = (_SHIFTL(t, 24, 8) | _SHIFTL(osVirtualToPhysical(lp->fstate), 0, 24));
    }
    lp->first = 0;
    return ptr;
}
