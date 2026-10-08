#include "n_audio_private.h"

extern void n_alSynNew(ALSynConfig *c);
extern void n_alSynDelete(void);

void n_alInit(N_ALGlobals *g, ALSynConfig *c)
{
    if (!n_alGlobals) {
        n_alGlobals = g;
        if (!n_syn) {
            n_syn = &n_alGlobals->drvr;
            n_alSynNew(c);
        }
    }
}

void n_alClose(N_ALGlobals *glob)
{
    if (n_alGlobals) {
        n_alSynDelete();
        n_alGlobals = 0;
        n_syn = 0;
    }
}
