#include "types.h"

extern void *Steps_InitStep_Free(u16);
extern u16 Steps_CopyObstacleRef(void *);

/* Follow the linked object chain and report whether it contains target_id.
 * Transitional exact reconstruction: the KMC compiler will not reproduce the
 * original branch-likely schedule from structured C yet. */
__asm__(
".text\n"
".globl func_80080B58\n"
"func_80080B58:\n"
"addiu $sp,$sp,-0x20\n"
"sw $ra,0x18($sp)\n"
"sw $17,0x14($sp)\n"
"sw $16,0x10($sp)\n"
"lhu $2,0xf4($4)\n"
"andi $17,$5,0xffff\n"
"andi $16,$2,0xffff\n"
"1:\n"
"jal Steps_InitStep_Free\n"
"addu $4,$16,$0\n"
"beql $2,$0,2f\n"
"addu $2,$0,$0\n"
"bne $16,$17,3f\n"
"nop\n"
"j 2f\n"
"addiu $2,$0,1\n"
"3:\n"
"jal Steps_CopyObstacleRef\n"
"addu $4,$2,$0\n"
"j 1b\n"
"andi $16,$2,0xffff\n"
"2:\n"
"lw $ra,0x18($sp)\n"
"lw $17,0x14($sp)\n"
"lw $16,0x10($sp)\n"
"addiu $sp,$sp,0x20\n"
"jr $ra\n"
"nop\n"
);
