#include "types.h"

extern f32 D_800710B0;
extern f32 D_800710B4;
extern void func_800B5728(u16, u8, u16);

/* Convert a floating heading to the target's packed-angle representation.
 * Transitional exact reconstruction for the original spill schedule. */
__asm__(
".text\n"
".globl func_8007D5B0\n"
"func_8007D5B0:\n"
"lui $1,%hi(D_800710B0)\n"
"lwc1 $f0,%lo(D_800710B0)($1)\n"
"mtc1 $5,$f4\n"
"addiu $sp,$sp,-0x20\n"
"add.s $f2,$f4,$f0\n"
"lui $1,%hi(D_800710B4)\n"
"lwc1 $f0,%lo(D_800710B4)($1)\n"
"lhu $3,4($4)\n"
"addu $8,$6,$0\n"
"c.le.s $f0,$f2\n"
"lbu $5,6($4)\n"
"bc1t 1f\n"
"sw $ra,0x18($sp)\n"
"trunc.w.s $f0,$f2\n"
"mfc1 $6,$f0\n"
"j 2f\n"
"sw $8,0x10($sp)\n"
"1:\n"
"sub.s $f0,$f2,$f0\n"
"trunc.w.s $f2,$f0\n"
"mfc1 $6,$f2\n"
"lui $2,0x8000\n"
"or $6,$6,$2\n"
"sw $8,0x10($sp)\n"
"2:\n"
"addu $4,$3,$0\n"
"jal func_800B5728\n"
"andi $6,$6,0xffff\n"
"lw $ra,0x18($sp)\n"
"addiu $sp,$sp,0x20\n"
"jr $ra\n"
"nop\n"
);
