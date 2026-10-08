/* SPAN 0x800C402C */
/* STATUS: func_800C30B4, func_800C3460, func_800C381C, func_800C3970,
 * func_800C3AC0 and func_800C3B10 match byte-for-byte when linked at their
 * own addresses (tools/fwdiff.sh); func_800C31D4 is a near miss (register
 * allocation and the shared draw tail), so the unit is not yet exact. */
/* RODATA_VRAM 0x800738E0 */
/* Seven functions; owns rodata 0x800738E0-0x80073AB0 (jump tables, three 1.0f
 * literals and padding). func_800C30B4 and func_800C3AC0 are `inline`
 * (extern linkage) and are also expanded inside func_800C3460 and
 * func_800C3B10. All other func_800C3xxx labels in the span are interior. */
#include "types.h"

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    u8 tag;
    u8 color;
    u8 pad2[6];
    void* target;
    u8 padC[4];
} HudEntry;

typedef struct {
    u8 pad0[4];
    HudEntry* bindings;
} HudItem;

typedef struct {
    u8 pad0[2];
    s16 x;
    s16 y;
} HudPos;

typedef struct {
    u8 pad0[2];
    u16 w;
    u16 h;
} HudSprite;

typedef struct {
    u8 pad0[0x16];
    u16 x;
    u16 y;
} HudMenuPos;

typedef struct {
    u32 color;
    HudSprite* sprite;
    void* name;
} HudChoice;

typedef struct {
    u8 pad0[0xE];
    s16 value;
} HudLabel;

typedef struct {
    u8 pad0[0x68];
    HudLabel* label0;
    u8 pad6C[0xC];
    HudLabel* label1;
    u8 pad7C[0xC];
    HudLabel* label2;
    u8 pad8C[0xC];
    HudLabel* label3;
    u8 pad9C[0xC];
    HudLabel* label4;
    u8 padAC[0x5C];
    void* name;
} HudChoiceWidget;

typedef struct {
    s32 value[5];
    u8 pad14[4];
    s32 type0;
    s32 type1;
} HudColorInfo;

typedef struct {
    u8 pad0[4];
    HudChoiceWidget* widget;
    u8 pad8[4];
    void* draw;
    void* update;
} HudChoicePage;

extern HudItem D_8011CD88;
extern HudItem D_8011CEF8;
extern HudItem D_8011D068;
extern HudItem D_8011D1C8;
extern HudItem D_8011C9A4;
extern s8 D_8011DC70[];
extern HudChoice D_8011D1E4[];
extern HudSprite D_801171C8;
extern u8 D_80125AB5;
extern u8 D_80125AB6;
extern u8 D_80122380;
extern s32 D_80117F34[];
extern void* D_8011CBC8[];
extern void* D_8011CBD8[];
extern u8 D_8011C388[];
extern u8 D_8011C3A4[];
extern u8 D_80117190[];
extern s32 D_80117ED4[];
extern u8 D_8011CA78[];
extern u8 D_8011CACC[];
extern u8 D_8011CB20[];
extern u8 D_8011CB74[];
extern u8 D_8011C418[][3];
extern u8 D_80117468[];
extern u8 D_8011736C[];
extern u8 D_80117334[];
extern u8 D_80117350[];
extern u8 D_801173A4[];
extern u8 D_801173C0[];
extern u8 D_801173F8[];
extern u8 D_801174A0[];
extern u8 D_801174BC[];
extern u8 D_80117484[];
extern u8 D_80117388[];
extern u8 D_8011744C[];
extern u8 D_80117430[];
extern u8 D_801174D8[];
extern u8 D_80117510[];
extern void func_8007BDFC(Gfx** dl, s32 mode);
extern void func_8007C9B8(Gfx** dl, HudSprite* sprite, s16 x, s16 y, f32 sx, f32 sy);
extern HudColorInfo* func_800D69E0(s32 index);

inline u8* func_800C30B4(s32 kind) {
    switch (kind) {
    case 1:
        return D_80117468;
    case 2:
        return D_8011736C;
    case 3:
        return D_80117334;
    case 4:
        return D_80117350;
    case 6:
        return D_801173A4;
    case 14:
        return D_801173C0;
    case 5:
        return D_801173F8;
    case 9:
        return D_801174A0;
    case 10:
        return D_801174BC;
    case 12:
        return D_80117484;
    case 7:
        return D_80117388;
    case 17:
        return D_8011744C;
    case 15:
        return D_80117430;
    case 16:
        return D_801174D8;
    case 13:
        return D_80117510;
    case 8:
    case 11:
    default:
        return 0;
    }
}

#define PRIM(pkt, w1v) { Gfx* _g = (pkt); _g->w0 = 0xFA000000; _g->w1 = (w1v); }
void func_800C31D4(Gfx** pdl, HudMenuPos* menu, HudPos* pos) {
    Gfx* dl = *pdl;
    u16 idx;
    s16 sel;
    HudSprite* sprite;

    s16 x;
    s16 y;
    u32 color;
    f32 scale;

    if (menu == (HudMenuPos*)&D_8011CD88) {
        idx = 0;
    } else if (menu == (HudMenuPos*)&D_8011CEF8) {
        idx = 1;
    } else if (menu == (HudMenuPos*)&D_8011D068) {
        idx = 2;
    } else if (menu == (HudMenuPos*)&D_8011D1C8) {
        idx = 3;
    } else {
        return;
    }
    sprite = &D_801171C8;
    sel = D_8011DC70[idx];
    x = pos->x;
    x -= (s16)D_801171C8.w >> 1;
    y = pos->y;
    y -= (s16)D_801171C8.h >> 1;
    switch (D_8011D1E4[(u16)sel].color) {
    case 0:
        func_8007BDFC(&dl, 2);
        dl->w0 = 0xFA000000;
        dl->w1 = 0xC8FF;
        dl++;
        func_8007C9B8(&dl, sprite, menu->x + x, menu->y + y, 1.0f, 1.0f);
        break;
    case 1:
        func_8007BDFC(&dl, 2);
        dl->w0 = 0xFA000000;
        dl->w1 = 0x00C800FF;
        dl++;
        func_8007C9B8(&dl, sprite, menu->x + x, menu->y + y, 1.0f, 1.0f);
        break;
    case 2:
        func_8007BDFC(&dl, 2);
        dl->w0 = 0xFA000000;
        dl->w1 = 0xC80000FF;
        dl++;
        func_8007C9B8(&dl, sprite, menu->x + x, menu->y + y, 1.0f, 1.0f);
        break;
    case 3:
        func_8007BDFC(&dl, 2);
        dl->w0 = 0xFA000000;
        dl->w1 = 0xFFC800FF;
        dl++;
        func_8007C9B8(&dl, sprite, menu->x + x, menu->y + y, 1.0f, 1.0f);
        break;
    }
    sprite = D_8011D1E4[(u16)sel].sprite;
    x = pos->x;
    y = pos->y;
    x -= (s16)sprite->w >> 1;
    y -= (s16)sprite->h >> 1;
    func_8007BDFC(&dl, 1);
    func_8007C9B8(&dl, sprite, menu->x + x, menu->y + y, 1.0f, 1.0f);
    *pdl = dl;
}

void func_800C3460(HudChoicePage* page, u16 idx) {
    s8 sel = D_8011DC70[idx];
    HudChoiceWidget* w;

    page->draw = D_8011C388;
    page->update = D_8011C3A4;
    w = page->widget;
    w->label0->value = func_800D69E0(D_8011D1E4[(u8)sel].color)->value[0];
    w->label1->value = func_800D69E0(D_8011D1E4[(u8)sel].color)->value[1];
    w->label2->value = func_800D69E0(D_8011D1E4[(u8)sel].color)->value[2];
    w->label3->value = func_800D69E0(D_8011D1E4[(u8)sel].color)->value[3];
    w->label4->value = func_800D69E0(D_8011D1E4[(u8)sel].color)->value[4];
    w->name = D_8011D1E4[(u8)sel].name;
    D_8011CBC8[idx] = func_800C30B4(func_800D69E0(D_8011D1E4[(u8)sel].color)->type0);
    D_8011CBD8[idx] = func_800C30B4(func_800D69E0(D_8011D1E4[(u8)sel].color)->type1);
}

s32 func_800C381C(HudItem* menu) {
    u16 idx;

    if (menu == &D_8011CD88) {
        idx = 0;
    } else if (menu == &D_8011CEF8) {
        idx = 1;
    } else if (menu == &D_8011D068) {
        idx = 2;
    } else if (menu == &D_8011D1C8) {
        idx = 3;
    } else {
        return 0;
    }
    D_8011DC70[idx]++;
    while ((D_8011DC70[idx] == 2 && D_80125AB5 == 0) || (D_8011DC70[idx] == 3 && D_80125AB6 == 0)) {
        D_8011DC70[idx]++;
    }
    if (D_8011DC70[idx] >= D_80122380 + 12) {
        D_8011DC70[idx] = 0;
    }
    func_800C3460((HudChoicePage*)menu, idx);
    D_80117F34[idx] = D_8011D1E4[D_8011DC70[idx]].color;
    return 0;
}

s32 func_800C3970(HudItem* menu) {
    u16 idx;

    if (menu == &D_8011CD88) {
        idx = 0;
    } else if (menu == &D_8011CEF8) {
        idx = 1;
    } else if (menu == &D_8011D068) {
        idx = 2;
    } else if (menu == &D_8011D1C8) {
        idx = 3;
    } else {
        return 0;
    }
    D_8011DC70[idx]--;
    while ((D_8011DC70[idx] == 2 && D_80125AB5 == 0) || (D_8011DC70[idx] == 3 && D_80125AB6 == 0)) {
        D_8011DC70[idx]--;
    }
    if (D_8011DC70[idx] < 0) {
        D_8011DC70[idx] = D_80122380 + 11;
    }
    func_800C3460((HudChoicePage*)menu, idx);
    D_80117F34[idx] = D_8011D1E4[D_8011DC70[idx]].color;
    return 0;
}

inline void func_800C3AC0(HudEntry* e, s32 mode) {
    switch (mode) {
    case 1:
    case 5:
        e->color = 1;
        break;
    case 2:
    case 6:
        e->color = 2;
        break;
    case 3:
        e->color = 3;
        break;
    case 4:
        e->color = 4;
        break;
    default:
        e->color = 0x10;
        break;
    }
}

void func_800C3B10(void) {
    HudEntry* e;
    u16 i;
    u8* dst;
    s32 c;

    for (e = D_8011C9A4.bindings; e->tag != 0; e++) {
        if (e->target == D_80117190) {
            break;
        }
    }
    func_800C3AC0(e, D_80117ED4[0]);
    func_800C3AC0(&e[2], D_80117ED4[1]);
    func_800C3AC0(&e[3], D_80117ED4[2]);
    func_800C3AC0(&e[5], D_80117ED4[3]);
    for (e = D_8011CD88.bindings; e->tag != 0; e++) {
        if (e->target == D_80117190) {
            break;
        }
    }
    func_800C3AC0(e, D_80117ED4[0]);
    for (e = D_8011CEF8.bindings; e->tag != 0; e++) {
        if (e->target == D_80117190) {
            break;
        }
    }
    func_800C3AC0(e, D_80117ED4[1]);
    for (e = D_8011D068.bindings; e->tag != 0; e++) {
        if (e->target == D_80117190) {
            break;
        }
    }
    func_800C3AC0(e, D_80117ED4[2]);
    for (e = D_8011D1C8.bindings; e->tag != 0; e++) {
        if (e->target == D_80117190) {
            break;
        }
    }
    func_800C3AC0(e, D_80117ED4[3]);
    for (i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            dst = D_8011CA78;
            break;
        case 1:
            dst = D_8011CACC;
            break;
        case 2:
            dst = D_8011CB20;
            break;
        default:
            dst = D_8011CB74;
            break;
        }
        switch (D_80117ED4[i]) {
        case 1:
        case 5:
            c = 0;
            break;
        case 2:
        case 6:
            c = 1;
            break;
        case 4:
            c = 3;
            break;
        default:
            c = 2;
            break;
        }
        dst[0] = D_8011C418[c][0];
        dst[1] = D_8011C418[c][1];
        dst[2] = D_8011C418[c][2];
    }
}
