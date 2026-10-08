#include "os_types.h"
#include "n_synth_types.h"

typedef struct {
    u32 control_flag;          /* 0x00 */
    s32 channels;              /* 0x04 */
    void *sched;               /* 0x08 */
    s32 thread_priority;       /* 0x0C */
    u8 *heap;                  /* 0x10 */
    s32 heap_length;           /* 0x14 */
    u8 *ptr;                   /* 0x18 */
    u8 *wbk;                   /* 0x1C */
    void *default_fxbank;      /* 0x20 */
    s32 fifo_length;           /* 0x24 */
    s32 syn_updates;           /* 0x28 */
    s32 syn_output_rate;       /* 0x2C */
    s32 syn_rsp_cmds;          /* 0x30 */
    s32 syn_retraceCount;      /* 0x34 */
    s32 syn_num_dma_bufs;      /* 0x38 */
    s32 syn_dma_buf_size;      /* 0x3C */
} musConfig;

typedef struct {
    s16 *ptr;                  /* 0x00 */
    s32 len;                   /* 0x04 */
} audio_buf_t;

typedef struct {
    void (*init)(void);        /* 0x00 */
    void (*frame)(void);       /* 0x04 */
    void (*task)(u32 *info);   /* 0x08 */
} audio_callbacks_t;

typedef u64 Acmd;

extern N_ALGlobals D_803ADC30;
extern Acmd *D_803ADC28;
extern audio_buf_t *D_803ADC24;
extern void *D_803ADC20;
extern u8 D_803ADA70[];
extern u64 n_aspMainTextStart[];
extern u64 D_801262E0[];
extern audio_callbacks_t *D_8012686C;
extern audio_buf_t *D_80126870;

extern void *__MusIntMemMalloc(s32 size);
extern void func_800FF0D4(void);
extern void *func_800FEF60(s32 count, s32 size);
extern ALHeap *func_800FF9E4(void);
extern void func_800FE710(N_ALGlobals *g, ALSynConfig *c);
extern s32 func_800FF820(s32 retraceCount, s32 outputRate, s32 a2, s32 a3);
extern s32 func_800FF8C4(s32 samples);
extern Acmd *func_801025C0(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen);
extern s32 osAiSetFrequency(u32 frequency);
extern u32 osAiGetStatus(void);
extern u32 osAiGetLength(void);
extern s32 osAiSetNextBuffer(void *buf, u32 size);
extern void osCreateThread(void *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
extern void osStartThread(void *t);

void func_800FF698(void *arg);

void func_800FF560(musConfig *config, s32 a1, s32 fxType)
{
    ALSynConfig c;
    s32 frameSize;
    u32 i;
    u8 *stack;

    c.maxVVoices = c.maxPVoices = config->channels;
    c.maxUpdates = config->syn_updates;
    c.dmaproc = func_800FEF60(config->syn_num_dma_bufs, config->syn_dma_buf_size);
    c.fxType = fxType;
    c.outputRate = osAiSetFrequency(config->syn_output_rate);
    c.heap = func_800FF9E4();
    func_800FE710(&D_803ADC30, &c);
    frameSize = func_800FF820(config->syn_retraceCount, c.outputRate, a1, 15);
    D_803ADC28 = __MusIntMemMalloc(config->syn_rsp_cmds * sizeof(Acmd));
    D_803ADC24 = __MusIntMemMalloc(3 * sizeof(audio_buf_t));
    for (i = 0; i < 3; i++)
        D_803ADC24[i].ptr = __MusIntMemMalloc(frameSize * 4);
    stack = __MusIntMemMalloc(0x2000);
    D_803ADC20 = stack;
    osCreateThread(D_803ADA70, 3, func_800FF698, 0, stack + 0x2000, config->thread_priority);
    osStartThread(D_803ADA70);
}

void func_800FF698(void *arg)
{
    u32 info[4];
    s32 cmdLen;
    u32 status;
    u32 samples;
    u32 frame;
    audio_buf_t *buf;
    Acmd *cmdEnd;

    info[0] = (u32)D_803ADC28;
    info[2] = (u32)n_aspMainTextStart;
    info[3] = (u32)D_801262E0;
    frame = 0;
    D_8012686C->init();
    while (1) {
        D_8012686C->frame();
        status = osAiGetStatus();
        samples = osAiGetLength() >> 2;
        if (status & 0x80000000)
            continue;
        func_800FF0D4();
        if (D_80126870 && cmdLen)
            osAiSetNextBuffer(D_80126870->ptr, D_80126870->len << 2);
        buf = &D_803ADC24[frame];
        buf->len = func_800FF8C4(samples);
        cmdEnd = func_801025C0(D_803ADC28, &cmdLen, (s16 *)osVirtualToPhysical(buf->ptr), buf->len);
        if (cmdLen) {
            info[1] = (cmdEnd - D_803ADC28) * sizeof(Acmd);
            D_8012686C->task(info);
            D_80126870 = buf;
        }
        frame = (frame + 1) % 3;
    }
}
