#include "types.h"

typedef struct HudBind HudBind;

typedef struct {
    u8 tag;
    u8 pad1[0xB];
    HudBind* bind;
} HudBindSlot;

struct HudBind {
    u8 pad0[4];
    void* target;
};

typedef struct {
    u8 pad0[4];
    HudBindSlot* slots;
} HudBindOwner;

typedef struct {
    u16 flags;
    u16 field2;
    s8 parent;
    s8 children[4];
    u8 pad9[3];
    void* object;
} HudNode;

extern HudNode D_803A57F0[];
extern s16 func_800BE5D0(void* object);
extern void func_800BEEB4(u16 index);
extern s16 func_800BE0BC(HudBindOwner* owner, HudBindSlot* slot);

void func_800BF204(void) {
    s16 i;
    s16 j;
    s16 result;
    HudNode* node;
    HudBindOwner* owner;
    HudBindSlot* slot;

    for (i = 0; i < 20; i++) {
        node = &D_803A57F0[i];
        if (node->flags & 2) {
            result = func_800BE5D0(node->object);
            if (result == 1 && node->parent >= 0) {
                func_800BEEB4(i);
            } else if (result == 3) {
                j = i;
                while (D_803A57F0[j].parent >= 0) {
                    j = D_803A57F0[j].parent;
                }
                func_800BEEB4(j);
            }
        } else if (node->flags & 1) {
            owner = D_803A57F0[i].object;
            for (slot = owner->slots; slot->tag != 0; slot++) {
                if (slot->bind != 0 && slot->bind->target != 0) {
                    result = func_800BE0BC(owner, slot);
                    if (result == 1 || result == 3) {
                        break;
                    }
                }
            }
        }
    }
    for (i = 0; i < 20; i++) {
        node = &D_803A57F0[i];
        if (node->flags & 0x40) {
            node->flags = (node->flags & ~0x40) | 2;
        }
    }
}
