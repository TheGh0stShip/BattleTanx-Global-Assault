#include "types.h"

extern s32 D_8021945C;

/* Advance the two timed record cursors when their delays expire. */
void func_8009025C(void *arg0) {
    __asm__ volatile(
        "addu $6,$0,$0\n"
        "lui $7,%%hi(D_8021945C)\n"
        "lw $7,%%lo(D_8021945C)($7)\n"
        "addiu $8,$0,-0x3e8\n"
        "1:\n"
        "lw $2,0x230($4)\n"
        "bltzl $2,3f\n"
        "addiu $6,$6,1\n"
        "lw $5,0x238($4)\n"
        "lw $3,8($5)\n"
        "subu $2,$7,$2\n"
        "slt $2,$2,$3\n"
        "bnel $2,$0,3f\n"
        "addiu $6,$6,1\n"
        "addiu $2,$5,0xc\n"
        "sw $2,0x238($4)\n"
        "lw $2,0x14($5)\n"
        "bnel $2,$0,2f\n"
        "sw $7,0x230($4)\n"
        "sw $8,0x230($4)\n"
        "2:\n"
        "addiu $6,$6,1\n"
        "3:\n"
        "sltiu $2,$6,2\n"
        "bnez $2,1b\n"
        "addiu $4,$4,4"
        :
        :
        : "$2", "$3", "$4", "$5", "$6", "$7", "$8", "memory");
}
