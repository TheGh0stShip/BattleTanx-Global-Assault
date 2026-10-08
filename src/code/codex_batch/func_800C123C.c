#include "types.h"

extern void *jtbl_80073748[];
extern void *func_800BD880(void);
extern void func_800BD8B8(void *);

/* Resolve and submit the current parser-side record.
 * Transitional exact reconstruction retaining the original jump table. */
__asm__(
".text\n"
".globl func_800C123C\n"
"func_800C123C:\n"
"addiu $sp,$sp,-0x18\n"
"sw $ra,0x10($sp)\n"
"jal func_800BD880\n"
"nop\n"
"addu $4,$2,$0\n"
"lw $2,8($4)\n"
"lbu $2,0($2)\n"
"addiu $3,$2,-0x2e\n"
"sltiu $2,$3,0x2d\n"
"beqz $2,4f\n"
"addu $5,$0,$0\n"
"sll $2,$3,2\n"
"lui $1,%hi(jtbl_80073748)\n"
"addu $1,$1,$2\n"
"lw $2,%lo(jtbl_80073748)($1)\n"
"jr $2\n"
"nop\n"
"1:\n"
"j 4f\n"
"addiu $5,$4,-0x80\n"
"2:\n"
"j 4f\n"
"addiu $5,$4,-0xb0\n"
"3:\n"
"addiu $5,$4,-0xa0\n"
"4:\n"
"beqz $5,5f\n"
"nop\n"
"jal func_800BD8B8\n"
"nop\n"
"5:\n"
"lw $ra,0x10($sp)\n"
"addiu $sp,$sp,0x18\n"
"jr $ra\n"
"nop\n"
);
