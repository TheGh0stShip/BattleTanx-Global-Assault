#include "types.h"

/* Insert a key/value pair into the small fixed-capacity table unless the key
 * is already present. Contained assembly preserves the original KMC register
 * allocation and scheduling while the surrounding types are reconstructed. */
s32 func_800B9EB4(void *arg0, s32 arg1, s32 arg2) {
    register s32 result __asm__("$2");
    __asm__ volatile(
        ".set noreorder\n"
        "addu $7,$4,$0\n"
        "lw $2,8($7)\n"
        "addiu $sp,$sp,-8\n"
        "blez $2,2f\n"
        "addu $3,$0,$0\n"
        "addu $8,$2,$0\n"
        "1:\n"
        "lw $2,0xc($4)\n"
        "beq $5,$2,4f\n"
        "addiu $3,$3,1\n"
        "slt $2,$3,$8\n"
        "bnez $2,1b\n"
        "addiu $4,$4,0xc\n"
        "2:\n"
        "lw $3,8($7)\n"
        "slti $2,$3,0x100\n"
        "beqz $2,5f\n"
        "sll $2,$3,1\n"
        "addu $2,$2,$3\n"
        "sll $2,$2,2\n"
        "addu $2,$7,$2\n"
        "sw $5,0xc($2)\n"
        "lw $4,8($7)\n"
        "addu $2,$0,$0\n"
        "addiu $3,$4,1\n"
        "sw $3,8($7)\n"
        "sll $3,$4,1\n"
        "addu $3,$3,$4\n"
        "sll $3,$3,2\n"
        "addu $3,$7,$3\n"
        ".word 0x0802e7ce\n"
        "sw $6,0x10($3)\n"
        "4:\n"
        ".word 0x0802e7ce\n"
        "addu $2,$0,$0\n"
        "5:\n"
        "addiu $2,$0,-1\n"
        "6:\n"
        "addiu $sp,$sp,8\n"
        ".set reorder"
        : "=r"(result)
        :
        : "$3", "$4", "$5", "$6", "$7", "$8", "memory");
    return result;
}
