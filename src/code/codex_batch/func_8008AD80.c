#include "types.h"

extern void _bzero(void *, s32);
extern u8 D_801AD128[];
extern s16 D_801AD120;
extern u8 D_801AD1CC[];

/* Clear the spatial grid and initialize each row's ownership bit.
 * Transitional exact reconstruction for the original address scheduling. */
__asm__(
".text\n"
".globl func_8008AD80\n"
"func_8008AD80:\n"
"addiu $sp,$sp,-0x18\n"
"lui $4,%hi(D_801AD128)\n"
"addiu $4,$4,%lo(D_801AD128)\n"
"sw $ra,0x10($sp)\n"
"jal _bzero\n"
"addiu $5,$0,0x7320\n"
"addu $5,$0,$0\n"
"addiu $6,$0,1\n"
"lui $1,%hi(D_801AD120)\n"
"sh $0,%lo(D_801AD120)($1)\n"
"andi $3,$5,0xffff\n"
"1:\n"
"sllv $4,$6,$3\n"
"sll $2,$3,2\n"
"addu $2,$2,$3\n"
"sll $2,$2,2\n"
"subu $2,$2,$3\n"
"sll $2,$2,2\n"
"addu $2,$2,$3\n"
"sll $2,$2,2\n"
"subu $2,$2,$3\n"
"sll $2,$2,2\n"
"lui $1,%hi(D_801AD1CC)\n"
"addu $1,$1,$2\n"
"sw $4,%lo(D_801AD1CC)($1)\n"
"addiu $5,$5,1\n"
"andi $2,$5,0xffff\n"
"sltiu $2,$2,0x18\n"
"bnez $2,1b\n"
"andi $3,$5,0xffff\n"
"lw $ra,0x10($sp)\n"
"addiu $sp,$sp,0x18\n"
"jr $ra\n"
"nop\n"
);
