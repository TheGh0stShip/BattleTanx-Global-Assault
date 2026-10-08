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

typedef struct {
    int addr;
    int pad04;
    unsigned char f8;
    unsigned char f9;
    unsigned char pad0A[2];
    int fC;
    int f10;
} wave_t;

typedef struct {
    int pad00[4];
    int f10;
    int pad14[3];
    int f20;
    unsigned char *f24;
    float *f28;
    wave_t **f2C;
} bank_t;

float func_800FD10C(float x)
{
    float x2;
    float x4;

    if (x == 0.0f)
        return 1.0f;
    if (x > 0.0f) {
        x2 = x * x;
        return x * D_80077660 + D_80077680 + x2 * D_80077668 + (x2 * x) * D_80077670 + (x2 * x2) * D_80077678 +
               (x2 * x2 * x) * D_80077688 + (x2 * x2 * x2) * D_80077690;
    }
    x = -x;
    x2 = x * x;
    x4 = x2 * x2;
    return D_80077648 / (x * D_80077628 + D_80077648 + x2 * D_80077630 + (x2 * x) * D_80077638 +
                         x4 * D_80077640 + (x4 * x) * D_80077650 + (x4 * x2) * D_80077658);
}
