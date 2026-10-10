#include "mus_channel.h"

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
        return x * 0.693147180559945 + 1.0 + x2 * 0.240226506959101 + (x2 * x) * 0.0555041086648216 + (x2 * x2) * 0.00961812910762848 +
               (x2 * x2 * x) * 0.00133335581464284 + (x2 * x2 * x2) * 0.000154035303933816;
    }
    x = -x;
    x2 = x * x;
    x4 = x2 * x2;
    return 1.0 / (x * 0.693147180559945 + 1.0 + x2 * 0.240226506959101 + (x2 * x) * 0.0555041086648216 +
                         x4 * 0.00961812910762848 + (x4 * x) * 0.00133335581464284 + (x4 * x2) * 0.000154035303933816);
}
