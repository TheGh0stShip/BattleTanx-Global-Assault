#include "types.h"

extern u8 D_802194A6;
extern u8 D_80235F00[];

/* Mark every active flagged record with state one. */
void func_8009B3C8(void) {
    __asm__ volatile(
        "lui $3,%%hi(D_802194A6)\n"
        "addiu $3,$3,%%lo(D_802194A6)\n"
        "lbu $2,0($3)\n"
        "addiu $sp,$sp,-8\n"
        "blez $2,3f\n"
        "addu $4,$0,$0\n"
        "addiu $9,$0,0x7f\n"
        "lui $8,%%hi(D_80235F00)\n"
        "addiu $8,$8,%%lo(D_80235F00)\n"
        "addiu $7,$0,1\n"
        "addu $6,$3,$0\n"
        "addu $5,$0,$0\n"
        "1:\n"
        "bne $4,$9,2f\n"
        "addu $3,$5,$8\n"
        "addu $3,$0,$0\n"
        "2:\n"
        "lbu $2,0xa($3)\n"
        "andi $2,$2,2\n"
        "bnel $2,$0,4f\n"
        "sw $7,0x74($3)\n"
        "4:\n"
        "lbu $2,0($6)\n"
        "addiu $4,$4,1\n"
        "slt $2,$4,$2\n"
        "bnez $2,1b\n"
        "addiu $5,$5,0x250\n"
        "3:\n"
        "addiu $sp,$sp,8"
        :
        :
        : "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "memory");
}
