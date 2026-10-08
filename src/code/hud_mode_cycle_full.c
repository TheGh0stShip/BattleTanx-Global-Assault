/* RODATA_VRAM 0x80073628 */
#include "types.h"

typedef struct {
    u8 pad0[8];
    void* table;
    u8 padC[4];
} HudModeEntry;

extern u32 D_80117EB4;
extern s8 D_80117EB0;
extern u8 D_80118590[];
extern u8 D_8011859C[];
extern u8 D_801185A8[];
extern u8 D_801185B4[];
extern u8 D_801185BC[];
extern u8 D_801185C4[];
extern u8 D_801185D0[];
extern u8 D_801185DC[];

s32 func_800C0608(s32 arg0, HudModeEntry* entry) {
    D_80117EB4--;
    if (D_80117EB0 < 3) {
        if (D_80117EB4 == 0) D_80117EB4 = 8;
    } else if (D_80117EB4 < 2) {
        D_80117EB4 = 8;
    }
    entry++;
    switch (D_80117EB4) {
    case 4: entry->table = D_801185B4; break;
    case 5: entry->table = D_801185BC; break;
    case 3: entry->table = D_8011859C; break;
    case 2: entry->table = D_801185A8; break;
    case 6: entry->table = D_801185C4; break;
    case 7: entry->table = D_801185D0; break;
    case 8: entry->table = D_801185DC; break;
    case 1:
    default: entry->table = D_80118590; break;
    }
    return 0;
}

s32 func_800C0704(s32 arg0, HudModeEntry* entry) {
    D_80117EB4++;
    if (D_80117EB0 < 3) {
        if (D_80117EB4 >= 9) D_80117EB4 = 1;
    } else if (D_80117EB4 >= 9) {
        D_80117EB4 = 2;
    }
    entry++;
    switch (D_80117EB4) {
    case 4: entry->table = D_801185B4; break;
    case 5: entry->table = D_801185BC; break;
    case 3: entry->table = D_8011859C; break;
    case 2: entry->table = D_801185A8; break;
    case 6: entry->table = D_801185C4; break;
    case 7: entry->table = D_801185D0; break;
    case 8: entry->table = D_801185DC; break;
    case 1:
    default: entry->table = D_80118590; break;
    }
    return 0;
}
