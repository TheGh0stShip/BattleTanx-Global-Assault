/* RODATA_VRAM 0x80073800 */
#include "types.h"

typedef struct {
    u8 pad0[0x18];
    void* next;
} HudMenu;

extern u16 D_803A5970;
extern u16 D_80121CC0;
extern u8 D_80119C20[];
extern u8 D_80119C3C[];
extern s32 D_8011A258;
extern f32 D_8011950C;
extern s16 D_8011DC58;
extern void func_800D6B80(void* arg0);
extern s32 func_800D0070(void* arg0, s32 arg1);

s32 func_800C1484(s32 arg0, HudMenu* menu) {
    s32 result;

    if (D_803A5970 != 0) return 0;
    if (D_80121CC0 != 0) {
        func_800D6B80(D_80119C20);
        D_803A5970 = 1;
        return 0;
    }
    result = func_800D0070(D_80119C20, arg0);
    if (result == 0) {
        menu->next = D_80119C3C;
        return 0;
    }
    D_8011A258 = result;
    if (D_80121CC0 != 0) {
        D_80119C20[0] = 0;
        D_8011DC58 = 0;
        return 0;
    }
    D_803A5970 = 1;
    D_8011950C = 30.0f;
    return 0;
}
