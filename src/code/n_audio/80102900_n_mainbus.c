#include "n_audio_private.h"
#include "n_abi.h"

typedef Acmd *(*N_ALCmdHandler)(s32, Acmd *);

Acmd *func_80102900(s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;

    aClearBuffer(ptr++, N_AL_MAIN_L_OUT, N_AL_DIVIDED << 1);
    ptr = ((N_ALCmdHandler)n_syn->mainBus->filter.handler)(sampleOffset, ptr);
    n_aMix(ptr++, 0, 0x7FFF, N_AL_AUX_L_OUT, N_AL_MAIN_L_OUT);
    n_aMix(ptr++, 0, 0x7FFF, N_AL_AUX_R_OUT, N_AL_MAIN_R_OUT);
    return ptr;
}
