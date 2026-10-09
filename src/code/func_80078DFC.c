#include "types.h"

/* Handwritten instruction-cache invalidation routine. */
__asm__(
    ".text\n"
    ".set noreorder\n"
    ".globl func_80078DFC\n"
    ".ent func_80078DFC\n"
    "func_80078DFC:\n"
    "lui $8,0x8000\n"
    "addiu $9,$8,0x1ff0\n"
    "1:\n"
    "cache 1,0($8)\n"
    "bne $8,$9,1b\n"
    "addiu $8,$8,0x10\n"
    "jr $31\n"
    "nop\n"
    ".end func_80078DFC\n");
