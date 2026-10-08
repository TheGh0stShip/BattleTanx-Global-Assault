/* RODATA_VRAM 0x80073B48 */
#include "types.h"

typedef struct {
    u8 tag; u8 color; u8 pad2[6]; void* target; u8 padC[4];
} HudEntry;
typedef struct {
    u8 pad0[4]; HudEntry* widget; u8 pad8[4]; void* draw; void* update;
} HudPage;

extern u8 D_8011D408[];
extern u8 D_8011D424[];
extern u8 D_8011D9E8[];
extern u8 D_8011D9F0[];
extern u8 D_8011D9FC[];
extern u8 D_8011DA04[];
extern u8 D_8011DA10[];
extern u8 D_8011DA1C[];
extern u8 D_8011DA28[];
extern u8 D_8011DA34[];
extern u8 D_8011DA40[];
extern u8 D_8011DA4C[];
extern u8 D_8011DA58[];
extern u8 D_8011DA64[];
extern u8 D_80118EDC[];
extern u8 D_80118EE0[];
extern u32 D_80117EF4[];
extern u32 D_80117ED4[];
extern s32 D_80117F04[];

void func_800C4BB4(HudPage* page, u16 player) {
    HudEntry* w = page->widget;
    HudEntry* e;
    HudEntry* valueEntry;

    page->draw = D_8011D408;
    page->update = D_8011D424;
    w[1].target = D_8011D9E8;
    w[3].color = 0x90;
    e = &w[3];
    switch (D_80117EF4[player]) {
    case 1: e->target = D_8011DA10; break;
    case 3: e->target = D_8011DA04; break;
    case 2:
    default: e->target = D_8011D9FC; break;
    }
    w[4].color = 0x10;
    valueEntry = &w[4];
    switch (D_80117ED4[player]) {
    case 1: valueEntry->target = D_8011DA28; break;
    case 2: valueEntry->target = D_8011DA34; break;
    case 3: valueEntry->target = D_8011DA1C; break;
    case 4: valueEntry->target = D_8011DA40; break;
    case 6: valueEntry->target = D_8011DA4C; break;
    case 5: valueEntry->target = D_8011DA58; break;
    case 0:
    default: valueEntry->target = D_8011DA64; break;
    }
}

void func_800C4D10(HudPage* page, u16 player) {
    HudEntry* w = page->widget;
    HudEntry* e;

    page->draw = D_8011D408;
    page->update = D_8011D424;
    w[1].target = D_8011D9F0;
    w[3].color = 1;
    w[4].color = 1;
    e = &w[4];
    switch (D_80117ED4[player]) {
    case 1: e->target = D_8011DA28; break;
    case 2: e->target = D_8011DA34; break;
    case 3: e->target = D_8011DA1C; break;
    case 4: e->target = D_8011DA40; break;
    case 6: e->target = D_8011DA4C; break;
    case 5: e->target = D_8011DA58; break;
    case 0:
    default: e->target = D_8011DA64; break;
    }
    if (D_80117F04[player] == 0) w[3].target = D_80118EE0;
    else w[3].target = D_80118EDC;
}
