#include "types.h"

/* Read the MIPS CP0 status register. */
__asm__(
    ".text\n"
    ".set noreorder\n"
    ".globl __osGetSR\n"
    ".ent __osGetSR\n"
    "__osGetSR:\n"
    "mfc0 $2,$12\n"
    "jr $31\n"
    "nop\n"
    ".end __osGetSR\n");
