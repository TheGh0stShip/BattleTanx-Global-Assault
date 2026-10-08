/* IDO 5.3: -O2 -mips2 -Wab,-r4300_mul */
#include "types.h"

typedef union {
    struct {
        unsigned int hi;
        unsigned int lo;
    } word;
    double d;
} du;

typedef union {
    unsigned int i;
    float f;
} fu;

#define ABS(x) ((x) > 0 ? (x) : -(x))
#define ROUND(d) (int)(((d) >= 0.0) ? ((d) + 0.5) : ((d) - 0.5))

static const du P[] = {
    { { 0x3ff00000, 0x00000000 } },
    { { 0xbfc55554, 0xbc83656d } },
    { { 0x3f8110ed, 0x3804c2a0 } },
    { { 0xbf29f6ff, 0xeea56814 } },
    { { 0x3ec5dbdf, 0x0e314bfe } },
};

static const du rpi = { { 0x3fd45f30, 0x6dc9c883 } };
static const du pihi = { { 0x400921fb, 0x50000000 } };
static const du pilo = { { 0x3e6110b4, 0x611a6263 } };
static const fu zero = { 0x00000000 };

extern float __libm_qnan_f;

float cosf(float x)
{
    double dx, xsq, poly;
    double dn;
    int n;
    double result;
    int ix, xpt;

    ix = *(int *)&x;
    xpt = (ix >> 22);
    xpt &= 0x1ff;

    if (xpt < 0x136) {
        dx = (float)ABS(x);
        dn = dx * rpi.d + 0.5;
        n = ROUND(dn);
        dn = n;
        dn -= 0.5;
        dx = dx - dn * pihi.d;
        dx = dx - dn * pilo.d;
        xsq = dx * dx;
        poly = ((P[4].d * xsq + P[3].d) * xsq + P[2].d) * xsq + P[1].d;
        result = dx + (dx * xsq) * poly;
        if ((n & 1) == 0)
            return (float)result;
        return -(float)result;
    }

    if (x != x)
        return __libm_qnan_f;
    return zero.f;
}
