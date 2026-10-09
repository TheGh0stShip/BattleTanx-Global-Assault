#include "types.h"

extern f32 D_8007121C;
extern f32 D_80071220;
extern f32 D_80071224;

/* Initialize an object's movement timer and copy its current position. */
void func_80082004(void *arg0) {
    register u8 *position __asm__("$3");
    register f32 last __asm__("$f0");
    __asm__ volatile(
        ".set noreorder\n"
        "lwc1 $f2,0x30($4)\n"
        "mtc1 $0,$f0\n"
        "c.le.s $f2,$f0\n"
        "nop\n"
        "bc1f 1f\n"
        "addiu $3,$4,0x134\n"
        "lhu $2,0x20($4)\n"
        "swc1 $f0,0x140($4)\n"
        "j 2f\n"
        "sh $2,0x13c($4)\n"
        "1:\n"
        "lui $1,%%hi(D_8007121C)\n"
        "lwc1 $f0,%%lo(D_8007121C)($1)\n"
        ".word 0x46001002\n"
        "lui $1,%%hi(D_80071220)\n"
        "lwc1 $f2,%%lo(D_80071220)($1)\n"
        "div.s $f0,$f0,$f2\n"
        "lui $1,%%hi(D_80071224)\n"
        "lwc1 $f2,%%lo(D_80071224)($1)\n"
        "lhu $2,0x48($4)\n"
        "add.s $f0,$f0,$f2\n"
        "sh $2,0x13c($4)\n"
        "swc1 $f0,0x140($4)\n"
        "2:\n"
        "lwc1 $f0,8($4)\n"
        "swc1 $f0,0x10($3)\n"
        "lwc1 $f0,0xc($4)"
        : "=r"(position), "=f"(last)
        :
        : "$1", "$2", "$4", "$f2", "memory");
    *(f32 *)(position + 0x14) = last;
}
