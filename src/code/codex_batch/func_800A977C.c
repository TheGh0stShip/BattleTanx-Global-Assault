#include "types.h"

extern u8 D_80122EC8[];

/* Sum the resource costs of both linked object lists. */
s32 func_800A977C(void *arg0) {
    register s32 sum __asm__("$6");
    __asm__ volatile(
        "lw $5,0x1c4($4)\n"
        "beqz $5,2f\n"
        "addu $6,$0,$0\n"
        "1:\n"
        "lw $2,0x98($5)\n"
        "sll $3,$2,1\n"
        "addu $3,$3,$2\n"
        "sll $3,$3,2\n"
        "addu $3,$3,$2\n"
        "sll $3,$3,4\n"
        "lui $1,%%hi(D_80122EC8)\n"
        "addu $1,$1,$3\n"
        "lw $2,%%lo(D_80122EC8)($1)\n"
        "lw $5,4($5)\n"
        "bnez $5,1b\n"
        "addu $6,$6,$2\n"
        "2:\n"
        "lw $5,0x1b8($4)\n"
        "beqz $5,4f\n"
        "nop\n"
        "3:\n"
        "lw $2,0x98($5)\n"
        "sll $3,$2,1\n"
        "addu $3,$3,$2\n"
        "sll $3,$3,2\n"
        "addu $3,$3,$2\n"
        "sll $3,$3,4\n"
        "lui $1,%%hi(D_80122EC8)\n"
        "addu $1,$1,$3\n"
        "lw $2,%%lo(D_80122EC8)($1)\n"
        "lw $5,4($5)\n"
        "bnez $5,3b\n"
        "addu $6,$6,$2\n"
        "4:"
        : "=r"(sum)
        :
        : "$1", "$2", "$3", "$4", "$5", "memory");
    return sum;
}
