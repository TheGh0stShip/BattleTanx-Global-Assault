#include "types.h"

extern s32 D_801ACEF0;
extern s16 D_801ACEF4;
extern u8 D_801ACEF8[];
extern u8 D_801ACEF9[];

/* Initialize the 24-by-24 lookup grid and its control fields. */
void func_80089610(void) {
    __asm__ volatile(
        "addu $5,$0,$0\n"
        "addu $4,$0,$0\n"
        "j 2f\n"
        "addiu $3,$4,1\n"
        "1:\n"
        "addiu $5,$5,1\n"
        "sll $2,$2,1\n"
        "lui $1,%%hi(D_801ACEF9)\n"
        "addu $1,$1,$2\n"
        "sb $3,%%lo(D_801ACEF9)($1)\n"
        "addiu $3,$3,1\n"
        "lui $1,%%hi(D_801ACEF8)\n"
        "addu $1,$1,$2\n"
        "sb $4,%%lo(D_801ACEF8)($1)\n"
        "2:\n"
        "andi $2,$3,0xff\n"
        "sltiu $2,$2,0x18\n"
        "bnez $2,1b\n"
        "andi $2,$5,0xffff\n"
        "addiu $4,$4,1\n"
        "andi $2,$4,0xff\n"
        "sltiu $2,$2,0x18\n"
        "bnel $2,$0,2b\n"
        "addiu $3,$4,1\n"
        "addiu $2,$0,0x113\n"
        "lui $1,%%hi(D_801ACEF4)\n"
        "sh $2,%%lo(D_801ACEF4)($1)\n"
        "lui $1,%%hi(D_801ACEF0)\n"
        "sw $0,%%lo(D_801ACEF0)($1)"
        :
        :
        : "$1", "$2", "$3", "$4", "$5", "memory");
}
