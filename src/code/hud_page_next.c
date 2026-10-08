/* SPAN 0x800C674C */
/* func_800C665C, func_800C66E8 and func_800C6734 are interior labels. */
#include "types.h"

typedef struct {
    u32 pad0[2];
    u32 flags;
} HudInfo;

typedef struct {
    u8 pad0[0x18];
    void* icon;
    u8 pad1C[0x15];
    u8 color0;
    u8 pad32[6];
    void* label;
    u8 pad3C[5];
    u8 color1;
} HudWidget;

typedef struct HudPage {
    u8 pad0[4];
    HudWidget* widget;
    u8 pad8[4];
    void* draw;
    void* update;
    u16 id;
    u8 pad16[0x5A];
} HudPage;

extern HudPage D_8011DAE4;
extern HudPage D_8011DB54;
extern HudPage D_8011DBC4;
extern HudPage D_8011DC34;
extern u16 D_8011DC80;
extern u8 D_8011C3B8[];
extern u8 D_8011D438[];
extern u8 D_8011D408[];
extern u8 D_8011D424[];
extern u8 D_8011D9F0[];
extern u8 D_80118EE0[];
extern HudInfo* func_8009836C(u16 id);
extern void func_800BF130(HudPage* page);
extern void func_800BF1A4(HudPage* page);

s32 func_800C65B4(HudPage* page) {
    HudPage* next;
    HudWidget* widget;
    u16 i;
    u16 n;

    if (func_8009836C(page->id)->flags & 0x20) {
        if (page->id == 0) {
            next = page;
            i = 0;
            do {
                i++;
                if (next == &D_8011DAE4) {
                    next = &D_8011DB54;
                } else if (next == &D_8011DB54) {
                    next = &D_8011DBC4;
                } else if (next == &D_8011DBC4) {
                    next = &D_8011DC34;
                } else {
                    next = &D_8011DAE4;
                }
            } while (i < 3 && next->id != 0);
            if (i < 4) {
                widget = page->widget;
                page->draw = D_8011D408;
                page->update = D_8011D424;
                widget->color0 = 1;
                widget->color1 = 1;
                func_800BF130(page);
                widget = next->widget;
                widget->color0 = 0x90;
                if (widget->icon == D_8011D9F0 && widget->label == D_80118EE0) {
                    widget->color1 = 1;
                } else {
                    widget->color1 = 0x10;
                }
                func_800BF1A4(next);
            }
        }
        return 0;
    } else {
        n = D_8011DC80;
        n--;
        D_8011DC80 = n;
        widget = page->widget;
        page->draw = D_8011C3B8;
        page->update = D_8011D438;
        widget->color0 = 1;
        widget->color1 = 1;
    }
    return 0;
}
