#include "types.h"

extern f32 D_800715F8;

/* Compute normalized progress across a cyclic scalar interval. */
f32 func_80087798(f32 start, f32 value, f32 limit) {
    register f32 result __asm__("$f0");
    __asm__ volatile(
        "mtc1 $6,$f0\n"
        "c.le.s $f14,$f0\n"
        "nop\n"
        "bc1f 1f\n"
        "nop\n"
        "c.le.s $f12,$f14\n"
        "nop\n"
        "bc1t 4f\n"
        "nop\n"
        "c.le.s $f0,$f12\n"
        "nop\n"
        "bc1t 2f\n"
        "nop\n"
        "sub.s $f2,$f12,$f14\n"
        "j 7f\n"
        "sub.s $f0,$f0,$f14\n"
        "1:\n"
        "c.le.s $f12,$f0\n"
        "nop\n"
        "bc1f 3f\n"
        "nop\n"
        "2:\n"
        "lui $1,%%hi(D_800715F8)\n"
        "lwc1 $f0,%%lo(D_800715F8)($1)\n"
        "j 6f\n"
        "nop\n"
        "3:\n"
        "c.le.s $f14,$f12\n"
        "nop\n"
        "bc1fl 5f\n"
        "sub.s $f2,$f12,$f0\n"
        "4:\n"
        "mtc1 $0,$f0\n"
        "j 6f\n"
        "nop\n"
        "5:\n"
        "sub.s $f0,$f14,$f0\n"
        "7:\n"
        "div.s $f0,$f2,$f0\n"
        "6:"
        : "=f"(result)
        :
        : "$1", "$f2");
    return result;
}
