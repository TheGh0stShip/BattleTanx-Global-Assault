#include "types.h"

typedef struct PoolRecord8007E7A8 {
    u8 type;
    u8 pad01;
    u16 field02;
    f32 x;
    f32 y;
    u8 pad0C[0xA];
    u16 link_16;
} PoolRecord8007E7A8;

typedef struct ObjectState8007E7A8 {
    u32 field00;
    u16 head;
    u16 count;
    u8 pad08[2];
    u16 field0A;
    u8 pad0C[8];
    u16 field14;
    u16 field16;
    u32 field18;
    u8 pad1C[4];
    f32 x;
    f32 y;
    u8 pad28[0x1A];
    u16 field42;
} ObjectState8007E7A8;

extern PoolRecord8007E7A8 *func_8007DA5C(u16 index);
extern void func_8007DB84(u16 index);
extern u16 func_8007DBE0(u16 *state);

void func_8007E7A8(u8 *object) {
    register u8 *object_base __asm__("$16") = object;
    register PoolRecord8007E7A8 *record __asm__("$18");
    PoolRecord8007E7A8 *next;
    register u16 head __asm__("$21");
    register u16 link __asm__("$20");
    register u16 next_index __asm__("$17");
    u16 removed;
    f32 x;
    register ObjectState8007E7A8 *state __asm__("$19");

    head = *(u16 *)(object_base + 0xF4);
    state = (ObjectState8007E7A8 *)(object_base + 0xF0);
    if (head == 0) {
        return;
    }

    *(u16 *)(object_base + 0x132) = 0;
    record = func_8007DA5C(head);
    x = record->x;
    __asm__ volatile("" : "=f"(x) : "0"(x));
    link = record->link_16;
    __asm__ volatile("" : "=r"(link) : "0"(link));
    *(f32 *)(object_base + 0x110) = x;
    *(f32 *)(object_base + 0x114) = record->y;
    *(u16 *)(object_base + 0xFA) = 0;
    *(u16 *)(object_base + 0x104) = 0;
    *(u16 *)(object_base + 0x106) = 0;
    *(u32 *)(object_base + 0x108) = 0;

    next_index = link;
    if (next_index == 0) {
        *(u16 *)(object_base + 0xF4) = 0;
        *(u16 *)(object_base + 0xF6) -= 1;
    } else {
        next = func_8007DA5C(next_index);
        switch (next->type) {
            case 3:
                next->field02 = 0;
                *(u16 *)(object_base + 0xF4) = link;
                *(u16 *)(object_base + 0xF6) -= 1;
                break;
            case 4:
                *(u32 *)(object_base + 0xF0) = 0;
                func_8007DB84(next_index);
                *(u16 *)(object_base + 0xF4) = 0;
                *(u16 *)(object_base + 0xF6) -= 2;
                break;
            default:
                removed = func_8007DBE0(&record->link_16);
                state->head = 0;
                state->count -= removed;
                break;
        }
    }
    func_8007DB84(head);
}
