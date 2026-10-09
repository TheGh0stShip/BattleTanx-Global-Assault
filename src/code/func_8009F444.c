#include "types.h"

/* Copy a 4x4 matrix while scaling its first three rows. */
void func_8009F444(const f32 *src, f32 *dst, f32 scale) {
    __asm__ volatile(
        "mtc1 $6,$f2\n"
        "addu $7,$0,$0\n"
        "1:\n"
        "slti $10,$7,3\n"
        "addu $3,$5,$0\n"
        "addu $6,$4,$0\n"
        "addiu $9,$5,0xc\n"
        "addiu $8,$5,0x10\n"
        "2:\n"
        "slt $2,$3,$9\n"
        "and $2,$10,$2\n"
        "beqz $2,3f\n"
        "nop\n"
        "lwc1 $f0,0($6)\n"
        "mul.s $f0,$f0,$f2\n"
        "j 4f\n"
        "nop\n"
        "3:\n"
        "lwc1 $f0,0($6)\n"
        "4:\n"
        "swc1 $f0,0($3)\n"
        "addiu $3,$3,4\n"
        "slt $2,$3,$8\n"
        "bnez $2,2b\n"
        "addiu $6,$6,4\n"
        "addiu $4,$4,0x10\n"
        "addiu $7,$7,1\n"
        "slti $2,$7,4\n"
        "bnez $2,1b\n"
        "addiu $5,$5,0x10"
        :
        :
        : "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$f0", "$f2", "memory");
}
