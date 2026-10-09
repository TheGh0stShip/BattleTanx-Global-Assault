#include "types.h"

extern s32 D_803A57E0;
extern s32 D_803A57C4[];
extern s32 D_803A57C0[];

/* Looks up key in the interleaved key/value table. */
s32 func_800B99F8(s32 key) {
    register s32 result __asm__("$2");

    __asm__ volatile(
        "lui $2,%%hi(D_803A57E0)\n"
        "lw $2,%%lo(D_803A57E0)($2)\n"
        "blezl $2,3f\n"
        "addu $2,$0,$0\n"
        "addu $3,$0,$0\n"
        "sll $5,$2,3\n"
        "1:\n"
        "lui $1,%%hi(D_803A57C4)\n"
        "addu $1,$1,$3\n"
        "lw $2,%%lo(D_803A57C4)($1)\n"
        "bnel $4,$2,2f\n"
        "addiu $3,$3,8\n"
        "lui $1,%%hi(D_803A57C0)\n"
        "addu $1,$1,$3\n"
        "lw $2,%%lo(D_803A57C0)($1)\n"
        "j 3f\n"
        "nop\n"
        "2:\n"
        "slt $2,$3,$5\n"
        "bnez $2,1b\n"
        "addu $2,$0,$0\n"
        "3:"
        : "=r"(result)
        : "r"(key)
        : "$1", "$3", "$5", "memory");
    return result;
}
