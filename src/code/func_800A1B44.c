#include "types.h"

extern s16 D_80235EF0;
extern u16 D_80224E68[];
extern s32 D_80224EF4;
extern s16 D_80224EF8;

void func_800A1B44(void) {
    volatile s32 stack_pad[2];
    register s32 head __asm__("$3");
    register s32 current __asm__("$6");
    register u16 *buckets __asm__("$7");
    register s32 sentinel __asm__("$8");
    register s32 signed_index __asm__("$2");
    register u32 offset __asm__("$3");
    register s32 group __asm__("$2");
    register s32 next __asm__("$5");
    register u16 old_head __asm__("$4");

    head = D_80235EF0;
    if (head != -1) {
        current = head;
        buckets = D_80224E68;
        sentinel = -1;
        do {
            signed_index = (s16)current;
            offset = signed_index * 0x44;
            group = *(s32 *)((u8 *)&D_80224EF4 + offset);
            next = *(s16 *)((u8 *)&D_80224EF8 + offset);
            old_head = buckets[group];

            D_80235EF0 = next;
            group = *(s32 *)((u8 *)&D_80224EF4 + offset);
            *(s16 *)((u8 *)&D_80224EF8 + offset) = old_head;
            buckets[group] = current;
            current = next;
        } while (next != sentinel);
    }
}
