#include "types.h"

typedef struct {
    u8 pad0[8];
    u32 flags;
} HudInfo;

typedef struct {
    u8 tag;
    u8 pad1[7];
    s32 value;
    u8 pad2[4];
} HudBinding;

typedef struct {
    u8 pad0[4];
    HudBinding* bindings;
    u8 pad8[0xC];
    u16 id;
} HudItem;

typedef struct {
    u32 mask;
    s32 value;
} HudFlagValue;

typedef struct {
    u8 pad0[0x18];
    s32 value;
} HudTarget;

extern u32 D_80117F24[];
extern u16 D_803A5988;
extern HudFlagValue D_8011B0D4[];
extern u8 D_8011A5C8[];
extern u8 D_8011AB78[];
extern u8 D_8011AD38[];
extern u8 D_8011AEF8[];
extern u8 D_8011B0B8[];
extern HudInfo* func_8009836C(u16 id);
extern void func_800C1F08(u16 id, s32 mode, HudItem* item);
extern void func_800BEBA8(void* arg0, s32 arg1, s32 arg2);
extern void func_800BF130(void* arg0);

s32 func_800C2284(HudItem* item) {
    u32 flags;
    s32 mode;

    flags = func_8009836C(item->id)->flags;
    if (flags & 0x8000) {
        u16 id = item->id;

        switch (D_80117F24[id]) {
        case 1:
            mode = 2;
            break;
        case 2:
            mode = 4;
            break;
        case 3:
            mode = 1;
            break;
        case 4:
            mode = 3;
            break;
        default:
            mode = 2;
            break;
        }
        func_800C1F08(id, mode, item);
        return 0;
    }
    if (flags & 0x2000) {
        if (D_803A5988 != 0) {
            func_800BEBA8(D_8011A5C8, 0, 0);
            func_800BF130(D_8011AB78);
            func_800BF130(D_8011AD38);
            func_800BF130(D_8011AEF8);
            func_800BF130(D_8011B0B8);
        }
    }
    return 0;
}

s32 func_800C2390(HudItem* item, HudTarget* target) {
    HudInfo* info;
    u16 i;
    HudBinding* b;

    info = func_8009836C(item->id);
    for (i = 0; i < 9; i++) {
        if (info->flags & D_8011B0D4[i].mask) {
            break;
        }
    }
    if (i >= 9) {
        return 0;
    }
    for (b = item->bindings; b->tag != 0; b++) {
        if (b->value == D_8011B0D4[i].value) {
            break;
        }
    }
    if (b->tag != 0) {
        b->value = target->value;
    }
    target->value = D_8011B0D4[i].value;
    return 0;
}
