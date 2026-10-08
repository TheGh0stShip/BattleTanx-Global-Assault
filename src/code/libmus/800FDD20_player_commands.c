/* RODATA_VRAM 0x800776A0: this unit's literal pool is linked at its retail address. */
#include "mus_channel.h"

extern int D_803AD984;

unsigned char *Fstop(channel_t *cp, unsigned char *p)
{
    cp->f38 = 0;
    cp->f34 = 0;
    cp->song_bank = 0;
    cp->sample_bank = 0;
    cp->handle = 0;
    cp->f08 = 0;
    return 0;
}

unsigned char *Fwave(channel_t *cp, unsigned char *p)
{
    int w;

    w = *p++;
    if (w & 0x80)
        w = *p++ | ((w & 0x7F) << 8);
    cp->fAE = w;
    return p;
}

unsigned char *Fport(channel_t *cp, unsigned char *p)
{
    if ((cp->fB8 = *p++) != 0)
        cp->f50 = cp->f2C;
    return p;
}

unsigned char *Fportoff(channel_t *cp, unsigned char *p)
{
    cp->fB8 = 0;
    return p;
}

unsigned char *Fdefa(channel_t *cp, unsigned char *p)
{
    int a;
    int b;

    a = *p++;
    if (a == 0)
        a = 1;
    cp->fBF = a;
    cp->f64 = 0x400 / a;
    cp->fC0 = *p++;
    a = *p++;
    cp->fC6 = a;
    b = *p++;
    cp->fC1 = b;
    cp->f58 = 1.0 / (float)a * (float)(b - cp->fC0);
    a = *p++;
    cp->fC7 = a;
    b = *p++;
    cp->fC2 = b;
    cp->f5C = 1.0 / (float)a * (float)(b - cp->fC1);
    a = *p++;
    cp->fC8 = a;
    cp->f60 = 1.0 / (float)a;
    return p;
}

unsigned char *Ftempo(channel_t *cp, unsigned char *p)
{
    int tempo;
    int scaled;
    int i;
    channel_t *c;

    tempo = (*p++ * 0x6000 / 120) / D_803AD984;
    scaled = (tempo * cp->volume_scale) >> 7;
    if (cp->sample_bank) {
        cp->volume = tempo;
    } else {
        for (i = 0, c = D_803AD97C; i < D_803AD974; i++, c++) {
            if (c->song_bank == cp->song_bank) {
                c->base_volume = tempo;
                c->volume = scaled;
            }
        }
    }
    return p;
}

unsigned char *Fendit(channel_t *cp, unsigned char *p)
{
    int v;

    v = *p++;
    cp->fB2 = 0;
    cp->fB4 = v;
    return p;
}

unsigned char *Fcutoff(channel_t *cp, unsigned char *p)
{
    int hi;
    int lo;

    hi = *p++;
    lo = *p++;
    cp->fB4 = 0;
    cp->fB2 = lo | (hi << 8);
    return p;
}

unsigned char *Fvibup(channel_t *cp, unsigned char *p)
{
    cp->fB6 = *p++;
    cp->fD5 = *p++;
    cp->f20 = (float)*p++ / 50.0;
    cp->fDC = 6.2831852 / (float)cp->fD5;
    return p;
}

unsigned char *Fvibdown(channel_t *cp, unsigned char *p)
{
    cp->fB6 = *p++;
    cp->fD5 = *p++;
    cp->f20 = -(float)*p++ / 50.0;
    cp->fDC = 6.2831852 / (float)cp->fD5;
    return p;
}

unsigned char *Fviboff(channel_t *cp, unsigned char *p)
{
    cp->fD5 = 0;
    cp->f68 = 0.0f;
    return p;
}

unsigned char *Flength(channel_t *cp, unsigned char *p)
{
    int v;

    v = *p++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *p++;
    }
    cp->fAC = v;
    return p;
}

unsigned char *Fignore(channel_t *cp, unsigned char *p)
{
    cp->fB7 = 1;
    return p;
}

unsigned char *Ftrans(channel_t *cp, unsigned char *p)
{
    cp->fB9 = *p++;
    return p;
}

unsigned char *Fignore_trans(channel_t *cp, unsigned char *p)
{
    cp->fBA = 1;
    return p;
}

extern int func_800FD438(int range);
extern int func_800FD8B8(void *bank, int number, int volume, int pan, int priority);
extern void ChangeCustomEffect(int type);
extern int D_803AD9A0;
extern void (*D_803AD9B0)(int handle, int marker);

unsigned char *Fdistort(channel_t *cp, unsigned char *p)
{
    int c;
    float f;

    c = *p++;
    if (c & 0x80)
        c |= -0x100;
    f = (float)c / 100.0;
    cp->freqoffset = cp->freqoffset - cp->base_freq + f;
    cp->base_freq = f;
    return p;
}

unsigned char *Fenvoff(channel_t *cp, unsigned char *p)
{
    cp->fD6 = 1;
    return p;
}

unsigned char *Fenvon(channel_t *cp, unsigned char *p)
{
    cp->fD6 = 0;
    return p;
}

unsigned char *Ftroff(channel_t *cp, unsigned char *p)
{
    cp->fD7 = 1;
    return p;
}

unsigned char *Ftron(channel_t *cp, unsigned char *p)
{
    cp->fD7 = 0;
    return p;
}

unsigned char *Ffor(channel_t *cp, unsigned char *p)
{
    int idx;

    idx = cp->fDB;
    cp->f120[idx] = *p++;
    cp->fE0[idx] = p;
    cp->fF0[idx] = cp->f38;
    cp->f100[idx] = cp->f34;
    cp->f124[idx] = cp->fBC;
    cp->f128[idx] = cp->f70;
    cp->f110[idx] = cp->fA2;
    cp->f118[idx] = cp->fA4;
    cp->fDB++;
    return p;
}

unsigned char *Fnext(channel_t *cp, unsigned char *p)
{
    int idx;

    idx = cp->fDB - 1;
    if (cp->f120[idx] != 0xFF) {
        if (--cp->f120[idx] == 0) {
            cp->fDB = idx;
            idx = -1;
        }
    }
    if (idx >= 0) {
        p = cp->fE0[idx];
        cp->f38 = cp->fF0[idx];
        cp->f34 = cp->f100[idx];
        cp->fBC = cp->f124[idx];
        cp->f70 = cp->f128[idx];
        cp->fA2 = cp->f110[idx];
        cp->fA4 = cp->f118[idx];
        cp->f24 = cp->f70 * cp->f6C;
    }
    return p;
}

unsigned char *Fwobble(channel_t *cp, unsigned char *p)
{
    cp->fD8 = *p++;
    cp->fCE = *p++;
    cp->fCF = *p++;
    return p;
}

unsigned char *Fwobbleoff(channel_t *cp, unsigned char *p)
{
    cp->fCE = 0;
    return p;
}

unsigned char *Fvelon(channel_t *cp, unsigned char *p)
{
    cp->fD2 = 1;
    return p;
}

unsigned char *Fveloff(channel_t *cp, unsigned char *p)
{
    cp->fD2 = 0;
    return p;
}

unsigned char *Fvelocity(channel_t *cp, unsigned char *p)
{
    int v;

    v = *p++;
    cp->fD2 = 0;
    cp->fD3 = v;
    return p;
}

unsigned char *Fpan(channel_t *cp, unsigned char *p)
{
    cp->fBD = *p++ >> 1;
    return p;
}

unsigned char *Fstereo(channel_t *cp, unsigned char *p)
{
    return p + 2;
}

unsigned char *Fdrums(channel_t *cp, unsigned char *p)
{
    int v;

    v = *p++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *p++;
    }
    cp->f84 = ((unsigned char **)cp->song_bank)[7] + v * 6;
    return p;
}

unsigned char *Fdrumsoff(channel_t *cp, unsigned char *p)
{
    cp->f84 = 0;
    return p;
}

unsigned char *Fprint(channel_t *cp, unsigned char *p)
{
    return p + 1;
}

unsigned char *Fgoto(channel_t *cp, unsigned char *p)
{
    int off;
    int v;

    off = *p++ << 8;
    off += *p++;
    v = *p++ << 8;
    v += *p++;
    cp->fA2 = 1;
    cp->f38 = cp->f8C + v;
    v = *p++ << 8;
    v += *p++;
    cp->fA4 = 1;
    cp->f34 = cp->f88 + v;
    return cp->f80 + off;
}

unsigned char *Freverb(channel_t *cp, unsigned char *p)
{
    cp->fCA = *p++;
    return p;
}

unsigned char *FrandNote(channel_t *cp, unsigned char *p)
{
    int r;

    r = func_800FD438(*p++);
    cp->fB9 = r;
    r += *p++;
    cp->fB9 = r;
    return p;
}

unsigned char *FrandVolume(channel_t *cp, unsigned char *p)
{
    int r;

    r = func_800FD438(*p++);
    cp->fBC = r;
    r += *p++;
    cp->fBC = r;
    return p;
}

unsigned char *FrandPan(channel_t *cp, unsigned char *p)
{
    int r;

    r = func_800FD438(*p++);
    cp->fBD = r;
    r += *p++;
    cp->fBD = r;
    return p;
}

unsigned char *Fvolume(channel_t *cp, unsigned char *p)
{
    cp->fBC = *p++;
    return p;
}

unsigned char *Fstartfx(channel_t *cp, unsigned char *p)
{
    int n;
    int handle;
    int i;
    int pr;
    channel_t *c;

    n = *p++;
    if (n >= 0x80)
        n = ((n & 0x7F) << 8) + *p++;
    pr = ++cp->f48;
    handle = func_800FD8B8(cp->sample_bank, n, cp->temscale, cp->pan_scale, pr);
    cp->f48--;
    if (handle) {
        for (i = 0, c = D_803AD97C; i < D_803AD974; i++, c++) {
            if (c->handle == handle) {
                c->handle = cp->handle;
                c->f7C = cp->f7C;
            }
        }
    }
    return p;
}

unsigned char *Fbendrange(channel_t *cp, unsigned char *p)
{
    float f;

    f = (float)*p++ * 0.015625;
    cp->f6C = f;
    cp->f24 = cp->f70 * f;
    return p;
}

unsigned char *Fsweep(channel_t *cp, unsigned char *p)
{
    cp->fD4 = *p++;
    return p;
}

unsigned char *Fchangefx(channel_t *cp, unsigned char *p)
{
    int type;

    type = *p++;
    if (D_803AD9A0 == 1)
        ChangeCustomEffect(type);
    return p;
}

unsigned char *Fmarker(channel_t *cp, unsigned char *p)
{
    int marker;
    int t;

    marker = *p++;
    t = *p++;
    if (t & 0x80)
        t = ((t & 0x7F) << 8) | *p++;
    if ((cp->flags & 3) == 2 && D_803AD9B0)
        D_803AD9B0(cp->handle, marker);
    return p;
}

unsigned char *Flength0(channel_t *cp, unsigned char *p)
{
    cp->fAC = 0;
    return p;
}
