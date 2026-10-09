#include "types.h"

/* Return the orientation of three two-dimensional points. */
s32 func_8007D628(const f32 *a, const f32 *b, const f32 *c) {
    register s32 result __asm__("$2");
    __asm__ volatile(
        "lwc1 $f6,4($4)\n"
        "lwc1 $f10,4($5)\n"
        "lwc1 $f8,0($5)\n"
        "sub.s $f6,$f6,$f10\n"
        "lwc1 $f2,0($6)\n"
        "sub.s $f2,$f2,$f8\n"
        "lwc1 $f4,0($4)\n"
        "lwc1 $f0,4($6)\n"
        "sub.s $f4,$f4,$f8\n"
        "mul.s $f6,$f6,$f2\n"
        "sub.s $f0,$f0,$f10\n"
        "mul.s $f0,$f0,$f4\n"
        "sub.s $f6,$f6,$f0\n"
        "mtc1 $0,$f0\n"
        "c.lt.s $f0,$f6\n"
        "nop\n"
        "bc1f 1f\n"
        "nop\n"
        "j 2f\n"
        "addiu $2,$0,1\n"
        "1:\n"
        "c.lt.s $f6,$f0\n"
        "nop\n"
        "bc1t 2f\n"
        "addiu $2,$0,2\n"
        "addu $2,$0,$0\n"
        "2:"
        : "=r"(result)
        :
        : "$f0", "$f2", "$f4", "$f6", "$f8", "$f10");
    return result;
}
