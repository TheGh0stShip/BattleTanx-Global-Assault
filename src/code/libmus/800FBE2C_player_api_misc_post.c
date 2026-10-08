#include "mus_channel.h"

typedef struct {
    unsigned char type;
    int value;
} mus_msg_t;

extern void *D_803AD9A8;
extern void *D_803AD9AC;
extern int D_803AD9B0;
extern int D_803AD9B4;
extern int D_803AD9B8;
extern int D_803AD9BC;
extern void *D_803AD9C0;
extern int D_8012686C;
extern void *__MusIntMemMalloc(int size);
extern void func_800FDCCC(int value, int a, int b);
extern void ChangeCustomEffect(int value);

int func_800FBE2C(int handle)
{
    int i;
    channel_t *cp;

    if (handle == 0)
        return 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle) {
            if (cp->song_bank)
                return ((int *)cp->song_bank)[8];
            return ((int *)cp->sample_bank)[5];
        }
    }
    return 0;
}

void func_800FBEA4(int value)
{
    D_803AD9B0 = value;
}

void func_800FBEB0(int count)
{
    if (count < 0x40)
        count = 0x40;
    else if (count > 0x400)
        count = 0x400;
    D_803AD9C0 = __MusIntMemMalloc(count * 8);
    D_803AD9BC = count;
    D_803AD9B8 = 0;
    D_803AD9B4 = 0;
}

void func_800FBF14(mus_msg_t *msg)
{
    switch (msg->type) {
    case 0:
        func_800FDCCC(msg->value, -2, 1);
        break;
    case 1:
        func_800FDCCC(msg->value, -2, 0);
        break;
    case 2:
        ChangeCustomEffect(msg->value);
        break;
    }
}
