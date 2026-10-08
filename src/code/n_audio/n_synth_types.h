/* Minimal libultra n_audio layouts used by the 0x800FE710-0x800FFB30 units.
   Offsets checked against the retail code; widths follow the N64 o32 ABI. */
#ifndef N_SYNTH_TYPES_H
#define N_SYNTH_TYPES_H

#include "ultra_basic_types.h"

typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;

typedef struct {
    void *base;
    void *cur;
    s32 len;
    s32 count;
} ALHeap;

typedef struct ALFilter_s {
    struct ALFilter_s *source; /* 0x00 */
    void *handler;             /* 0x04 */
    void *setParam;            /* 0x08 */
    s16 inp;                   /* 0x0C */
    s16 outp;                  /* 0x0E */
    s32 type;                  /* 0x10 */
} ALFilter;

typedef struct {
    s16 fc;                    /* 0x00 */
    s16 fgain;                 /* 0x02 */
    union {
        s16 fccoef[16];
        long long force_aligned;
    } fcvec;                   /* 0x08 */
    void *fstate;              /* 0x28 */
    s32 first;                 /* 0x2C */
} ALLowPass;

typedef struct {
    ALFilter filter;           /* 0x00 */
    void *state;               /* 0x14 */
    f32 ratio;                 /* 0x18 */
    s32 upitch;                /* 0x1C */
    f32 delta;                 /* 0x20 */
    s32 first;                 /* 0x24 */
    void *ctrlList;            /* 0x28 */
    void *ctrlTail;            /* 0x2C */
    s32 motion;                /* 0x30 */
} ALResampler;

typedef struct {
    u32 input;                 /* 0x00 */
    u32 output;                /* 0x04 */
    s16 ffcoef;                /* 0x08 */
    s16 fbcoef;                /* 0x0A */
    s16 gain;                  /* 0x0C */
    f32 rsinc;                 /* 0x10 */
    f32 rsval;                 /* 0x14 */
    s32 rsdelta;               /* 0x18 */
    f32 rsgain;                /* 0x1C */
    ALLowPass *lp;             /* 0x20 */
    ALResampler *rs;           /* 0x24 */
} ALDelay;

typedef struct {
    ALFilter filter;           /* 0x00 */
    s16 *base;                 /* 0x14 */
    s16 *input;                /* 0x18 */
    u32 length;                /* 0x1C */
    ALDelay *delay;            /* 0x20 */
    u8 section_count;          /* 0x24 */
    void *paramHdl;            /* 0x28 */
} ALFx;

typedef struct {
    s32 maxVVoices;            /* 0x00 */
    s32 maxPVoices;            /* 0x04 */
    s32 maxUpdates;            /* 0x08 */
    s32 maxFXbusses;           /* 0x0C */
    void *dmaproc;             /* 0x10 */
    ALHeap *heap;              /* 0x14 */
    s32 outputRate;            /* 0x18 */
    u8 fxType;                 /* 0x1C */
    s32 *params;               /* 0x20 */
} ALSynConfig;

typedef struct ALParam_s {
    struct ALParam_s *next;    /* 0x00 */
    s32 delta;
    s16 type;
    union { f32 f; s32 i; } data;
    union { f32 f; s32 i; } moredata;
    union { f32 f; s32 i; } stillmoredata;
    union { f32 f; s32 i; } yetstillmoredata;
} ALParam;                     /* 0x1C */

typedef struct {
    u32 start;
    u32 end;
    u32 count;
} ALRawLoop;

typedef struct N_PVoice_s {
    ALLink node;               /* 0x00 */
    struct N_ALVoice_s *vvoice; /* 0x08 */
    void *dc_state;            /* 0x0C */
    void *dc_lstate;           /* 0x10 */
    ALRawLoop dc_loop;         /* 0x14 */
    struct ALWaveTable_s *dc_table; /* 0x20 */
    s32 dc_bookSize;           /* 0x24 */
    void *dc_dma;              /* 0x28 */
    void *dc_dmaState;         /* 0x2C */
    s32 dc_sample;             /* 0x30 */
    s32 dc_lastsam;            /* 0x34 */
    s32 dc_first;              /* 0x38 */
    s32 dc_memin;              /* 0x3C */
    void *rs_state;            /* 0x40 */
    f32 rs_ratio;              /* 0x44 */
    s32 rs_upitch;             /* 0x48 */
    f32 rs_delta;              /* 0x4C */
    s32 rs_first;              /* 0x50 */
    void *em_state;            /* 0x54 */
    s16 em_pan;                /* 0x58 */
    s16 em_volume;             /* 0x5A */
    s16 em_cvolL;              /* 0x5C */
    s16 em_cvolR;              /* 0x5E */
    s16 em_dryamt;             /* 0x60 */
    s16 em_wetamt;             /* 0x62 */
    u16 em_lratl;              /* 0x64 */
    s16 em_lratm;              /* 0x66 */
    s16 em_ltgt;               /* 0x68 */
    u16 em_rratl;              /* 0x6A */
    s16 em_rratm;              /* 0x6C */
    s16 em_rtgt;               /* 0x6E */
    s32 em_delta;              /* 0x70 */
    s32 em_segEnd;             /* 0x74 */
    s32 em_first;              /* 0x78 */
    struct ALParam_s *em_ctrlList; /* 0x7C */
    struct ALParam_s *em_ctrlTail; /* 0x80 */
    s32 em_motion;             /* 0x84 */
    s32 offset;                /* 0x88 */
} N_PVoice;                    /* 0x8C */

typedef struct {
    ALFilter filter;           /* 0x00 */
    s32 sourceCount;           /* 0x14 */
    s32 maxSources;            /* 0x18 */
    N_PVoice **sources;        /* 0x1C */
    ALFx *fx;                  /* 0x20 */
    ALFx *fx_array[8];         /* 0x24 */
} N_ALAuxBus;                  /* 0x44 */

typedef struct {
    ALFilter filter;           /* 0x00 */
} N_ALMainBus;

typedef struct {
    void *head;                /* 0x00 */
    ALLink pFreeList;          /* 0x04 */
    ALLink pAllocList;         /* 0x0C */
    ALLink pLameList;          /* 0x14 */
    s32 paramSamples;          /* 0x1C */
    s32 curSamples;            /* 0x20 */
    void *dma;                 /* 0x24 */
    ALHeap *heap;              /* 0x28 */
    ALParam *paramList;        /* 0x2C */
    N_ALMainBus *mainBus;      /* 0x30 */
    N_ALAuxBus *auxBus;        /* 0x34 */
    s32 numPVoices;            /* 0x38 */
    s32 maxAuxBusses;          /* 0x3C */
    s32 outputRate;            /* 0x40 */
    s32 maxOutSamples;         /* 0x44 */
    s32 sv_dramout;            /* 0x48 */
    s32 sv_first;              /* 0x4C */
} N_ALSynth;

typedef struct {
    N_ALSynth drvr;
} N_ALGlobals;

extern void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
#define alHeapAlloc(hp, num, size) alHeapDBAlloc(0, 0, hp, num, size)
extern void alLink(ALLink *ln, ALLink *to);

#endif
