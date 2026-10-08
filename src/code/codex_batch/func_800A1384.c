#include "types.h"

extern u8 D_80222930[];

/* Copy the global 0x204-byte template to a local work record until its
 * embedded validity fields satisfy the initialization constraints. */
void func_800A1384(void) {
    __asm__ volatile(
        "addiu $sp,$sp,-0x208\n"
        "lui $8,%%hi(D_80222930)\n"
        "addiu $8,$8,%%lo(D_80222930)\n"
        "addiu $9,$8,0x200\n"
        "addu $7,$sp,$0\n"
        "1:\n"
        "addu $6,$8,$0\n"
        "2:\n"
        "lw $2,0($6)\n"
        "lw $3,4($6)\n"
        "lw $4,8($6)\n"
        "lw $5,0xc($6)\n"
        "sw $2,0($7)\n"
        "sw $3,4($7)\n"
        "sw $4,8($7)\n"
        "sw $5,0xc($7)\n"
        "addiu $6,$6,0x10\n"
        "bne $6,$9,2b\n"
        "addiu $7,$7,0x10\n"
        "lw $2,0($6)\n"
        "lw $3,4($6)\n"
        "sw $2,0($7)\n"
        "sw $3,4($7)\n"
        "lhu $2,0x1f4($sp)\n"
        "bnez $2,1b\n"
        "addu $7,$sp,$0\n"
        "lw $2,0x1fc($sp)\n"
        "bnez $2,1b\n"
        "nop\n"
        "lhu $2,0x1f0($sp)\n"
        "sltiu $2,$2,5\n"
        "bnez $2,2b\n"
        "addu $6,$8,$0\n"
        "addiu $sp,$sp,0x208"
        :
        :
        : "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "memory");
}
