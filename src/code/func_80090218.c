#include "types.h"
#include "m2c_macros.h"

extern f32 D_80071DC8;

void func_80090218(void *arg0) {
    register f32 temp_f4 __asm__("$f4") = M2C_FIELD(arg0, f32 *, 0x250);
    register f32 decrement __asm__("$f0") = D_80071DC8;

    temp_f4 -= decrement;
    __asm__ volatile(
        "mtc1 $0,$f2\n"
        "mtc1 $0,$f3\n"
        "cvt.d.s $f0,$f4\n"
        "c.le.d $f0,$f2\n"
        "nop\n"
        "bc1f 1f\n"
        "swc1 $f4,0x250(%0)\n"
        "lw $2,0x1e0(%0)\n"
        "addiu $3,$0,-0x21\n"
        "and $2,$2,$3\n"
        "sw $2,0x1e0(%0)\n"
        "1:"
        :
        : "r"(arg0), "f"(temp_f4)
        : "$2", "$3", "$f2", "$f3", "memory");
}
