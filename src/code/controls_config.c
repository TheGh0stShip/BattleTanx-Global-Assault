/* SPAN 0x800C1E48 */
/* Three functions. func_800C1AE4, func_800C1BA4 and func_800C1BB0 are
 * interior labels. */
/* Exact via the label-gated func_800C1938 normalizer rules, including an
 * inserted dead indexed load. This is not a pure C source match and the dead
 * load's source/compiler cause remains open. See docs/NORMALIZER_ASSISTED.md. */
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

typedef struct {
    void* key;
    u16 value;
} HudLookup;

typedef struct {
    u32 mask;
    void* value;
} HudFlagValue;

extern s32 D_80117EB4;
extern u8 D_8011A8D4[];
extern HudLookup D_8011B124[];
extern HudFlagValue D_8011B0D4[];
extern s32 D_80117F14[];
extern s32 D_80117F24[];
extern s8 D_80117EB0;
extern s32 D_80219208[];
extern u8 D_8023606C[][0x250];
extern HudItem D_8011AB78;
extern HudItem D_8011AD38;
extern HudItem D_8011AEF8;
extern HudItem D_8011B0B8;
extern void* D_8011B1A4[];
extern u8 D_8011A97C[];

void func_800C1938(u32* cfg, HudItem* item, u16 player) {
    HudEntry* e;
    HudLookup* p;
    HudFlagValue* q;
    u16 i;
    u16 idx;
    s32 v;

    for (e = item->bindings; e->target != D_8011A8D4; e++) {
    }
    for (i = 0; i < 17; i++) {
        cfg[i] = 0;
    }
    if (D_80117EB4 == 6) {
        cfg[0] = 0x2000;
        cfg[3] = 0x20;
        cfg[4] = 8;
        cfg[5] = 4;
        cfg[12] = 1;
        cfg[13] = 2;
        cfg[8] = 0x50000000;
        cfg[9] = 0x50000000;
        cfg[10] = 0xA0000000;
        cfg[11] = 0xA0000000;
        cfg[14] = 0xA0000000;
        cfg[15] = 0xA0000000;
        D_80117F14[player] = 4;
        return;
    }
    for (i = 0; i < 9; i++) {
        for (p = D_8011B124; p->key != 0; p++) {
            if (p->key == e->target) {
                break;
            }
        }
        idx = p->value;
        e++;
        if (idx < 17) {
            for (q = D_8011B0D4; q->value != 0; q++) {
                if (q->value == e->target) {
                    break;
                }
            }
            cfg[idx] = q->mask;
        }
        e++;
    }
    cfg[8] = 0x50000000;
    cfg[9] = 0x50000000;
    cfg[10] = 0xA0000000;
    cfg[11] = 0xA0000000;
    cfg[14] = 0xA0000000;
    cfg[15] = 0xA0000000;
    v = D_80117F24[player];
    D_80117F14[player] = v;
}

void func_800C1AF8(u16 player) {
    u32* cfg;

    if (player < D_80117EB0) {
        cfg = (u32*)D_8023606C[player];
        switch (D_80219208[player]) {
        case 1:
            func_800C1938(cfg, &D_8011AD38, player);
            break;
        case 2:
            func_800C1938(cfg, &D_8011AEF8, player);
            break;
        case 3:
            func_800C1938(cfg, &D_8011B0B8, player);
            break;
        case 0:
        default:
            func_800C1938(cfg, &D_8011AB78, player);
            break;
        }
    }
}

static inline void controls_reset_item(HudItem* item) {
    HudEntry* e;
    void** src = D_8011B1A4;
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

void func_800C1BC8(void) {
    HudEntry* b;
    u16 i;

    controls_reset_item(&D_8011AB78);
    controls_reset_item(&D_8011AD38);
    controls_reset_item(&D_8011AEF8);
    controls_reset_item(&D_8011B0B8);
    for (i = 0; i < 4; i++) {
        D_80117F24[i] = 2;
    }
    b = D_8011AB78.bindings;
    b[24].target = D_8011A97C;
    b = D_8011AD38.bindings;
    b[24].target = D_8011A97C;
    b = D_8011AEF8.bindings;
    b[24].target = D_8011A97C;
    b = D_8011B0B8.bindings;
    b[24].target = D_8011A97C;
}
