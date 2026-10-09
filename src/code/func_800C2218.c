#include "types.h"
#include "m2c_macros.h"

M2C_UNK func_800C1F08(u16, M2C_UNK, void *);
extern u32 D_80117F24[];

s32 func_800C2218(void *arg0) {
    register M2C_UNK var_a1 __asm__("$5");
    u16 temp_a0;

    temp_a0 = M2C_FIELD(arg0, u16 *, 0x14);
    __asm__ volatile(
        "sll $2,%1,2\n"
        "lui $1,%%hi(D_80117F24)\n"
        "addu $1,$1,$2\n"
        "lw $3,%%lo(D_80117F24)($1)\n"
        "addiu $2,$0,2\n"
        "beq $3,$2,1f\n"
        "addiu $5,$0,4\n"
        "sltiu $2,$3,3\n"
        "bnez $2,1f\n"
        "addiu $5,$0,2\n"
        "addiu $2,$0,3\n"
        "beq $3,$2,1f\n"
        "addiu $5,$0,1\n"
        "addiu $2,$0,4\n"
        "beq $3,$2,1f\n"
        "addiu $5,$0,3\n"
        "addiu $5,$0,2\n"
        "1:"
        : "=r"(var_a1) : "r"(temp_a0) : "$1", "$2", "$3");
    func_800C1F08(temp_a0, var_a1, arg0);
    return 0;
}
