#include "types.h"

extern f32 D_800716A4;
extern s32 D_8021945C;

void func_80088ABC(void *object, void *owner, s32 value, u8 kind) {
    f32 initial = D_800716A4;
    register s32 timer __asm__("$2");
    register u8 flags __asm__("$3");
    register s32 one __asm__("$2");
    register s32 owner_value __asm__("$3");

    __asm__ volatile("" : "=f"(initial) : "0"(initial));
    timer = D_8021945C;
    flags = *((u8 *)object + 0x16C);

    *(s32 *)((u8 *)object + 0xB4) = 0;
    *(s32 *)((u8 *)object + 0xB8) = 0;
    *((u8 *)object + 0xC4) = 0;
    *((u8 *)object + 0xC5) = 0;
    __asm__ volatile("" : : : "memory");
    *(s32 *)((u8 *)object + 0xB4) = value;
    timer += 0x12C;
    flags &= 0xFE;
    __asm__ volatile("" : : "r"(timer), "r"(flags));
    *(f32 *)((u8 *)object + 0xBC) = initial;
    *(s32 *)((u8 *)object + 0xC0) = timer;
    *((u8 *)object + 0x16C) = flags;
    owner_value = **(s32 **)owner;
    one = 1;
    *((u8 *)object + 0xC4) = kind;
    *(void **)((u8 *)object + 0xC8) = owner;
    *((u8 *)object + 0xC5) = one;
    *(s32 *)((u8 *)object + 0xB8) = owner_value;
}
