#include "types.h"

s32 func_8008E6BC(void *unused, s32 index) {
    register s32 result __asm__("$2");

    if (index == 1) {
        return 1;
    }
    __asm__ volatile(
        "lui $1,%%hi(D_801146D4)\n"
        "addu $1,$1,$5\n"
        "lbu $2,%%lo(D_801146D4)($1)"
        : "=r"(result)
        : "r"(index)
        : "$1");
    __asm__ volatile("" : "=r"(result) : "0"(result));
    return result < 11;
}
