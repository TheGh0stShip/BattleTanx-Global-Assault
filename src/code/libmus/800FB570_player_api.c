#include "mus_channel.h"

typedef struct {
    int pad[6];
    struct {
        int ptr;
        int priority;
    } fx[1];
} fxbank_t;

extern void *D_803AD9AC;
extern void *D_803AD9A8;
extern int D_803AD998;
extern channel_t *D_803AD980;
extern int func_800FD8B8(void *bank, int number, int volume, int pan, int priority);
extern int func_800FD808(channel_t *cp, void *bank, int number, int volume, int pan, int priority);

int func_800FB570(int number, int volume, int pan, int restartflag, int priority)
{
    void *bank;
    channel_t *cp;
    int i;
    int handle;

    if (D_803AD9AC != 0) {
        bank = D_803AD9AC;
        D_803AD9AC = 0;
    } else {
        bank = D_803AD9A8;
        if (bank == 0) {
            D_803AD998 = 0;
            return 0;
        }
    }
    if (D_803AD998 == 0)
        D_803AD998 = ((int *)bank)[4];
    if (restartflag) {
        for (i = 4, cp = D_803AD980; i < D_803AD974; i++, cp++) {
            if (cp->fx_number == number && cp->sample_bank == bank) {
                if (priority == -1)
                    priority = ((fxbank_t *)bank)->fx[number].priority;
                handle = func_800FD808(cp, bank, number, volume, pan, priority);
                D_803AD998 = 0;
                return handle;
            }
        }
    }
    handle = func_800FD8B8(bank, number, volume, pan, priority);
    D_803AD998 = 0;
    return handle;
}

void func_800FB688(int flags, int speed)
{
    int i;
    channel_t *cp;
    int speed2;

    speed2 = speed;
    if (speed == 0)
        speed2 = 1;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->sample_bank ? !(flags & 1) : !(flags & 2))
            continue;
        if (cp->pdata && cp->stopping == -1) {
            if (cp->flags & 1) {
                cp->stopping_speed = 1;
                cp->stopping = 0;
                cp->flags &= ~1;
            } else {
                cp->stopping_speed = speed2;
                cp->stopping = speed;
            }
        }
    }
}

int func_800FB754(int flags)
{
    int i;
    int count;
    channel_t *cp;

    count = 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->pdata == 0)
            continue;
        if (cp->sample_bank ? (flags & 1) : (flags & 2))
            count++;
    }
    return count;
}
