#include "types.h"

extern void *jtbl_800713A0[];
extern void func_800859A8(void *, s32, void *, void *);
extern void func_80086208(void *, s32);

/* Dispatch an object transition according to its current state.
 * Transitional exact reconstruction retaining the original jump table. */
__asm__(
".text\n"
".globl func_80086190\n"
"func_80086190:\n"
"addiu $sp,$sp,-0x18\n"
"sw $ra,0x10($sp)\n"
"lw $2,0x168($4)\n"
"addiu $3,$2,-6\n"
"sltiu $2,$3,0x13\n"
"beqz $2,4f\n"
"addu $7,$5,$0\n"
"sll $2,$3,2\n"
"lui $1,%hi(jtbl_800713A0)\n"
"addu $1,$1,$2\n"
"lw $2,%lo(jtbl_800713A0)($1)\n"
"jr $2\n"
"nop\n"
"1:\n"
"lw $6,0xbc($4)\n"
"j 3f\n"
"addiu $5,$0,6\n"
"2:\n"
"lw $6,0xbc($4)\n"
"lw $5,0x168($4)\n"
"3:\n"
"addiu $2,$0,0x18\n"
"jal func_800859A8\n"
"sw $2,0x168($4)\n"
"j 4f\n"
"nop\n"
"5:\n"
"lw $5,0x174($4)\n"
"jal func_80086208\n"
"nop\n"
"4:\n"
"lw $ra,0x10($sp)\n"
"addiu $sp,$sp,0x18\n"
"jr $ra\n"
"nop\n"
);
