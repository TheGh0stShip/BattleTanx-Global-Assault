#include "ultra_basic_types.h"

extern u32 D_803ADC80;
extern u32 D_803ADC84;
extern u32 D_803ADC88;
extern u32 D_803ADC8C;
extern s32 D_80126880;

u32 func_800FF820(s32 retraceCount, s32 outputRate, u32 viRate, u32 extraPercent)
{
    u32 samples;
    u32 frame;

    samples = (retraceCount * outputRate + viRate - 1) / viRate;
    frame = (samples / 184 + 1) * 184;
    D_803ADC80 = frame;
    D_803ADC84 = frame - 184;
    D_803ADC88 = frame + 184;
    D_803ADC8C = frame * extraPercent / 100;
    return D_803ADC80 + 184 + D_803ADC8C;
}

u32 func_800FF8C4(u32 samples)
{
    if (samples > D_803ADC8C + 184) {
        if (D_80126880) {
            D_80126880 = 0;
            return D_803ADC84;
        }
    } else if (samples < D_803ADC8C) {
        if (D_80126880) {
            D_80126880 = 0;
            return D_803ADC88;
        }
    } else {
        D_80126880 = 1;
    }
    return D_803ADC80;
}
