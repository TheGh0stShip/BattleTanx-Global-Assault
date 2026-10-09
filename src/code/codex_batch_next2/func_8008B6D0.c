#include "types.h"

typedef struct Object8B6D0 Object8B6D0;

struct Object8B6D0 {
    void *descriptor;
    Object8B6D0 *next;
    u8 pad08[0x94];
    s32 list_index;
    u8 padA0[0x130];
    u8 *owner;
    u8 pad1D4[0xC];
    u32 flags;
};

extern u16 D_801AD120;
extern void func_80089F40(Object8B6D0 *object, s32 old_index, s32 new_index,
                         s32 arg3);

void func_8008B6D0(Object8B6D0 *object) {
    register s32 old_index __asm__("$7") = object->list_index;
    register u32 count __asm__("$4");
    register void *descriptor __asm__("$5");
    register u8 *owner __asm__("$3");
    register u32 flags __asm__("$2");
    register Object8B6D0 *call_object __asm__("$4");
    register s32 call_old_index __asm__("$5");
    register s32 call_new_index __asm__("$6");
    register s32 call_arg3 __asm__("$7");
    s32 mask;
    Object8B6D0 **link = (Object8B6D0 **)(object->owner + 0x1B8 + old_index * 4);

    __asm__ volatile("" : "=r"(old_index) : "0"(old_index));

    while (*link != 0 && *link != object) {
        link = &(*link)->next;
    }
    *link = object->next;
    owner = object->owner;
    owner += old_index;
    owner[0x1C8]--;
    count = D_801AD120;
    flags = object->flags;
    descriptor = object->descriptor;
    mask = -2;
    count--;
    flags &= mask;
    object->flags = flags;
    flags = 4;
    D_801AD120 = count;
    call_object = object;
    __asm__ volatile("" : "=r"(call_object) : "0"(call_object));
    *(s32 *)((u8 *)descriptor + 0xC) = 0;
    call_old_index = old_index;
    __asm__ volatile("" : "=r"(call_old_index) : "0"(call_old_index));
    object->list_index = flags;
    __asm__ volatile("" ::: "memory");
    call_new_index = 4;
    __asm__ volatile("" : "=r"(call_new_index) : "0"(call_new_index));
    call_arg3 = 0;
    func_80089F40(call_object, call_old_index, call_new_index, call_arg3);
}
