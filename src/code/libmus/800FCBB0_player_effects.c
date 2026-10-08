#include "mus_channel.h"

extern double D_80077620;
extern float sinf(float x);

void func_800FCBB0(channel_t *cp)
{
    int t;
    unsigned char phase;

    if (cp->f54 - cp->f0C < 0 && cp->fC3 < 4) {
        cp->fC3 = 4;
        cp->fC5 = 1;
        cp->fCD = cp->fC4;
    }
    if (--cp->fC5 != 0)
        return;
    phase = cp->fC3;
    cp->fC5 = cp->fBF;
    switch (phase) {
    case 1:
        t = (((unsigned int)(cp->f0C - cp->f40) >> 8) * cp->f64) >> 10;
        if (t < cp->fC6) {
            cp->fC4 = cp->fC0 + (int)(cp->f58 * t);
        } else {
            cp->fC3 = phase + 1;
            cp->fC4 = cp->fC1;
        }
        break;
    case 2:
        t = ((((unsigned int)(cp->f0C - cp->f40) >> 8) - cp->fC6) * cp->f64) >> 10;
        if (t < cp->fC7) {
            cp->fC4 = cp->fC1 + (int)(cp->f5C * t);
        } else {
            cp->fC3 = phase + 1;
            cp->fC4 = cp->fC2;
        }
        break;
    case 3:
        break;
    case 4:
        t = (((unsigned int)(cp->f0C - cp->f54) >> 8) * cp->f64) >> 10;
        if (t < cp->fC8) {
            cp->fC4 = cp->fCD - (int)(cp->f60 * t * cp->fCD);
        } else {
            cp->fC3 = phase + 1;
            cp->fC4 = 0;
        }
        break;
    }
}

void __MusIntInitSweep(channel_t *cp)
{
    cp->f94 = cp->f40;
    cp->fD9 = 0;
    cp->fDA = cp->fBD & 0x40;
}

void func_800FCDC0(channel_t *cp)
{
    unsigned int a;

    do {
        a = cp->fD9 + cp->fD4;
        cp->f94 += 0x100;
        if (a < 0x40) {
            cp->fD9 = a;
        } else {
            cp->fD9 = a & 0x3F;
            a >>= 6;
            if (cp->fDA == 0) {
                cp->fBD += a;
                if (cp->fBD > 0x7F) {
                    cp->fBD = 0x7F;
                    cp->fDA = 1;
                }
            } else {
                cp->fBD -= a;
                if (cp->fBD > 0x7F || cp->fBD == 0) {
                    cp->fBD = 0;
                    cp->fDA = 0;
                }
            }
        }
    } while (cp->f94 - cp->f0C < 0);
}

float func_800FCE78(channel_t *cp)
{
    if (--cp->fD0 == 0) {
        if (cp->fD1 == 0) {
            cp->fD1 = cp->fD8;
            cp->fD0 = cp->fCE;
        } else {
            cp->fD1 = 0;
            cp->fD0 = cp->fCF;
        }
    }
    return cp->fD1;
}

float func_800FCED0(channel_t *cp)
{
    int t;

    t = cp->fAA - cp->fB6;
    if (t > 0)
        return cp->f68 = sinf(t * cp->fDC) * cp->f20;
    return 0.0f;
}

void func_800FCF34(channel_t *cp)
{
    unsigned char c;

    do {
        cp->f14 += 0x100;
        if (--cp->fA2 == 0) {
            c = *cp->f38++;
            if (c >= 0x80) {
                cp->fBC = c & 0x7F;
                c = *cp->f38++;
                if (c >= 0x80) {
                    cp->fA2 = (c & 0x7F) << 8;
                    cp->fA2 = *cp->f38++ + (((c & 0x7F) << 8) + 2);
                } else {
                    cp->fA2 = c + 2;
                }
            } else {
                cp->fBC = c;
                cp->fA2 = 1;
            }
        }
    } while (cp->f14 - cp->f0C < 0);
}

void func_800FCFF8(channel_t *cp)
{
    unsigned char c;
    float f;

    do {
        cp->f18 += 0x100;
        if (--cp->fA4 == 0) {
            c = *cp->f34++;
            if (c >= 0x80) {
                f = (float)(c & 0x7F) - D_80077620;
                cp->f70 = f;
                cp->f24 = f * cp->f6C;
                c = *cp->f34++;
                if (c >= 0x80) {
                    cp->fA4 = (c & 0x7F) << 8;
                    cp->fA4 = *cp->f34++ + (((c & 0x7F) << 8) + 2);
                } else {
                    cp->fA4 = c + 2;
                }
            } else {
                f = (float)c - D_80077620;
                cp->fA4 = 1;
                cp->f70 = f;
                cp->f24 = f * cp->f6C;
            }
        }
    } while (cp->f18 - cp->f0C < 0);
}
