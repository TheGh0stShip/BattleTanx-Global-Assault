#include "types.h"

extern s16 D_80114510;
extern u8 D_80114512;
extern u8 D_8017CEE0[];

/* Allocates from the current 0x2000-byte arena and returns its banked address. */
void *func_8007B190(s32 arg0) {
    register void *result __asm__("$2");

    __asm__ volatile(
        "lui $6,%%hi(D_80114510)\n"
        "lh $6,%%lo(D_80114510)($6)\n"
        "addiu $sp,$sp,-8\n"
        "addu $5,$4,$0\n"
        "addu $2,$6,$5\n"
        "slti $2,$2,0x2001\n"
        "beqz $2,1f\n"
        "addu $3,$6,$0\n"
        "lui $4,%%hi(D_80114512)\n"
        "lbu $4,%%lo(D_80114512)($4)\n"
        "addu $2,$3,$5\n"
        "lui $1,%%hi(D_80114510)\n"
        "sh $2,%%lo(D_80114510)($1)\n"
        "sll $2,$6,3\n"
        "lui $3,%%hi(D_8017CEE0)\n"
        "addiu $3,$3,%%lo(D_8017CEE0)\n"
        "addu $2,$2,$3\n"
        "sll $4,$4,16\n"
        "j 2f\n"
        "addu $2,$4,$2\n"
        "1:\n"
        "addu $2,$0,$0\n"
        "2:\n"
        "addiu $sp,$sp,8"
        : "=r"(result)
        :
        : "$1", "$3", "$4", "$5", "$6", "memory");
    return result;
}
