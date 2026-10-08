#include "types.h"
#include "m2c_macros.h"

extern u8 *D_80114680;

void func_8007DB84(s32 arg0) {
    register u8 *node __asm__("$5");

    __asm__ volatile(
        "andi $5,%1,0xffff\n"
        "bnez $5,1f\n"
        "sll $2,$5,1\n"
        "j 2f\n"
        "addu $5,$0,$0\n"
        "1:\n"
        "lui $3,%%hi(D_80114680)\n"
        "lw $3,%%lo(D_80114680)($3)\n"
        "addu $2,$2,$5\n"
        "sll $2,$2,3\n"
        "addu $5,$3,$2\n"
        "2:"
        : "=r"(node)
        : "r"(arg0)
        : "$2", "$3");
    __asm__ volatile(
        "lui $2,%%hi(D_80114680)\n"
        "lw $2,%%lo(D_80114680)($2)\n"
        "lui $1,1\n"
        "addu $1,$2,$1\n"
        "lw $3,-0x4000($1)\n"
        "sw $3,4($5)\n"
        "lui $1,1\n"
        "addu $1,$2,$1\n"
        "sw $5,-0x4000($1)"
        :
        : "r"(node)
        : "$1", "$2", "$3", "memory");
    M2C_FIELD(node, s16 *, 8) = arg0;
    M2C_FIELD(node, s8 *, 0) = 1;
}
