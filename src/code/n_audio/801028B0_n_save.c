#include "n_audio_private.h"
#include "n_abi.h"

extern Acmd *func_80102900(s32 sampleOffset, Acmd *p);

Acmd *func_801028B0(s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;

    ptr = func_80102900(sampleOffset, ptr);
    n_aInterleave(ptr++);
    n_aSaveBuffer(ptr++, FIXED_SAMPLE << 2, 0, n_syn->sv_dramout);
    return ptr;
}
