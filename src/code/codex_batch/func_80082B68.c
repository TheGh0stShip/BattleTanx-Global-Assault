#include "types.h"

extern s32 D_8021945C;

/* Select one of the object's timed states. */
void func_80082B68(void *arg0, s32 state) {
    __asm__ volatile(
        "addiu $2,$0,2\n"
        "beq $5,$2,2f\n"
        "nop\n"
        "sltiu $2,$5,3\n"
        "beqz $2,1f\n"
        "addiu $2,$0,1\n"
        "beql $5,$2,4f\n"
        "sw $5,0x158($4)\n"
        "j 4f\n"
        "nop\n"
        "1:\n"
        "addiu $2,$0,3\n"
        "beq $5,$2,3f\n"
        "nop\n"
        "j 4f\n"
        "nop\n"
        "2:\n"
        "lui $2,%%hi(D_8021945C)\n"
        "lw $2,%%lo(D_8021945C)($2)\n"
        "sw $5,0x158($4)\n"
        "j 5f\n"
        "addiu $2,$2,0x78\n"
        "3:\n"
        "lui $2,%%hi(D_8021945C)\n"
        "lw $2,%%lo(D_8021945C)($2)\n"
        "sw $5,0x158($4)\n"
        "addiu $2,$2,0x1e\n"
        "5:\n"
        "sw $2,0x15c($4)\n"
        "4:"
        :
        :
        : "$2", "$4", "$5", "memory");
}
