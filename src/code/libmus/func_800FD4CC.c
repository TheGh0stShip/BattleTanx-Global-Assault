#include "mus_channel.h"

extern double D_80077628, D_80077630, D_80077638, D_80077640, D_80077648, D_80077650, D_80077658;
extern double D_80077660, D_80077668, D_80077670, D_80077678, D_80077680, D_80077688, D_80077690;
extern double D_80077698;
extern unsigned int D_803AD994;
extern int D_803AD984;
extern void *D_803AD998;
extern void *D_803AD99C;
extern void osWritebackDCacheAll(void);
extern void func_800FD7D0(void *ptrs, void *base, int count);

void func_800FD4CC(unsigned char *p)
{
    register channel_t *cp asm("$6");
    register unsigned char old asm("$7");
    register unsigned int ff asm("$5");
    register unsigned int sevenf asm("$3");
    unsigned int i;
    int tempo;

    cp = (channel_t *)p;
    old = cp->fC9;
    cp->pdata = 0;
    for (i = 0; i < sizeof(channel_t); i++)
        *p++ = 0;
    ff = 0xFF;
    cp->pan_dirty = ff;
    cp->pan_changed = ff;
    tempo = 0x6000 / D_803AD984;
    sevenf = 0x7F;
    cp->fD3 = sevenf;
    cp->fBC = sevenf;
    cp->fC1 = sevenf;
    cp->fC7 = ff;
    cp->fC2 = sevenf;
    cp->fBD = 0x40;
    cp->fBF = 1;
    cp->fC6 = 1;
    cp->fC8 = 0xF;
    cp->fA0 = 0xFFFF;
    cp->f9A = 1;
    cp->fA2 = 1;
    cp->fA4 = 1;
    cp->stopping = -1;
    cp->temscale = 0x80;
    cp->pan_scale = 0x80;
    cp->volume_scale = 0x80;
    cp->f28 = 99.9f;
    cp->f6C = 0.03125f;
    cp->f58 = 1.0f;
    cp->f5C = (float)(1.0 / 255.0);
    cp->f60 = (float)(1.0 / 15.0);
    cp->base_volume = tempo;
    cp->volume = tempo;
    cp->f7C = D_803AD998 ? D_803AD998 : D_803AD99C;
    cp->fC9 = old;
}
