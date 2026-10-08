#include "mus_channel.h"

extern char *D_803AD978;
extern int D_803AD988;
extern unsigned short D_803AD98C;
extern unsigned short D_803AD98E;
extern double D_80077610;
extern double D_80077618;
extern void func_801022D0(void *voice);
extern void func_80102240(void *voice, void *wave);
extern void func_80102190(void *voice, short volume, int time);
extern void func_80102070(void *voice, int pan);
extern void func_80102100(void *voice, float pitch);
extern float func_800FD10C(float x);

void func_800FC864(channel_t *cp, int i)
{
    if (cp->fC9)
        func_801022D0(D_803AD978 + i * 0x1C);
    cp->fC9 = 1;
    func_80102240(D_803AD978 + i * 0x1C, cp->f08);
    cp->f08 = 0;
}

void func_800FC8E0(channel_t *cp, int i)
{
    unsigned int volume;

    volume = (unsigned int)(cp->fBC * cp->fC4 * cp->fBB * cp->temscale) >> 13;
    if (volume > 0x7FFF)
        volume = 0x7FFF;
    volume *= cp->sample_bank == 0 ? D_803AD98E : D_803AD98C;
    volume >>= 15;
    if (cp->stopping != -1)
        volume = volume * cp->stopping / cp->stopping_speed;
    if (volume != cp->fA0) {
        cp->fA0 = volume;
        func_80102190(D_803AD978 + i * 0x1C, volume, D_803AD988);
    }
    volume = (cp->fBD * cp->pan_scale >> 7) & 0x7F;
    if (volume != cp->pan_changed) {
        cp->pan_changed = volume;
        func_80102070(D_803AD978 + i * 0x1C, volume);
    }
}

void func_800FCA34(channel_t *cp, int i, float freq)
{
    float f;

    f = cp->f2C;
    if (cp->fB8) {
        if (cp->fB8 < cp->fAA) {
            cp->f50 = f;
        } else {
            f = cp->f4C + (f - cp->f4C) / cp->fB8 * cp->fAA;
            cp->f50 = f;
        }
    }
    f += freq + cp->f24;
    if (f != cp->f28) {
        cp->f28 = f;
        f = func_800FD10C(f * D_80077610);
        if (f > D_80077618) {
            f = 2.0f;
            cp->fBB = 0;
        }
        func_80102100(D_803AD978 + i * 0x1C, f);
    }
}

void func_800FCB40(channel_t *cp)
{
    if (cp->f9A != 0x7FFF) {
        if (cp->fB2)
            cp->f54 = cp->f40 + (cp->fB2 << 8);
        else
            cp->f54 = cp->f3C - (cp->fB4 << 8);
    } else {
        cp->f54 = cp->f40 + 0x7FFFFFFF;
    }
    cp->fC3 = 1;
    cp->fC4 = cp->fC0;
    cp->fC5 = cp->fBF;
}
