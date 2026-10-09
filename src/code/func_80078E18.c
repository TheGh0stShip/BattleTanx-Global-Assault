#include "types.h"

/* Handwritten data-cache invalidation routine. */
__asm__(
    ".text\n"
    ".set noreorder\n"
    ".globl func_80078E18\n"
    ".ent func_80078E18\n"
    "func_80078E18:\n"
    "lui $8,0x8000\n"
    "addiu $9,$8,0x3fe0\n"
    "1:\n"
    "cache 0,0($8)\n"
    "bne $8,$9,1b\n"
    "addiu $8,$8,0x20\n"
    "jr $31\n"
    "nop\n"
    ".end func_80078E18\n");
