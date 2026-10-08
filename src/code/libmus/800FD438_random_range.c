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

int func_800FD438(int range)
{
    int i;
    unsigned int s;
    unsigned int t;

    for (i = 0; i < 8; i++) {
        s = D_803AD994;
        D_803AD994 = s << 1;
        t = s & 0x48000000;
        if (t == 0x48000000 || t == 0x08000000)
            D_803AD994 = (s << 1) | 1;
    }
    return range * ((float)(int)D_803AD994 / 65536.0f / 65536.0f);
}
