/* Data reconstruction: n_audio n_sl.c globals n_alGlobals / n_syn (n_syn is spelled alGlobals in this repo via n_audio_private.h) */
#include "n_audio_private.h"

N_ALGlobals *n_alGlobals = 0;
N_ALSynth *n_syn = 0;

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
