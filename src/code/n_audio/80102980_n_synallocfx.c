#include "n_audio_private.h"

extern void n_alFxNew(ALFx **fx_ar, ALSynConfig *c, ALHeap *hp);

ALFx *func_80102980(s16 bus, ALSynConfig *c, ALHeap *hp)
{
    n_alFxNew(&n_syn->auxBus->fx_array[bus], c, hp);
    return n_syn->auxBus->fx_array[bus];
}
