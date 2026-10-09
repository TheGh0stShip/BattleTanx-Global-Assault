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

inline void func_800FD7D0(void *addr, void *offset, int count)
{
    unsigned long *p;
    unsigned long base;
    int i;

    p = (unsigned long *)addr;
    base = (unsigned long)offset;
    for (i = 0; i < count; i++)
        if (p[i])
            p[i] += base;
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

int player_text_329C(void *addr)
{
    song_t *song;
    int i;
    int count;
    channel_t *cp;
    unsigned long handle;

    song = addr;
    count = song->count;
    /* Preserve the original libmus expression shape; KMC allocation depends on it. */
    if (!song->f28 & 1) {
        song->f28 |= 1;
        func_800FD7D0(&song->fC, addr, 7);
        func_800FD7D0(song->fC, addr, count);
        func_800FD7D0(song->f10, addr, count);
        func_800FD7D0(song->f14, addr, count);
    }
    handle = D_803AD990++;
    cp = &D_803AD97C[func_800FD608(song, -1)];
    func_800FD4CC(cp);
    cp->fD2 = 1;
    cp->flags |= 3;
    cp->song_bank = song;
    cp->pdata = cp->f80 = song->f24;
    cp->handle = handle;
    for (i = 0; i < count; i++) {
        if (song->fC[i]) {
            cp = &D_803AD97C[func_800FD608(song, i)];
            func_800FD4CC(cp);
            cp->fD2 = 1;
            cp->flags |= 1;
            cp->song_bank = song;
            cp->f38 = cp->f8C = song->f10[i];
            cp->f34 = cp->f88 = song->f14[i];
            cp->pdata = cp->f80 = song->fC[i];
            cp->handle = handle;
        }
    }
    D_803AD998 = 0;
    return handle;
}
