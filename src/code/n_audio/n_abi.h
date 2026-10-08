/* n_audio ABI command helpers (N64 big-endian, 64-bit command words). */
#ifndef N_ABI_H
#define N_ABI_H
#include "ultra_basic_types.h"

typedef union {
    struct {
        u32 w0;
        u32 w1;
    } words;
    long long force_structure_alignment;
} Acmd;

#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define _SHIFTR(v, s, w) ((unsigned int)(((unsigned int)(v) >> (s)) & ((0x01 << (w)) - 1)))

#define A_CLEARBUFF 2
#define A_SAVEBUFF 6
#define A_MIX 12
#define A_INTERLEAVE 13

#define FIXED_SAMPLE 184
#define N_AL_DIVIDED 0x170
#define N_AL_MAIN_L_OUT 0x4E0
#define N_AL_MAIN_R_OUT 0x650
#define N_AL_AUX_L_OUT 0x7C0
#define N_AL_AUX_R_OUT 0x930

#define aClearBuffer(pkt, d, c)                                         \
    {                                                                   \
        Acmd *_a = (Acmd *)pkt;                                         \
        _a->words.w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24); \
        _a->words.w1 = (unsigned int)(c);                               \
    }

#define n_aInterleave(pkt)                               \
    {                                                    \
        Acmd *_a = (Acmd *)pkt;                          \
        _a->words.w0 = _SHIFTL(A_INTERLEAVE, 24, 8);     \
    }

#define n_aSaveBuffer(pkt, c, d, s)                                                          \
    {                                                                                       \
        Acmd *_a = (Acmd *)pkt;                                                             \
        _a->words.w0 = (_SHIFTL(A_SAVEBUFF, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(d, 0, 12)); \
        _a->words.w1 = (unsigned int)(s);                                                   \
    }

#define n_aMix(pkt, f, g, i, o)                                                    \
    {                                                                             \
        Acmd *_a = (Acmd *)pkt;                                                   \
        _a->words.w0 = (_SHIFTL(A_MIX, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16)); \
        _a->words.w1 = _SHIFTL(i, 16, 16) | _SHIFTL(o, 0, 16);                    \
    }

#define A_ADPCM 1
#define A_ENVMIXER 3
#define A_LOADBUFF 4
#define A_RESAMPLE 5
#define A_SETVOL 9
#define A_DMEMMOVE 10
#define A_LOADADPCM 11
#define A_POLEF 14
#define A_SETLOOP 15

#define n_aLoadBuffer(pkt, c, d, s)                                                          \
    {                                                                                       \
        Acmd *_a = (Acmd *)pkt;                                                             \
        _a->words.w0 = (_SHIFTL(A_LOADBUFF, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(d, 0, 12)); \
        _a->words.w1 = (unsigned int)(s);                                                   \
    }

#define n_aDMEMMove(pkt, i, o, c)                                            \
    {                                                                       \
        Acmd *_a = (Acmd *)pkt;                                             \
        _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24);      \
        _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16);              \
    }

#define n_aResample(pkt, s, f, p, i, o)                                                    \
    {                                                                                     \
        Acmd *_a = (Acmd *)pkt;                                                           \
        _a->words.w0 = (_SHIFTL(A_RESAMPLE, 24, 8) | _SHIFTL(s, 0, 24));                  \
        _a->words.w1 = (_SHIFTL(f, 30, 2) | _SHIFTL(p, 14, 16) | _SHIFTL(i, 2, 12) |      \
                        _SHIFTL(o >> 8, 0, 2));                                           \
    }

#define n_aLoadADPCM(pkt, c, d)                                               \
    {                                                                        \
        Acmd *_a = (Acmd *)pkt;                                              \
        _a->words.w0 = _SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24);      \
        _a->words.w1 = (unsigned int)d;                                      \
    }

#define n_aPoleFilter(pkt, f, g, t, s)                                                    \
    {                                                                                    \
        Acmd *_a = (Acmd *)pkt;                                                          \
        _a->words.w0 = (_SHIFTL(A_POLEF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16)); \
        _a->words.w1 = (_SHIFTL(t >> 8, 24, 8) | _SHIFTL(s, 0, 24));                     \
    }
#endif
