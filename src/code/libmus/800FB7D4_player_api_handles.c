#include "mus_channel.h"

extern void *D_803AD9A8;
extern int D_803AD998;
extern int D_803AD99C;
extern int D_803AD9A0;
extern int D_803AD9A4;
extern int func_800FD2A0(void *bank);

typedef struct {
    unsigned char type;
    int value;
} mus_msg_t;

extern int player_text_1AE0(mus_msg_t *msg);

int func_800FB7D4(int handle, int speed)
{
    int i;
    int count;
    channel_t *cp;
    int speed2;

    if (handle == 0)
        return 0;
    speed2 = speed;
    if (speed == 0)
        speed2 = 1;
    count = 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle && cp->stopping == -1) {
            if (cp->flags & 1) {
                cp->stopping_speed = 1;
                cp->stopping = 0;
                cp->flags &= ~1;
            } else {
                cp->stopping_speed = speed2;
                cp->stopping = speed;
            }
            count++;
        }
    }
    return count;
}

int func_800FB888(int handle)
{
    channel_t *cp;
    int count;
    int i;

    if (handle == 0)
        return 0;
    count = 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++)
        count += cp->handle == handle;
    return count;
}

int func_800FB8E0(int handle, int speed)
{
    int i;
    int count;
    channel_t *cp;

    if (handle == 0)
        return 0;
    count = 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle) {
            cp->temscale = speed;
            count++;
        }
    }
    return count;
}

int func_800FB940(int handle, int scale)
{
    int i;
    int count;
    channel_t *cp;

    if (handle == 0)
        return 0;
    count = 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle) {
            count++;
            cp->pan_scale = scale;
            cp->pan_changed = 0xFF;
        }
    }
    return count;
}

int func_800FB9B0(int handle, float offset)
{
    int i;
    int count;
    channel_t *cp;

    if (handle == 0)
        return 0;
    count = 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle) {
            cp->freqoffset = offset + cp->base_freq;
            count++;
        }
    }
    return count;
}

int func_800FBA20(int handle, int volume)
{
    int i;
    int count;
    channel_t *cp;

    if (handle == 0)
        return 0;
    if (volume <= 0)
        volume = 1;
    else if (volume > 0x100)
        volume = 0x100;
    count = 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle) {
            count++;
            cp->volume_scale = volume;
            cp->volume = (cp->base_volume * volume) >> 7;
        }
    }
    return count;
}

int func_800FBAAC(int handle, int pan)
{
    int i;
    int count;
    channel_t *cp;

    if (handle == 0)
        return 0;
    if (pan < 0)
        pan = 0;
    else if (pan > 0x7F)
        pan = 0x7F;
    count = 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle) {
            count++;
            cp->pan = pan;
            cp->pan_dirty = 0xFF;
        }
    }
    return count;
}

void func_800FBB34(void *bank)
{
    func_800FD2A0(bank);
    if (D_803AD99C == 0)
        D_803AD99C = (int)bank;
}

int func_800FBB70(int *bank)
{
    if (bank != 0 && bank[4] < 0)
        D_803AD998 = (int)bank;
    return D_803AD998;
}

void MusPtrBankSetCurrent(int *bank)
{
    if (bank != 0 && bank[4] < 0)
        D_803AD99C = (int)bank;
}

int __osGetActiveQueue(void)
{
    return D_803AD99C;
}

int func_800FBBC8(int handle)
{
    int i;
    channel_t *cp;

    if (handle == 0)
        return 0;
    for (i = 0, cp = D_803AD97C; i < D_803AD974; i++, cp++) {
        if (cp->handle == handle)
            return *(int *)((char *)cp + 0x7C);
    }
    return 0;
}

int MusHandlePause(int handle)
{
    mus_msg_t msg;

    msg.type = 0;
    msg.value = handle;
    return player_text_1AE0(&msg);
}

int MusHandleUnPause(int handle)
{
    mus_msg_t msg;

    msg.type = 1;
    msg.value = handle;
    return player_text_1AE0(&msg);
}

int MusSetFxType(int type)
{
    mus_msg_t msg;

    msg.type = 2;
    msg.value = type;
    D_803AD9A4 = type;
    return player_text_1AE0(&msg);
}

void func_800FBCA0(int a)
{
    int result;
    mus_msg_t msg;

    result = 1;
    if (a == 0) {
        int type = D_803AD9A4;
        msg.type = 2;
        msg.value = type;
        D_803AD9A4 = type;
        result = player_text_1AE0(&msg);
    }
    if (result)
        D_803AD9A0 = a;
}

typedef struct {
    int count;
    int pad04[2];
    int flags;
    int pad10;
    int offset14;
    struct {
        int ptr;
        int pad;
    } wave[1];
} ptr_bank_t;

void func_800FBCFC(ptr_bank_t *b)
{
    int i;
    int base;
    ptr_bank_t *bank;

    bank = b;
    base = (int)b;
    if (!(bank->flags & 1)) {
        if (D_803AD9A8 == 0)
            D_803AD9A8 = bank;
        bank->flags = 1;
        bank->pad10 = 0;
        bank->offset14 = (int)bank + bank->offset14;
        for (i = 0; i < bank->count; i++)
            bank->wave[i].ptr = base + bank->wave[i].ptr;
    }
}
