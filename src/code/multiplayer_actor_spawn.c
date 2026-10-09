#include "types.h"

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct ActorEntry {
    u8 pad[0x42];
    s16 index;
} ActorEntry;

typedef struct ActorNode {
    u8 pad[0x10];
    Vec3f pos;
    u8 type;
    u8 slot;
    u8 pad1E[2];
    ActorEntry* actor;
    u8 pad24[0x20];
} ActorNode;

typedef struct ActorSlot {
    u8 pad[0x10];
    s32 base;
    u8 pad14[0x24];
} ActorSlot;

typedef struct ActorTable {
    u8 pad[4];
    ActorSlot* slots;
} ActorTable;

typedef struct ActorSource {
    u8 pad;
    u8 team;
    u16 id;
    s32 offset;
} ActorSource;

extern u32 D_802194A0;
extern u8 D_80219582[];
extern u8 D_802194A6;
extern s32 D_803A5610;
extern ActorNode D_80224EF0[];

extern ActorNode* func_800A18D0(s32, s32);
extern ActorEntry* func_800DD3A0(s32, ActorTable*, Vec3f*, u16, u8, s32);
extern s32 func_800DF758(ActorTable*, s32, u16);
extern void func_800E6540(ActorNode*, s32, s32);

static inline s32 is_multiplayer_actor_mode(void) {
    switch (D_802194A0) {
        case 1:
        case 3:
        case 11:
            return 1;
    }
    return 0;
}

void func_800E80F4(ActorSource* source, ActorTable* table, Vec3f* pos,
                   u16 id, u8 type, s32 slot) {
    ActorNode* node;
    s32 count;
    ActorEntry* actor;

    if (is_multiplayer_actor_mode()) {
        count = D_80219582[source->team];
        if (count < D_802194A6) {
            node = func_800A18D0(20, 36);
            if (node != 0) {
                node->pos = *pos;
                node->type = type;
                node->slot = count;
                actor = func_800DD3A0(table->slots[slot].base + source->offset,
                                      table, pos, id, type, slot);
                node->actor = actor;
                actor->index = ((u32)node - (u32)D_80224EF0) /
                               sizeof(ActorNode);

                /* Preserve the retail compiler's second mode-load position. */
                __asm__("");
                switch (D_802194A0) {
                    case 1:
                        func_800E6540(node, D_803A5610, 0);
                        break;
                    case 11:
                        func_800E6540(node,
                                      func_800DF758(table, slot, source->id),
                                      0);
                        break;
                }
            }
        }
    }
}
