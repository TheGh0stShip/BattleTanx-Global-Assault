/* RODATA_VRAM 0x800776F0; DATA_VRAM 0x80126890.
 * Preset tables follow the SDK source retained by Dr. Mario 64. */
#include "n_audio_private.h"

#define RANGE 2.0
#define CONVERT 173123.404906676
#define LENGTH (d->output - d->input)

#define ms *(((s32)((f32)44.1)) & ~0x7)

static s32 SMALLROOM_PARAMS[26] = {
    3, 100 ms,
    0, 54 ms, 9830, -9830, 0, 0, 0, 0,
    19 ms, 38 ms, 3276, -3276, 0x3fff, 0, 0, 0,
    0, 60 ms, 5000, 0, 0, 0, 0, 0x5000
};
static s32 BIGROOM_PARAMS[34] = {
    4, 100 ms,
    0, 66 ms, 9830, -9830, 0, 0, 0, 0,
    22 ms, 54 ms, 3276, -3276, 0x3fff, 0, 0, 0,
    66 ms, 91 ms, 3276, -3276, 0x3fff, 0, 0, 0,
    0, 94 ms, 8000, 0, 0, 0, 0, 0x5000
};
static s32 ECHO_PARAMS[10] = {
    1, 200 ms, 0, 179 ms, 12000, 0, 0x7fff, 0, 0, 0
};
static s32 CHORUS_PARAMS[10] = {
    1, 20 ms, 0, 5 ms, 0x4000, 0, 0x7fff, 7600, 700, 0
};
static s32 FLANGE_PARAMS[10] = {
    1, 20 ms, 0, 5 ms, 0, 0x5fff, 0x7fff, 380, 500, 0
};
static s32 NULL_PARAMS[10] = { 0 };

typedef void *(*ALDMANew)(void *state);

extern void init_lpfilter(ALLowPass *lp);

void n_alFxNew(ALFx **fx_ar, ALSynConfig *c, ALHeap *hp)
{
    u16 i, j, k;
    s32 *param = 0;
    ALDelay *d;
    ALFx *r;

    *fx_ar = r = (ALFx *)alHeapAlloc(hp, 1, sizeof(ALFx));

    switch (c->fxType) {
    case 1:
        param = SMALLROOM_PARAMS;
        break;
    case 2:
        param = BIGROOM_PARAMS;
        break;
    case 5:
        param = ECHO_PARAMS;
        break;
    case 3:
        param = CHORUS_PARAMS;
        break;
    case 4:
        param = FLANGE_PARAMS;
        break;
    case 6:
        param = c->params;
        break;
    default:
        param = NULL_PARAMS;
        break;
    }

    j = 0;

    r->section_count = param[j++];
    r->length = param[j++];

    r->delay = alHeapAlloc(hp, r->section_count, sizeof(ALDelay));
    r->base = alHeapAlloc(hp, r->length, sizeof(s16));
    r->input = r->base;

    for (k = 0; k < r->length; k++)
        r->base[k] = 0;

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];

        if (param[j]) {
            d->rsinc = ((((f32)param[j++]) / 1000) * RANGE) / c->outputRate;
            d->rsgain = (((f32)param[j++]) / CONVERT) * LENGTH;
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = alHeapAlloc(hp, 1, sizeof(ALResampler));
            d->rs->state = alHeapAlloc(hp, 1, 0x20);
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (param[j]) {
            d->lp = alHeapAlloc(hp, 1, sizeof(ALLowPass));
            d->lp->fstate = alHeapAlloc(hp, 1, 8);
            d->lp->fc = param[j++];
            init_lpfilter(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}

void alN_PVoiceNew(N_PVoice *mv, ALDMANew dmaNew, ALHeap *hp)
{
    mv->dc_state = alHeapAlloc(hp, 1, 0x20);
    mv->dc_lstate = alHeapAlloc(hp, 1, 0x20);
    mv->dc_dma = dmaNew(&mv->dc_dmaState);
    mv->dc_lastsam = 0;
    mv->dc_first = 1;
    mv->dc_memin = 0;

    mv->rs_state = alHeapAlloc(hp, 1, 0x20);
    mv->rs_delta = 0.0;
    mv->rs_first = 1;
    mv->rs_ratio = 1.0;
    mv->rs_upitch = 0;

    mv->em_state = alHeapAlloc(hp, 1, 0x50);
    mv->em_first = 1;
    mv->em_motion = 0;
    mv->em_volume = 1;
    mv->em_ltgt = 1;
    mv->em_rtgt = 1;
    mv->em_cvolL = 1;
    mv->em_cvolR = 1;
    mv->em_dryamt = 0;
    mv->em_wetamt = 0;
    mv->em_lratm = 1;
    mv->em_lratl = 0;
    mv->em_lratm = 1;
    mv->em_lratl = 0;
    mv->em_delta = 0;
    mv->em_segEnd = 0;
    mv->em_pan = 0;
    mv->em_ctrlList = 0;
    mv->em_ctrlTail = 0;
}
