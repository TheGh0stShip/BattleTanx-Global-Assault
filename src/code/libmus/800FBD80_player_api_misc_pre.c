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

int MusFxBankNumberOfEffects(int *bank)
{
    return bank[1];
}

void osViExtendVStart(void *bank)
{
    D_803AD9A8 = bank;
}

void func_800FBD94(void *bank)
{
    D_803AD9AC = bank;
}

void *func_800FBDA0(void)
{
    return D_803AD9A8;
}

void MusFxBankSetPtrBank(int *bank, int value)
{
    bank[4] = value;
}

int MusFxBankGetPtrBank(int *bank)
{
    return bank[4];
}

void func_800FBDBC(int value)
{
    D_8012686C = value;
}
