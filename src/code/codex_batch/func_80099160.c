#include "types.h"

extern u8 D_80114814[];

/* Translate at most length bytes through the game's compact text table. */
void func_80099160(u8 *dst, const u8 *src, s32 length) {
    register u8 *out __asm__("$4") = dst;
    __asm__ volatile(
        "lbu $7,0($5)\n"
        "j 3f\n"
        "addu $8,$0,$0\n"
        "1:\n"
        "addiu $3,$0,0x20\n"
        "addiu $2,$7,-0x42\n"
        "sltiu $2,$2,0x54\n"
        "beqz $2,2f\n"
        "addiu $8,$8,1\n"
        "j 4f\n"
        "addiu $3,$0,0x2a\n"
        "2:\n"
        "addiu $2,$7,-0x10\n"
        "sltiu $2,$2,0x33\n"
        "beqz $2,4f\n"
        "andi $2,$7,0xff\n"
        "lui $1,%%hi(D_80114814)\n"
        "addu $1,$1,$2\n"
        "lbu $3,%%lo(D_80114814)($1)\n"
        "4:\n"
        "sb $3,0($4)\n"
        "lbu $7,0($5)\n"
        "addiu $4,$4,1\n"
        "3:\n"
        "slt $3,$8,$6\n"
        "sltu $2,$0,$7\n"
        "and $2,$2,$3\n"
        "bnel $2,$0,1b\n"
        "addiu $5,$5,1"
        : "=r"(out)
        : "0"(out)
        : "$1", "$2", "$3", "$5", "$7", "$8", "memory");
    *out = 0;
}
