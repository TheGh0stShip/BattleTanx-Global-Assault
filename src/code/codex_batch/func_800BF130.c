#include "types.h"

extern u8 D_803A57F0[];
extern u8 D_803A57F2[];
extern u8 D_803A57FC[];

/* Clears the active flag for the first matching entry in the 20-slot table. */
void func_800BF130(s32 arg0) {
    __asm__ volatile(
        "addu $3,$0,$0\n"
        "andi $2,$3,0xffff\n"
        "1:\n"
        "sll $5,$2,4\n"
        "lui $1,%%hi(D_803A57FC)\n"
        "addu $1,$1,$5\n"
        "lw $2,%%lo(D_803A57FC)($1)\n"
        "bne $2,$4,2f\n"
        "addiu $3,$3,1\n"
        "lui $1,%%hi(D_803A57F0)\n"
        "addu $1,$1,$5\n"
        "lhu $2,%%lo(D_803A57F0)($1)\n"
        "lui $1,%%hi(D_803A57F0)\n"
        "addu $1,$1,$5\n"
        "lhu $3,%%lo(D_803A57F0)($1)\n"
        "andi $2,$2,0xfffd\n"
        "lui $1,%%hi(D_803A57F2)\n"
        "addu $1,$1,$5\n"
        "sh $3,%%lo(D_803A57F2)($1)\n"
        "lui $1,%%hi(D_803A57F0)\n"
        "addu $1,$1,$5\n"
        "sh $2,%%lo(D_803A57F0)($1)\n"
        "j 3f\n"
        "nop\n"
        "2:\n"
        "andi $2,$3,0xffff\n"
        "sltiu $2,$2,0x14\n"
        "bnez $2,1b\n"
        "andi $2,$3,0xffff\n"
        "3:"
        :
        : "r"(arg0)
        : "$1", "$2", "$3", "$5", "memory");
}
