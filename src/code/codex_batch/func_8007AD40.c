#include "types.h"

extern u8 *D_80114500;

/* Append count eight-byte records to the current arena stream. */
void func_8007AD40(s32 count, const void *records) {
    __asm__ volatile(
        "addu $8,$4,$0\n"
        "addu $9,$5,$0\n"
        "lui $2,%%hi(D_80114500)\n"
        "lw $2,%%lo(D_80114500)($2)\n"
        "addu $3,$8,$0\n"
        "addiu $8,$8,-1\n"
        "beqz $3,2f\n"
        "addiu $10,$2,0xc8\n"
        "1:\n"
        "lw $3,0($10)\n"
        "addu $4,$8,$0\n"
        "addiu $8,$8,-1\n"
        "addiu $2,$3,8\n"
        "sw $2,0($10)\n"
        "lw $2,0($9)\n"
        "lw $5,4($9)\n"
        "sw $2,0($3)\n"
        "sw $5,4($3)\n"
        "bnez $4,1b\n"
        "addiu $9,$9,8\n"
        "2:"
        :
        :
        : "$2", "$3", "$4", "$5", "$8", "$9", "$10", "memory");
}
