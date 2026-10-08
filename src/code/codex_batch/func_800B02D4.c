#include "types.h"

extern u8 *D_80219498;

/* Tests a point against the first rectangle stored in a player bounds record. */
s32 func_800B02D4(f32 arg0, f32 arg1, s32 arg2) {
    register s32 result __asm__("$2");

    __asm__ volatile(
        "andi $6,$6,0xff\n"
        "lui $2,%%hi(D_80219498)\n"
        "lw $2,%%lo(D_80219498)($2)\n"
        "sll $6,$6,5\n"
        "addiu $6,$6,4\n"
        "addu $4,$2,$6\n"
        "lh $2,0($4)\n"
        "mtc1 $2,$f0\n"
        "cvt.s.w $f0,$f0\n"
        "c.le.s $f0,$f12\n"
        "nop\n"
        "bc1f 1f\n"
        "addu $2,$0,$0\n"
        "lh $3,4($4)\n"
        "mtc1 $3,$f0\n"
        "cvt.s.w $f0,$f0\n"
        "c.le.s $f12,$f0\n"
        "nop\n"
        "bc1f 1f\n"
        "nop\n"
        "lh $3,2($4)\n"
        "mtc1 $3,$f0\n"
        "cvt.s.w $f0,$f0\n"
        "c.le.s $f0,$f14\n"
        "nop\n"
        "bc1f 1f\n"
        "nop\n"
        "lh $3,6($4)\n"
        "mtc1 $3,$f0\n"
        "cvt.s.w $f0,$f0\n"
        "c.le.s $f14,$f0\n"
        "nop\n"
        "bc1tl 1f\n"
        "addiu $2,$0,1\n"
        "1:"
        : "=r"(result)
        :
        : "$3", "$4", "$6", "$f0", "memory");
    return result;
}
