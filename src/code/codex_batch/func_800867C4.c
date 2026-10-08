#include "types.h"

extern s32 D_802194A0;
extern void *jtbl_80071508[];
extern u8 *D_80114680;
extern void func_8008723C(void *, s32, s32, s32);

/* Select and submit the current object interaction mode.
 * Transitional exact reconstruction retaining the original jump table. */
__asm__(
".text\n"
".globl func_800867C4\n"
"func_800867C4:\n"
"lui $2,%hi(D_802194A0)\n"
"lw $2,%lo(D_802194A0)($2)\n"
"addiu $sp,$sp,-0x18\n"
"addiu $3,$2,-1\n"
"sltiu $2,$3,0xe\n"
"beqz $2,3f\n"
"sw $ra,0x10($sp)\n"
"sll $2,$3,2\n"
"lui $1,%hi(jtbl_80071508)\n"
"addu $1,$1,$2\n"
"lw $2,%lo(jtbl_80071508)($1)\n"
"jr $2\n"
"nop\n"
"1:\n"
"j 4f\n"
"addiu $5,$0,2\n"
"2:\n"
"lui $2,%hi(D_80114680)\n"
"lw $2,%lo(D_80114680)($2)\n"
"lui $1,1\n"
"addu $1,$2,$1\n"
"lw $3,-0x3f90($1)\n"
"lw $2,0x10($4)\n"
"bne $3,$2,4f\n"
"addiu $5,$0,7\n"
"3:\n"
"addiu $5,$0,1\n"
"4:\n"
"addu $6,$0,$0\n"
"jal func_8008723C\n"
"addu $7,$0,$0\n"
"lw $ra,0x10($sp)\n"
"addiu $sp,$sp,0x18\n"
"jr $ra\n"
"nop\n"
);
