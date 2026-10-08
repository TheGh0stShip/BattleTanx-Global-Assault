/* RODATA_VRAM 0x800776E0: this unit's literal pool is linked at its retail address. */
#include "n_synth_types.h"

extern N_ALGlobals *n_alGlobals;
extern N_ALSynth *alGlobals;
extern N_ALSynth *D_803AD9E0;
extern struct {
    ALDelay *delay;
    s16 *base;
    ALResampler *rs;
    void *rs_state;
    ALLowPass *lp;
    void *lp_state;
} D_803AD9F0;
extern s32 D_801266B8[];
extern s32 D_80126840;
extern s32 *D_80126844[];
extern s32 alFxParamHdl();
extern s32 alFxParam(void *filter, s32 paramID, void *param);
extern void init_lpfilter(ALLowPass *lp);
extern s32 func_80101320();
extern s32 func_800FFA90();
extern void alN_PVoiceNew(N_PVoice *pv, void *dma, ALHeap *hp);

void func_800FEE60(ALSynConfig *c);
void func_800FE778(ALSynConfig *c);
ALFx *func_800FE9E8(s16 bus, ALSynConfig *c, ALHeap *hp);
void func_800FEA38(ALFx **fx_ar, ALSynConfig *c, ALHeap *hp);
void func_800FEB80(s32 *param);
s32 func_800FEE78(void);
s32 func_800FEEBC(void);

void func_800FE710(N_ALGlobals *g, ALSynConfig *c)
{
    if (!n_alGlobals) {
        n_alGlobals = g;
        if (!alGlobals) {
            func_800FEE60(c);
            D_803AD9E0 = &n_alGlobals->drvr;
            alGlobals = &n_alGlobals->drvr;
            func_800FE778(c);
        }
    }
}

void func_800FE778(ALSynConfig *c)
{
    s32 i;
    N_PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALParam *params;
    ALParam *paramPtr;

    alGlobals->head = 0;
    alGlobals->numPVoices = c->maxPVoices;
    alGlobals->curSamples = 0;
    alGlobals->paramSamples = 0;
    alGlobals->outputRate = c->outputRate;
    alGlobals->maxOutSamples = 184;
    alGlobals->dma = c->dmaproc;
    alGlobals->sv_dramout = 0;
    alGlobals->sv_first = 1;

    alGlobals->auxBus = alHeapAlloc(hp, 1, sizeof(N_ALAuxBus));
    alGlobals->auxBus->sourceCount = 0;
    alGlobals->auxBus->maxSources = c->maxPVoices;
    alGlobals->auxBus->sources = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice *));

    alGlobals->mainBus = alHeapAlloc(hp, 1, 0x14);

    if (c->fxType) {
        alGlobals->auxBus->fx = func_800FE9E8(0, c, hp);
        alGlobals->mainBus->filter.handler = func_80101320;
    } else {
        alGlobals->mainBus->filter.handler = func_800FFA90;
    }

    alGlobals->pFreeList.next = 0;
    alGlobals->pFreeList.prev = 0;
    alGlobals->pLameList.next = 0;
    alGlobals->pLameList.prev = 0;
    alGlobals->pAllocList.next = 0;
    alGlobals->pAllocList.prev = 0;

    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        N_PVoice *pv = &pvoices[i];
        alLink((ALLink *)pv, &alGlobals->pFreeList);
        pv->vvoice = 0;
        alN_PVoiceNew(pv, alGlobals->dma, hp);
        alGlobals->auxBus->sources[alGlobals->auxBus->sourceCount++] = pv;
    }

    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    alGlobals->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = alGlobals->paramList;
        alGlobals->paramList = paramPtr;
    }
    alGlobals->heap = hp;
}

ALFx *func_800FE9E8(s16 bus, ALSynConfig *c, ALHeap *hp)
{
    func_800FEA38(&alGlobals->auxBus->fx_array[bus], c, hp);
    return alGlobals->auxBus->fx_array[bus];
}

void func_800FEA38(ALFx **fx_ar, ALSynConfig *c, ALHeap *hp)
{
    ALFx *r;

    *fx_ar = r = alHeapAlloc(hp, 1, sizeof(ALFx));
    r->paramHdl = alFxParamHdl;
    D_803AD9F0.delay = alHeapAlloc(hp, func_800FEE78(), sizeof(ALDelay));
    D_803AD9F0.base = alHeapAlloc(hp, func_800FEEBC(), sizeof(s16));
    D_803AD9F0.rs = alHeapAlloc(hp, 1, sizeof(ALResampler));
    D_803AD9F0.rs_state = alHeapAlloc(hp, 1, 0x20);
    D_803AD9F0.lp = alHeapAlloc(hp, 1, sizeof(ALLowPass));
    D_803AD9F0.lp_state = alHeapAlloc(hp, 1, 8);
    func_800FEB80(c->params);
}

#define RANGE 2.0
#define CONVERT 173123.404906676
#define LENGTH (d->output - d->input)

void func_800FEB80(s32 *param)
{
    u16 i, j;
    ALFx *r = D_803AD9E0->auxBus->fx_array[0];
    ALDelay *d;

    j = 0;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = D_803AD9F0.delay;
    r->base = D_803AD9F0.base;
    r->input = r->base;

    for (i = 0; i < r->length; i++)
        r->base[i] = 0;

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];

        if (param[j]) {
            d->rsinc = ((((f32)param[j++]) / 1000) * RANGE) / D_803AD9E0->outputRate;
            d->rsgain = (((f32)param[j++]) / CONVERT) * LENGTH;
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = D_803AD9F0.rs;
            d->rs->state = D_803AD9F0.rs_state;
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (param[j]) {
            d->lp = D_803AD9F0.lp;
            d->lp->fstate = D_803AD9F0.lp_state;
            d->lp->fc = param[j++];
            init_lpfilter(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}

void func_800FEE60(ALSynConfig *c)
{
    c->fxType = 6;
    c->params = D_801266B8;
}

s32 func_800FEE78(void)
{
    s32 i = 0;
    s32 max = 0;

    while (D_80126844[i]) {
        if (max < D_80126844[i][0])
            max = D_80126844[i][0];
        i++;
    }
    return max;
}

s32 func_800FEEBC(void)
{
    s32 i = 0;
    s32 max = 0;

    while (D_80126844[i]) {
        if (max < D_80126844[i][1])
            max = D_80126844[i][1];
        i++;
    }
    return max;
}

s32 ChangeCustomEffect(s32 type)
{
    if (type >= D_80126840)
        return 1;
    func_800FEB80(D_80126844[type]);
    alFxParam(D_803AD9E0->auxBus->fx, 1, D_803AD9E0->auxBus);
    return 0;
}
