/* RODATA_VRAM 0x800740E0 */
#include "types.h"

extern s32 sprintf(char* buf, const char* fmt, ...);
extern s32 D_801177D8;
extern s32 D_801177F4;
extern s32 D_80117810;
extern s32 D_80116EC4;
extern s32 D_80117864;
extern s32 D_80117848;
extern s32 D_8011782C;
extern s16 D_803A65C0;
extern s16 D_803A65B0;

void func_800CDBE0(s32 frames, char* buf) {
    s32 hours = frames / 108000;
    s32 minutes;
    s32 seconds;

    frames -= hours * 108000;
    minutes = frames / 1800;
    frames -= minutes * 1800;
    seconds = frames / 30;
    if (hours == 0) sprintf(buf, "%01d:%02d", minutes, seconds);
    else sprintf(buf, "%01d:%02d:%02d", hours, minutes, seconds);
}

void func_800CDCCC(void) {
    D_801177D8 = 0;
    D_801177F4 = 0;
    D_80117810 = 0;
    D_80116EC4 = 0;
    D_80117864 = 0;
    D_80117848 = 0;
    D_8011782C = 0;
}

void func_800CDD0C(u16 value) {
    if (value <= 500) {
        D_803A65C0 = 25;
        D_803A65B0 = 20;
    } else if (value <= 750) {
        D_803A65C0 = 25;
        D_803A65B0 = 13;
    } else if (value <= 1000) {
        D_803A65C0 = 12;
        D_803A65B0 = 20;
    } else {
        D_803A65C0 = 12;
        D_803A65B0 = 13;
    }
}
