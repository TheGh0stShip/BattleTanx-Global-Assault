#include "types.h"

typedef struct {
    u8 pad_000[0x10];
    s32 identity;
    u8 pad_014[0x23C];
} Entity80083EBC;

typedef struct SearchNode80083EBC {
    u8 pad_00[0xC];
    s32 occupied;
    u8 pad_10[0xD];
    u8 entity_index;
} SearchNode80083EBC;

typedef struct {
    u8 pad_000[0x1D0];
    Entity80083EBC *entity;
} Object80083EBC;

extern Entity80083EBC D_80235F00[];
extern SearchNode80083EBC *func_800A1A28(SearchNode80083EBC *previous, s32 kind);

SearchNode80083EBC *func_80083EBC(Object80083EBC *object) {
    register Object80083EBC *saved_object __asm__("$16");
    register Entity80083EBC *entities __asm__("$17");
    register s32 sentinel __asm__("$18");
    SearchNode80083EBC *node;
    register Entity80083EBC *entity __asm__("$3");
    register u32 raw_index __asm__("$2");
    register u32 entity_index __asm__("$3");
    register u32 entity_offset __asm__("$2");

    saved_object = object;
    node = func_800A1A28(0, 0x14);
    if (node != 0) {
        sentinel = 0x7F;
        entities = D_80235F00;
        do {
            raw_index = node->entity_index;
            __asm__ volatile("" : "=r"(raw_index) : "0"(raw_index));
            if (raw_index == sentinel) {
                entity = 0;
            } else {
                entity_index = raw_index;
                entity_offset = entity_index * sizeof(Entity80083EBC);
                __asm__ volatile("" : "=r"(entity_offset) : "0"(entity_offset));
                entity = (Entity80083EBC *)(entity_offset + (u32)entities);
            }
            if ((entity->identity != saved_object->entity->identity) &&
                (node->occupied != 0)) {
                return node;
            }
            node = func_800A1A28(node, 0x14);
        } while (node != 0);
    }
    return node;
}
