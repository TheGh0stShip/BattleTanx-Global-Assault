#include "types.h"

typedef struct {
    u8 tag;
    u8 flags;
    u8 pad2[6];
    void* target;
    u8 padC[4];
} HudEntry;

typedef struct {
    u8 pad0[4];
    HudEntry* bindings;
} HudItem;

extern s32 D_80117F24[];
extern u8 D_8011A974[];
extern u8 D_8011A97C[];
extern u8 D_8011A984[];
extern u8 D_8011A990[];
extern void* D_8011B1A4[];
extern void* D_8011B1EC[];
extern void* D_8011B234[];
extern void* D_8011B27C[];

static inline void controls_apply_preset(HudItem* item, void** src) {
    HudEntry* e;
    u16 i;

    for (e = item->bindings; e->tag != 4; e++) {
    }
    for (i = 0; i < 9; i++) {
        e->target = *src++;
        if (e->target == 0) {
            e->flags &= ~0x10;
        } else {
            e->flags |= 0x10;
        }
        e++;
        e->target = *src++;
        e++;
    }
}

void func_800C1F08(u16 player, u32 preset, HudItem* item) {
    HudEntry* b = item->bindings;

    D_80117F24[player] = preset;
    switch (preset) {
    case 3:
        b[24].target = D_8011A990;
        controls_apply_preset(item, D_8011B1EC);
        break;
    case 1:
        b[24].target = D_8011A974;
        controls_apply_preset(item, D_8011B234);
        break;
    case 4:
        b[24].target = D_8011A984;
        controls_apply_preset(item, D_8011B27C);
        break;
    case 2:
    default:
        b[24].target = D_8011A97C;
        controls_apply_preset(item, D_8011B1A4);
        break;
    }
}
