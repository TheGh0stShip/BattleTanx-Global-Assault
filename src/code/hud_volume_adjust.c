/* SPAN 0x800C1094 */
/* RODATA_VRAM 0x80073680 */
/* Owns rodata 0x80073680-0x8007368C (-2.0f, 8.0f, -2.0f). func_800C0E28,
 * func_800C0E3C, func_800C0FC0, func_800C0FD4, func_800C1024 and
 * func_800C104C are interior labels. */
#include "types.h"

typedef struct {
    s8 stick;
    u8 pad1[3];
    u32 buttons;
} HudInput;

typedef struct {
    u8 pad0[0x14];
    u32 decMask;
    u32 incMask;
} HudButtons;

typedef struct {
    u8 pad0[8];
    s16 x;
} HudBar;

typedef struct HudEntry {
    u8 tag;
    u8 pad1;
    s16 x;
    u8 pad4[4];
    void* target;
    u8 padC[4];
} HudEntry;

typedef struct {
    u8 pad0[4];
    HudEntry* bindings;
    u8 pad8[4];
    HudButtons* buttons;
    u8 pad10[4];
    u16 id;
} HudItem;

extern u8 D_80118E44[];
extern u8 D_80118E6C[];
extern u8 D_80118E78[];
extern f32 D_803A5948;
extern f32 D_8011DC5C;
extern s8 D_80117ED0;
extern s8 D_80117ED1;
extern HudEntry* func_800BD880(HudItem* item);
extern HudInput* func_8009836C(u16 id);
extern void func_800979F4(s32 sound);
extern void MusSetMasterVolume(s32 type, s32 volume);
extern void func_80097C1C(s32 volume);

s32 func_800C0D00(HudItem* item) {
    HudEntry* e;
    s16 count;
    s16 step;
    s8 value;
    s32 pos;
    HudBar* bar;

    count = 0;
    for (e = item->bindings; e->tag != 0 && count < 2; e++) {
        if (e->target == D_80118E44) {
            count++;
            e->tag = 1;
        }
    }
    e = func_800BD880(item);
    if (e->target == D_80118E6C) {
        HudInput* input = func_8009836C(item->id);
        if ((u8)(input->stick + 10) > 20) {
            step = (input->stick >> 4) * D_803A5948;
        } else if (input->buttons & item->buttons->incMask) {
            step = D_803A5948 + D_803A5948;
        } else if (input->buttons & item->buttons->decMask) {
            step = D_803A5948 * -2.0f;
        } else {
            step = 0;
        }
        if (step == 0) {
            goto done;
        }
        D_8011DC5C -= D_803A5948;
        if (D_8011DC5C < 0.0f) {
            func_800979F4(0x2D);
            D_8011DC5C += 8.0f;
        }
        D_80117ED0 += step;
        if (D_80117ED0 >= 0x5C) {
            D_80117ED0 = 0x5B;
        } else if (D_80117ED0 < 0) {
            D_80117ED0 = 0;
        }
        MusSetMasterVolume(1, D_80117ED0 * 360);
        value = D_80117ED0;
    } else if (e->target == D_80118E78) {
        HudInput* input = func_8009836C(item->id);
        if ((u8)(input->stick + 10) > 20) {
            step = (input->stick >> 4) * D_803A5948;
        } else if (input->buttons & item->buttons->incMask) {
            step = D_803A5948 + D_803A5948;
        } else if (input->buttons & item->buttons->decMask) {
            step = D_803A5948 * -2.0f;
        } else {
            step = 0;
        }
        if (step == 0) {
            goto done;
        }
        D_80117ED1 += step;
        if (D_80117ED1 >= 0x5C) {
            D_80117ED1 = 0x5B;
        } else if (D_80117ED1 < 0) {
            D_80117ED1 = 0;
        }
        func_80097C1C(D_80117ED1 * 360);
        value = D_80117ED1;
    } else {
        goto done;
    }
    bar = (HudBar*)e[1].target;
    pos = value;
    bar->x = pos + 11;
    e[2].x = pos + 0x1C;
    e[3].x = pos + 0x20;
    e[2].tag = 0x12;
done:
    return 0;
}
