#include "mus_channel.h"

int func_800FBDC8(int handle)
{
    int i;
    channel_t *cp;
    int *p;
    register channel_t *base asm("$2");

    if (handle != 0) {
        base = D_803AD97C;
        for (i = 0, cp = base; i < D_803AD974; i++, cp++) {
            if (cp->handle == handle) {
                p = cp->song_bank;
                if (p == 0)
                    p = cp->sample_bank;
                return p[2];
            }
        }
    }
    return 0;
}
