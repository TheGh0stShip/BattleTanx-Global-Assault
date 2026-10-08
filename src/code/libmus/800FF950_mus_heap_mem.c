#include "n_synth_types.h"

extern ALHeap D_803ADC90;
extern void alHeapInit(ALHeap *hp, u8 *base, s32 len);

void func_800FF9F0(void *p, s32 value, s32 len);

void func_800FF950(u8 *base, s32 len)
{
    func_800FF9F0(base, 0, len);
    alHeapInit(&D_803ADC90, base, len);
}

void *__MusIntMemMalloc(s32 size)
{
    return alHeapAlloc(&D_803ADC90, 1, size);
}

s32 func_800FF9CC(void)
{
    return (u8 *)D_803ADC90.cur - (u8 *)D_803ADC90.base;
}

ALHeap *func_800FF9E4(void)
{
    return &D_803ADC90;
}

void func_800FF9F0(void *p, s32 value, s32 len)
{
    u8 *dst = p;
    u32 n = len;

    while (n--)
        *dst++ = value;
}

void func_800FFA1C(void *dst, void *src, s32 len)
{
    u8 *d = dst;
    u8 *s = src;

    if (s < d) {
        d += len;
        s += len;
        while (--len != -1)
            *--d = *--s;
    } else {
        while (--len != -1)
            *d++ = *s++;
    }
}
