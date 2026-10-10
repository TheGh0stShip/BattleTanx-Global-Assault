#include "types.h"

extern s32 D_8021945C;

void func_80088B18(void *object, void *owner, s32 value, u8 kind) {
    f32 initial = 64000.0f;
    s32 timer = D_8021945C;
    u8 flags = *(u8 *)((u8 *)object + 0x16C);

    *(s32 *)((u8 *)object + 0xB4) = 0;
    *(s32 *)((u8 *)object + 0xB8) = 0;
    *(u8 *)((u8 *)object + 0xC4) = 0;
    *(u8 *)((u8 *)object + 0xC5) = 0;
    *(s32 *)((u8 *)object + 0xB4) = value;
    *(s32 *)((u8 *)object + 0xB8) = 0;
    *(u8 *)((u8 *)object + 0xC4) = kind;
    *(void **)((u8 *)object + 0xC8) = owner;
    *(u8 *)((u8 *)object + 0xC5) = 3;
    *(f32 *)((u8 *)object + 0xBC) = initial;
    *(s32 *)((u8 *)object + 0xC0) = timer + 0x12C;
    *(u8 *)((u8 *)object + 0x16C) = flags & 0xFE;
}
