#include "types.h"

typedef struct {
    u8 pad_000[0x10];
    s32 identity;
} Entity80089E84;

typedef struct {
    u8 pad_000[0x1D0];
    Entity80089E84 *entity;
    u8 pad_1D4[0xC];
    u32 flags;
    u8 pad_1E4[0x2E8];
} Object80089E84;

typedef struct {
    u32 count;
    Object80089E84 *objects[24];
} ObjectList80089E84;

extern Object80089E84 D_801AD128[];

u16 func_80089E84(ObjectList80089E84 *list, u32 mask, s32 identity,
                   s32 include_match, s32 include_other) {
    register u16 count __asm__("$8");
    register u16 index __asm__("$9");
    register u32 bit __asm__("$10");
    register Object80089E84 *objects __asm__("$11");
    register s32 saved_include_other __asm__("$12");

    saved_include_other = include_other;
    count = 0;
    index = 0;
    bit = 1;
    objects = D_801AD128;
    do {
        if ((bit & mask) != 0) {
            register u32 offset __asm__("$3");
            Object80089E84 *object;

            offset = index * sizeof(Object80089E84);
            __asm__ volatile("" : "=r"(offset) : "0"(offset));
            object = (Object80089E84 *)(offset + (u32)objects);
            if ((object->flags & 1) != 0) {
                if (object->entity->identity == identity) {
                    if (include_match != 0) {
                        list->objects[count] = object;
                        count++;
                    }
                } else if (saved_include_other != 0) {
                    list->objects[count] = object;
                    count++;
                }
            }
        }
        index++;
        bit <<= 1;
    } while (index < 24);
    list->count = count;
    return count;
}
