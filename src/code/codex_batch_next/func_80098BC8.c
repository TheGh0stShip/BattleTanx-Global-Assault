#include "types.h"

void func_80098BC8(s32 controller) {
    __asm__ volatile(
        "sll $4,$4,2\n"
        "lui $1,%%hi(D_80216FA0)\n"
        "addu $1,$1,$4\n"
        "lw $3,%%lo(D_80216FA0)($1)\n"
        "bltz $3,1f\n"
        "sll $3,$3,2\n"
        "addiu $2,$0,1\n"
        "lui $1,%%hi(D_80216FD0)\n"
        "addu $1,$1,$3\n"
        "sw $2,%%lo(D_80216FD0)($1)\n"
        "1:"
        :
        :
        : "$1", "$2", "$3", "$4", "memory");
}
