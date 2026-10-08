/* IDOFLAGS: -O3 -mips2 */
#include "audio.h"
#include "abi.h"

#define AL_FILTER_RESET 4
#define AL_FILTER_SET_WAVETABLE 5
#define AL_ADPCM_WAVE 0
#define AL_RAW16_WAVE 1
#define AL_DECODER_IN 0
#define ADPCMFSIZE 16
#define ADPCMFBYTES 9
#define LFSAMPLES 4
#define ADPCMVSIZE 8
#define K0_TO_PHYS(x) ((u32)(x) & 0x1FFFFFFF)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
    u32 start;
    u32 end;
    u32 count;
    s16 state[16];
} ALADPCMloop;

typedef struct {
    s32 order;
    s32 npredictors;
    s16 book[1];
} ALADPCMBook;

typedef struct ALWaveTable_s {
    u8 *base;                  /* 0x00 */
    s32 len;                   /* 0x04 */
    u8 type;                   /* 0x08 */
    u8 flags;                  /* 0x09 */
    union {
        struct {
            ALADPCMloop *loop; /* 0x0C */
            ALADPCMBook *book; /* 0x10 */
        } adpcmWave;
        struct {
            ALRawLoop *loop;   /* 0x0C */
        } rawWave;
    } waveInfo;
} ALWaveTable;

extern void alCopy(void *src, void *dest, s32 len);

static Acmd *_decodeChunk(Acmd *ptr, ALLoadFilter *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);

Acmd *alAdpcmPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    s32 nbytes;
    s32 nframes;
    s32 tsam;
    s32 looped = 0;
    s32 startZero;
    s32 overFlow;
    s32 nOver;
    s32 nSam;
    s32 op;
    s32 nLeft;
    s32 bEnd;
    s32 decoded = 0;
    s16 inp;
    ALLoadFilter *f = (ALLoadFilter *)filter;

    if (outCount == 0)
        return ptr;

    inp = AL_DECODER_IN;
    aLoadADPCM(ptr++, f->bookSize, K0_TO_PHYS(f->table->waveInfo.adpcmWave.book->book));

    looped = (outCount + f->sample > f->loop.end) && (f->loop.count != 0);
    if (looped)
        nSam = f->loop.end - f->sample;
    else
        nSam = outCount;

    if (f->lastsam)
        nLeft = ADPCMFSIZE - f->lastsam;
    else
        nLeft = 0;
    tsam = nSam - nLeft;
    if (tsam < 0)
        tsam = 0;

    nframes = (tsam + ADPCMFSIZE - 1) >> LFSAMPLES;
    nbytes = nframes * ADPCMFBYTES;

    if (looped) {
        ptr = _decodeChunk(ptr, f, tsam, nbytes, *outp, inp, f->first);

        if (f->lastsam)
            *outp += (f->lastsam << 1);
        else
            *outp += (ADPCMFSIZE << 1);

        f->lastsam = f->loop.start & 0xf;
        f->memin = (s32)f->table->base + ADPCMFBYTES * ((s32)(f->loop.start >> LFSAMPLES) + 1);
        f->sample = f->loop.start;

        bEnd = *outp;
        while (outCount > nSam) {
            outCount -= nSam;
            op = (bEnd + ((nframes + 1) << (LFSAMPLES + 1))) & ~0x1f;
            bEnd += (nSam << 1);
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            nSam = MIN(outCount, f->loop.end - f->loop.start);
            tsam = nSam - ADPCMFSIZE + f->lastsam;
            if (tsam < 0)
                tsam = 0;
            nframes = (tsam + ADPCMFSIZE - 1) >> LFSAMPLES;
            nbytes = nframes * ADPCMFBYTES;
            ptr = _decodeChunk(ptr, f, tsam, nbytes, op, inp, f->first | A_LOOP);
            aDMEMMove(ptr++, op + (f->lastsam << 1), bEnd, nSam << 1);
        }

        f->lastsam = (outCount + f->lastsam) & 0xf;
        f->sample += outCount;
        f->memin += ADPCMFBYTES * nframes;
        return ptr;
    }

    nSam = nframes << LFSAMPLES;

    overFlow = f->memin + nbytes - ((s32)f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    nOver = (overFlow / ADPCMFBYTES) << LFSAMPLES;
    if (nOver > nSam + nLeft)
        nOver = nSam + nLeft;

    nbytes -= overFlow;

    if ((nOver - (nOver & 0xf)) < outCount) {
        decoded = 1;
        ptr = _decodeChunk(ptr, f, nSam - nOver, nbytes, *outp, inp, f->first);

        if (f->lastsam)
            *outp += (f->lastsam << 1);
        else
            *outp += (ADPCMFSIZE << 1);

        f->lastsam = (outCount + f->lastsam) & 0xf;
        f->sample += outCount;
        f->memin += ADPCMFBYTES * nframes;
    } else {
        f->lastsam = 0;
        f->memin += ADPCMFBYTES * nframes;
    }

    if (nOver) {
        f->lastsam = 0;
        if (decoded)
            startZero = (nLeft + nSam - nOver) << 1;
        else
            startZero = 0;
        aClearBuffer(ptr++, startZero + *outp, nOver << 1);
    }

    return ptr;
}

Acmd *alRaw16Pull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    s32 nbytes;
    s32 dramLoc;
    s32 dramAlign;
    s32 dmemAlign;
    s32 overFlow;
    s32 startZero;
    s32 nSam;
    s32 op;
    ALLoadFilter *f = (ALLoadFilter *)filter;

    if (outCount == 0)
        return ptr;

    if ((f->sample + outCount > f->loop.end) && (f->loop.count != 0)) {
        nSam = f->loop.end - f->sample;
        nbytes = nSam << 1;
        if (nSam > 0) {
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            aSetBuffer(ptr++, 0, *outp, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);
        } else {
            dramAlign = 0;
        }

        *outp += dramAlign;

        f->memin = (s32)f->table->base + (f->loop.start << 1);
        f->sample = f->loop.start;
        op = *outp;

        while (outCount > nSam) {
            op += (nSam << 1);
            outCount -= nSam;
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            nSam = MIN(outCount, f->loop.end - f->loop.start);
            nbytes = nSam << 1;
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            if (op & 0x7)
                dmemAlign = 8 - (op & 0x7);
            else
                dmemAlign = 0;
            aSetBuffer(ptr++, 0, op + dmemAlign, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);
            if (dramAlign || dmemAlign)
                aDMEMMove(ptr++, op + dramAlign + dmemAlign, op, nSam << 1);
        }

        f->sample += outCount;
        f->memin += (outCount << 1);
        return ptr;
    }

    nbytes = outCount << 1;
    overFlow = f->memin + nbytes - ((s32)f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    if (overFlow > nbytes)
        overFlow = nbytes;

    if (overFlow < nbytes) {
        if (outCount > 0) {
            nbytes -= overFlow;
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            aSetBuffer(ptr++, 0, *outp, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);
        } else {
            dramAlign = 0;
        }
        *outp += dramAlign;
        f->sample += outCount;
        f->memin += outCount << 1;
    } else {
        f->memin += outCount << 1;
    }

    if (overFlow) {
        startZero = (outCount << 1) - overFlow;
        if (startZero < 0)
            startZero = 0;
        aClearBuffer(ptr++, startZero + *outp, overFlow);
    }

    return ptr;
}

void alLoadParam(void *filter, s32 paramID, void *param)
{
    ALLoadFilter *a = (ALLoadFilter *)filter;
    ALFilter *f = (ALFilter *)filter;

    switch (paramID) {
    case AL_FILTER_SET_WAVETABLE:
        a->table = (ALWaveTable *)param;
        a->memin = (s32)a->table->base;
        a->sample = 0;
        switch (a->table->type) {
        case AL_ADPCM_WAVE:
            f->handler = alAdpcmPull;
            a->table->len = ADPCMFBYTES * ((s32)(a->table->len / ADPCMFBYTES));
            a->bookSize = 2 * a->table->waveInfo.adpcmWave.book->order *
                          a->table->waveInfo.adpcmWave.book->npredictors * ADPCMVSIZE;
            if (a->table->waveInfo.adpcmWave.loop) {
                a->loop.start = a->table->waveInfo.adpcmWave.loop->start;
                a->loop.end = a->table->waveInfo.adpcmWave.loop->end;
                a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                alCopy(a->table->waveInfo.adpcmWave.loop->state, a->lstate, 0x20);
            } else {
                a->loop.start = a->loop.end = a->loop.count = 0;
            }
            break;
        case AL_RAW16_WAVE:
            f->handler = alRaw16Pull;
            if (a->table->waveInfo.rawWave.loop) {
                a->loop.start = a->table->waveInfo.rawWave.loop->start;
                a->loop.end = a->table->waveInfo.rawWave.loop->end;
                a->loop.count = a->table->waveInfo.rawWave.loop->count;
            } else {
                a->loop.start = a->loop.end = a->loop.count = 0;
            }
            break;
        default:
            break;
        }
        break;
    case AL_FILTER_RESET:
        a->lastsam = 0;
        a->first = 1;
        a->sample = 0;
        if (a->table) {
            a->memin = (s32)a->table->base;
            if (a->table->type == AL_ADPCM_WAVE) {
                if (a->table->waveInfo.adpcmWave.loop)
                    a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
            } else if (a->table->type == AL_RAW16_WAVE) {
                if (a->table->waveInfo.rawWave.loop)
                    a->loop.count = a->table->waveInfo.rawWave.loop->count;
            }
        }
        break;
    default:
        break;
    }
}

static Acmd *_decodeChunk(Acmd *ptr, ALLoadFilter *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags)
{
    s32 dramAlign, dramLoc;

    if (nbytes > 0) {
        dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
        dramAlign = dramLoc & 0x7;
        nbytes += dramAlign;
        aSetBuffer(ptr++, 0, inp, 0, nbytes + 8 - (nbytes & 0x7));
        aLoadBuffer(ptr++, dramLoc - dramAlign);
    } else {
        dramAlign = 0;
    }

    if (flags & A_LOOP) {
        aSetLoop(ptr++, K0_TO_PHYS(f->lstate));
    }

    aSetBuffer(ptr++, 0, inp + dramAlign, outp, tsam << 1);
    aADPCMdec(ptr++, flags, K0_TO_PHYS(f->state));
    f->first = 0;

    return ptr;
}
