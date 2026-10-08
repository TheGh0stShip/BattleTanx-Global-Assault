#include "mus_channel.h"

extern channel_t *D_803AD980;
extern int D_803AD990;
extern int D_803AD998;
extern void func_800FD4CC(channel_t *cp);
void func_800FDCCC(int handle, int andmask, int ormask)
{
    int i;
    channel_t *cp;

    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle)
            cp->flags = (cp->flags & andmask) | ormask;
    }
}
