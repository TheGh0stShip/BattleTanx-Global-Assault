/* RODATA_VRAM 0x800777E0: this unit's literal pool is linked at its retail address. */
#include "n_audio_private.h"

typedef union {
    struct {
        u32 w0;
        u32 w1;
    } words;
    long long force_structure_alignment;
} Acmd;

typedef s32 (*ALVoiceHandler)(void *);

extern s32 func_80101320();
extern s32 func_800FFA90();
extern void alN_PVoiceNew(N_PVoice *pv, void *dma, ALHeap *hp);
extern ALFx *func_80102980(s16 bus, ALSynConfig *c, ALHeap *hp);
extern Acmd *func_801028B0(s32 sampleOffset, Acmd *p);

s32 func_80102804(s32 micros);
void func_8010276C(void);

void n_alSynNew(ALSynConfig *c)
{
    s32 i;
    N_PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALParam *params;
    ALParam *paramPtr;

    n_syn->head = 0;
    n_syn->numPVoices = c->maxPVoices;
    n_syn->curSamples = 0;
    n_syn->paramSamples = 0;
    n_syn->outputRate = c->outputRate;
    n_syn->maxOutSamples = 184;
    n_syn->dma = c->dmaproc;
    n_syn->sv_dramout = 0;
    n_syn->sv_first = 1;

    n_syn->auxBus = alHeapAlloc(hp, 1, sizeof(N_ALAuxBus));
    n_syn->auxBus->sourceCount = 0;
    n_syn->auxBus->maxSources = c->maxPVoices;
    n_syn->auxBus->sources = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice *));

    n_syn->mainBus = alHeapAlloc(hp, 1, 0x14);

    if (c->fxType) {
        n_syn->auxBus->fx = func_80102980(0, c, hp);
        n_syn->mainBus->filter.handler = func_80101320;
    } else {
        n_syn->mainBus->filter.handler = func_800FFA90;
    }

    n_syn->pFreeList.next = 0;
    n_syn->pFreeList.prev = 0;
    n_syn->pLameList.next = 0;
    n_syn->pLameList.prev = 0;
    n_syn->pAllocList.next = 0;
    n_syn->pAllocList.prev = 0;

    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        N_PVoice *pv = &pvoices[i];
        alLink((ALLink *)pv, &n_syn->pFreeList);
        pv->vvoice = 0;
        alN_PVoiceNew(pv, n_syn->dma, hp);
        n_syn->auxBus->sources[n_syn->auxBus->sourceCount++] = pv;
    }

    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    n_syn->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = n_syn->paramList;
        n_syn->paramList = paramPtr;
    }
    n_syn->heap = hp;
}

Acmd *func_801025C0(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen)
{
    ALPlayer *client;
    Acmd *cmdlEnd = cmdList;
    s32 nOut;
    s16 *lOutBuf = outBuf;

    if (n_syn->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }

    client = n_syn->head;
    while (client->samplesLeft - n_syn->curSamples < outLen) {
        n_syn->paramSamples = client->samplesLeft & ~0xF;
        client->samplesLeft += func_80102804(((ALVoiceHandler)client->handler)(client));
    }
    n_syn->paramSamples = n_syn->paramSamples & ~0xF;

    while (outLen > 0) {
        nOut = n_syn->maxOutSamples;
        if (outLen < nOut)
            nOut = outLen;
        n_syn->sv_dramout = (s32)lOutBuf;
        cmdlEnd = func_801028B0(n_syn->curSamples, cmdlEnd);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        n_syn->curSamples += nOut;
    }
    *cmdLen = cmdlEnd - cmdList;
    func_8010276C();
    return cmdlEnd;
}

ALParam *__allocParam(void)
{
    ALParam *update = 0;

    if (n_syn->paramList) {
        update = n_syn->paramList;
        n_syn->paramList = n_syn->paramList->next;
        update->next = 0;
    }
    return update;
}

void __freeParam(ALParam *param)
{
    param->next = n_syn->paramList;
    n_syn->paramList = param;
}

void func_8010276C(void)
{
    ALLink *dl;

    while ((dl = n_syn->pLameList.next) != 0) {
        alUnlink(dl);
        alLink(dl, &n_syn->pFreeList);
    }
}

void _n_freePVoice(N_PVoice *pvoice)
{
    alUnlink((ALLink *)pvoice);
    alLink((ALLink *)pvoice, &n_syn->pLameList);
}

inline s32 func_80102804(s32 micros)
{
    f32 tmp = ((f32)micros) * n_syn->outputRate / 1000000.0 + 0.5;

    return (s32)tmp;
}

s32 func_80102854(s32 micros)
{
    return func_80102804(micros) & ~0xF;
}
