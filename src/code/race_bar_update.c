#include "types.h"

typedef struct HudBar {
    u8 pad0[8];
    u16 width;
} HudBar;

typedef struct HudEntry {
    u8 tag;
    u8 color;
    u8 pad2[6];
    HudBar *bar;
} HudEntry;

typedef struct HudGauge {
    s32 *current;
    s32 *maximum;
    s32 last_current;
    s32 last_maximum;
} HudGauge;

extern s8 D_80117EB0;
extern HudGauge D_803A5FF0[];
extern u8 D_8011DEAC[];
extern u8 D_8011DEB8[];
extern u8 D_8011DEC4[];
extern f32 D_80073EE4;
extern f32 D_80073EE8;
extern f64 D_80073EF0;
extern f64 D_80073EF8;

s32 func_800C98E8(s32 unused, HudEntry *entry) {
    HudBar *bar = entry->bar;
    u8 index;
    f32 scale;
    f32 ratio;
    HudGauge *gauge;

    if ((void *)bar == D_8011DEAC) {
        index = 0;
    } else if ((void *)bar == D_8011DEB8) {
        index = 1;
    } else if ((void *)bar == D_8011DEC4) {
        index = 2;
    } else {
        index = 3;
    }

    scale = D_80073EE4;
    if (D_80117EB0 >= 2) {
        scale = D_80073EE8;
    }

    gauge = &D_803A5FF0[index];
    if (gauge->maximum == 0) {
        bar->width = 0;
        return 0;
    }
    if (gauge->last_current != *gauge->current ||
        gauge->last_maximum != *gauge->maximum) {
        gauge->last_current = *gauge->current;
        gauge->last_maximum = *gauge->maximum;
        ratio = (f32)gauge->last_maximum / (f32)gauge->last_current;
        bar->width = (u32)(scale * ratio);
        if (ratio < D_80073EF0) {
            entry->color = 1;
        } else if (ratio < D_80073EF8) {
            entry->color = 2;
        } else {
            entry->color = 3;
        }
    }
    return 0;
}
