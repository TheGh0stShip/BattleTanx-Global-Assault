/* RODATA_VRAM 0x80073608 */
#include "types.h"

typedef struct {
    u8 pad0[8];
    void* table;
} HudModeOwner;

extern s32 D_80117EB4;
extern u8 D_80118590[];
extern u8 D_8011859C[];
extern u8 D_801185A8[];
extern u8 D_801185B4[];
extern u8 D_801185BC[];
extern u8 D_801185C4[];
extern u8 D_801185D0[];
extern u8 D_801185DC[];

void func_800C041C(HudModeOwner* arg0) {
    void* table;

    switch (D_80117EB4) {
    case 4: table = D_801185B4; break;
    case 5: table = D_801185BC; break;
    case 3: table = D_8011859C; break;
    case 2: table = D_801185A8; break;
    case 6: table = D_801185C4; break;
    case 7: table = D_801185D0; break;
    case 8: table = D_801185DC; break;
    case 1:
    default: table = D_80118590; break;
    }
    arg0->table = table;
}
