#ifndef N_WAVETABLE_H
#define N_WAVETABLE_H
#include "n_synth_types.h"

typedef struct {
    u32 start;
    u32 end;
    u32 count;
    s16 state[16];             /* 0x0C */
} ALADPCMloop;

typedef struct {
    s32 order;
    s32 npredictors;
    s16 book[1];               /* 0x08 */
} ALADPCMBook;

typedef struct {
    ALADPCMloop *loop;         /* 0x0C */
    ALADPCMBook *book;         /* 0x10 */
} ALADPCMWaveInfo;

typedef struct {
    ALRawLoop *loop;           /* 0x0C */
} ALRAWWaveInfo;

typedef struct ALWaveTable_s {
    u8 *base;                  /* 0x00 */
    s32 len;                   /* 0x04 */
    u8 type;                   /* 0x08 */
    u8 flags;                  /* 0x09 */
    union {
        ALADPCMWaveInfo adpcmWave;
        ALRAWWaveInfo rawWave;
    } waveInfo;                /* 0x0C */
} ALWaveTable;

#define K0_TO_PHYS(x) ((u32)(x) & 0x1FFFFFFF)
#endif
