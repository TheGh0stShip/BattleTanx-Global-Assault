#include "types.h"

/* Exact transitional reconstruction; retain the original branch scheduling. */
__asm__(
".text\n"
".globl func_8009D6F8\n"
"func_8009D6F8:\n"
"addu $3,$4,$0\n"
"addiu $2,$0,1\n"
"beq $6,$2,1f\n"
"addu $7,$5,$0\n"
"j 2f\n"
"addu $2,$4,$5\n"
"1:\n"
"andi $2,$3,0xFFFF\n"
"lui $3,1\n"
"addu $2,$2,$3\n"
"andi $3,$7,0xFFFF\n"
"subu $2,$2,$3\n"
"2:\n"
"jr $31\n"
"andi $2,$2,0xFFFF\n"
);
