#include "types.h"

extern u8 D_802194A6;
extern u8 D_80235F00[];

/* Sum the active records associated with the requested owner. */
s32 func_8009B62C(s32 owner) {
    register s32 result __asm__("$2");
    __asm__ volatile(
        "addiu $sp,$sp,-8\n"
        "lui $2,%%hi(D_802194A6)\n"
        "lbu $2,%%lo(D_802194A6)($2)\n"
        "addu $5,$0,$0\n"
        "blez $2,3f\n"
        "addu $7,$0,$0\n"
        "addiu $10,$0,0x7f\n"
        "lui $9,%%hi(D_80235F00)\n"
        "addiu $9,$9,%%lo(D_80235F00)\n"
        "addu $8,$2,$0\n"
        "addu $6,$0,$0\n"
        "1:\n"
        "bne $5,$10,2f\n"
        "addu $3,$6,$9\n"
        "addu $3,$0,$0\n"
        "2:\n"
        "lw $2,0x10($3)\n"
        "bne $2,$4,4f\n"
        "addiu $5,$5,1\n"
        "lw $2,0x1b0($3)\n"
        "addu $7,$7,$2\n"
        "4:\n"
        "slt $2,$5,$8\n"
        "bnez $2,1b\n"
        "addiu $6,$6,0x250\n"
        "3:\n"
        "addu $2,$7,$0\n"
        "addiu $sp,$sp,8"
        : "=r"(result)
        :
        : "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "memory");
    return result;
}
