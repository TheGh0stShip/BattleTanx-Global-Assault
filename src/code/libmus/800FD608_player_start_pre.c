#include "mus_channel.h"

extern channel_t *D_803AD980;
extern int D_803AD990;
extern int D_803AD998;
extern void func_800FD4CC(channel_t *cp);

typedef struct {
    int pad00[4];
    int f10;
    int pad14;
    struct {
        unsigned char *ptr;
        int priority;
    } fx[1];
} fxbank_t;

typedef struct {
    int pad00;
    int count;
    int pad08;
    unsigned char **fC;
    unsigned char **f10;
    unsigned char **f14;
    int pad18[3];
    unsigned char *f24;
    int f28;
} song_t;

int func_800FD608(song_t *song, int index)
{
    int i;
    channel_t *cp;
    int best;
    int pr;

    if (index < 0) {
        for (i = 0, cp = D_803AD97C; i < 4; i++, cp++) {
            if (cp->pdata == 0)
                return i;
        }
    }
    for (i = 4, cp = D_803AD980; i < D_803AD974; i++, cp++) {
        if (cp->pdata == 0)
            return i;
    }
    i = 4;
    cp = D_803AD980;
    pr = 0x7FFFFFFF;
    best = 3;
    for (; i < D_803AD974; i++, cp++) {
        if (cp->sample_bank && cp->f48 <= pr) {
            pr = cp->f48;
            best = i;
        }
    }
    if (best >= 4)
        return best;
    for (i = 4, cp = D_803AD980; i < D_803AD974; i++) {
        if (cp->sample_bank == 0 && cp->song_bank != song)
            return i;
    }
    for (i = 4, cp = D_803AD980; i < D_803AD974; i++, cp++) {
        if (cp->song_bank == song && song->fC[index] == cp->f80)
            return i;
    }
    return index % (D_803AD974 - 4) + 4;
}

inline void func_800FD7D0(int *p, int base, int count)
{
    int i;

    for (i = 0; i < count; i++, p++) {
        if (*p)
            *p += base;
    }
}

inline int func_800FD808(channel_t *cp, fxbank_t *bank, int number, int volume, int pan, int priority)
{
    func_800FD4CC(cp);
    cp->fx_number = number;
    cp->sample_bank = bank;
    cp->temscale = volume;
    cp->pan_scale = pan;
    cp->handle = D_803AD990++;
    cp->f48 = priority;
    if (bank->f10)
        cp->f7C = (void *)bank->f10;
    cp->pdata = cp->f80 = bank->fx[number].ptr;
    return cp->handle;
}

int func_800FD8B8(fxbank_t *bank, int number, int volume, int pan, int priority)
{
    int i;
    channel_t *cp;
    channel_t *best;
    int pr;

    if (priority == -1)
        priority = bank->fx[number].priority;
    pr = priority + 1;
    for (i = 4, cp = D_803AD980; i < D_803AD974; i++, cp++) {
        if (cp->pdata == 0)
            return func_800FD808(cp, bank, number, volume, pan, priority);
        if (cp->sample_bank && cp->f48 < pr) {
            pr = cp->f48;
            best = cp;
        }
    }
    if (pr < priority)
        return func_800FD808(best, bank, number, volume, pan, priority);
    return 0;
}
