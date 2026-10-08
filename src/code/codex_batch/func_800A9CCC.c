#include "types.h"

extern u8 D_80236B30[];

void func_800A9CCC(s32 arg0, void *arg1, s8 arg2, s8 arg3, u8 arg4, u8 arg5) {
    register u8 *source __asm__("$10");
    register volatile u8 *state __asm__("$3");
    register u8 *entry __asm__("$2");
    register s32 count __asm__("$5");
    register u8 stack_arg4 __asm__("$8");
    register u8 stack_arg5 __asm__("$9");
    register s32 limit __asm__("$2");
    register s32 masked_count __asm__("$4");
    register s32 entry_offset __asm__("$2");
    s32 first;
    s32 second;
    s32 third;

    __asm__ volatile(
        "addu $10,$5,$0\n"
        "andi $4,$4,0xff\n"
        "sll $2,$4,4\n"
        "addu $2,$2,$4\n"
        "sll $2,$2,2\n"
        "subu $2,$2,$4\n"
        "sll $2,$2,2\n"
        "lui $3,%%hi(D_80236B30)\n"
        "addiu $3,$3,%%lo(D_80236B30)\n"
        "addu $3,$2,$3\n"
        "lbu $5,3($3)\n"
        "lbu $8,0x13($sp)\n"
        "lbu $9,0x17($sp)\n"
        "addiu $2,$0,0x10\n"
        "andi $4,$5,0xff"
        : "=r"(source), "=r"(state), "=r"(count),
          "=r"(stack_arg4), "=r"(stack_arg5), "=r"(limit),
          "=r"(masked_count)
        :
        : "$2", "$4", "memory");
    if (masked_count != limit) {
        entry_offset = count + 1;
        state[3] = entry_offset;
        __asm__ volatile("" : "=r"(masked_count) : "0"(masked_count) : "memory");
        entry_offset = masked_count * 0x10;
        __asm__("addiu %0,%0,4" : "=r"(entry_offset) : "0"(entry_offset));
        entry = (u8 *)state + entry_offset;
        entry[0xC] = arg2;
        entry[0xD] = arg3;
        entry[0xE] = stack_arg4;
        entry[0xF] = stack_arg5;
        first = *(s32 *)source;
        second = *(s32 *)(source + 4);
        third = *(s32 *)(source + 8);
        *(s32 *)entry = first;
        *(s32 *)(entry + 4) = second;
        *(s32 *)(entry + 8) = third;
    }
}
