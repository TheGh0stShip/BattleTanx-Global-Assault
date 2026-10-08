#include "types.h"

extern u8 *D_80219498;
extern u8 D_80236B33[];

/* Clears each active player's per-player state byte. */
void func_800A9EC0(void) {
    __asm__ volatile(
        "lui $2,%%hi(D_80219498)\n"
        "lw $2,%%lo(D_80219498)($2)\n"
        "lbu $2,0xc4($2)\n"
        "addiu $sp,$sp,-8\n"
        "blez $2,2f\n"
        "addu $3,$0,$0\n"
        "addu $4,$0,$0\n"
        "1:\n"
        "lui $1,%%hi(D_80236B33)\n"
        "addu $1,$1,$4\n"
        "sb $0,%%lo(D_80236B33)($1)\n"
        "lui $2,%%hi(D_80219498)\n"
        "lw $2,%%lo(D_80219498)($2)\n"
        "lbu $2,0xc4($2)\n"
        "addiu $3,$3,1\n"
        "slt $2,$3,$2\n"
        "bnez $2,1b\n"
        "addiu $4,$4,0x10c\n"
        "2:\n"
        "addiu $sp,$sp,8"
        :
        :
        : "$1", "$2", "$3", "$4", "memory");
}
