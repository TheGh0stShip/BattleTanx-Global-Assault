#ifndef GU_H
#define GU_H

#include "types.h"

typedef long Mtx_t[4][4];

typedef union {
    Mtx_t m;
    long long int force_structure_alignment;
} Mtx;

extern float sqrtf(float);
extern void guMtxIdentF(float mf[4][4]);
extern void guMtxF2L(float mf[4][4], Mtx *m);

#endif
