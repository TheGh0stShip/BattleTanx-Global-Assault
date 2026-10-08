/* RODATA_VRAM 0x80073808 */
#include "types.h"

typedef struct { u16 x; u16 y; s32 value; s32 type; s32 mode; } HudOptionRec;
typedef struct { u8 pad0[4]; f32 scale; u16* size; } HudSprite;
typedef struct { u8 pad0[4]; HudSprite* sprite; } HudSub;
typedef struct {
    u8 color; u8 pad1; u16 x; u16 y; u8 pad6[2]; void* draw;
    HudSub* sub; u8 pad10[0x18]; s32 value;
} HudOption;
typedef struct { u8 pad0[0x68]; void* draw68; u8 pad6C[0xC]; void* draw78; } HudPanel;
typedef struct { u8 pad0[4]; HudPanel* panel; } HudMenu;
typedef struct { void* draw; } HudDrawRef;

extern u16 D_8011DC54;
extern u16 D_8011DC56;
extern HudOptionRec* D_803A5984;
extern s32 D_80117F44;
extern HudDrawRef D_8011B3FC;
extern u8 D_80116FD0[];
extern u8 D_8011705C[];
extern u8 D_8011B404[];
extern HudSprite D_8011B4CC;
extern u8 D_80116FEC[];
extern HudMenu D_8011B68C;
extern HudPanel* D_8011B690;
extern u8 D_80117078[];
extern u8 D_801170CC[];
extern u8 D_80117094[];
extern u8 D_801170E8[];
extern u8 D_801170B0[];
extern u8 D_80117104[];

void func_800C2528(HudMenu* menu, HudOption* option) {
    option->x = D_803A5984[D_8011DC54].x;
    option->y = D_803A5984[D_8011DC54].y;
    D_80117F44 = D_803A5984[D_8011DC54].mode;
    switch (D_803A5984[D_8011DC54].type) {
    case 1:
        option->color = 0x12;
        D_8011B3FC.draw = D_80116FD0;
        option->draw = D_8011B404;
        option->sub->sprite = &D_8011B4CC;
        option->sub->sprite->scale = *option->sub->sprite->size;
        option->value = D_803A5984[D_8011DC54].value;
        D_8011DC56 = 1;
        break;
    case 2:
        option->color = 0x12;
        D_8011B3FC.draw = D_8011705C;
        option->draw = D_8011B404;
        option->sub->sprite = &D_8011B4CC;
        option->sub->sprite->scale = *option->sub->sprite->size;
        option->value = D_803A5984[D_8011DC54].value;
        D_8011DC56 = 2;
        break;
    case 3:
        option->color = 10;
        option->draw = D_80116FEC;
        option->sub->sprite = 0;
        option->value = D_803A5984[D_8011DC54].value;
        if (menu == &D_8011B68C) D_8011B690->draw68 = D_80117078;
        else menu->panel->draw78 = D_801170CC;
        break;
    case 4:
        option->color = 10;
        option->draw = D_80116FEC;
        option->sub->sprite = 0;
        option->value = D_803A5984[D_8011DC54].value;
        if (menu == &D_8011B68C) D_8011B690->draw68 = D_80117094;
        else menu->panel->draw78 = D_801170E8;
        break;
    case 5:
    default:
        option->color = 10;
        option->draw = D_80116FEC;
        option->sub->sprite = 0;
        option->value = D_803A5984[D_8011DC54].value;
        if (menu == &D_8011B68C) D_8011B690->draw68 = D_801170B0;
        else menu->panel->draw78 = D_80117104;
        break;
    }
}
