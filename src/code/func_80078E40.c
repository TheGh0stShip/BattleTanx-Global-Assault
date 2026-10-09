#include "types.h"

/* Write the MIPS CP0 status register. */
__asm__(
    ".text\n"
    ".set noreorder\n"
    ".globl func_80078E40\n"
    ".ent func_80078E40\n"
    "func_80078E40:\n"
    "mtc0 $4,$12\n"
    "jr $31\n"
    "nop\n"
    ".end func_80078E40\n");
