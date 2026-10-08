/* SPAN 0x800CF0CC */
/* Two functions (0x800CE9A0-0x800CF0CC). They reference the format strings at
 * 0x800740E0/0x800740EC owned by the 0x800CDBE0 unit (extern here). */
/* Exact via two label-gated func_800CEA50 normalizer rules (one reorder and
 * one register rename), not a pure C source match. See
 * docs/NORMALIZER_ASSISTED.md. */
#include "types.h"

typedef struct {
    u8 pad0[0x88];
    void* label;
    u8 pad8C[0x14];
    u8 colorA;
    u8 padA1[0xF];
    u8 colorB;
    u8 padB1[7];
    void* icon;
    u8 padBC[6];
    s16 iconY;
    u8 padC4[0xE];
    s16 frameY;
    u8 padD4[4];
    void* frame;
} ResultsPanel;

typedef struct {
    u8 pad0[4];
    ResultsPanel* panel;
} ResultsPanelRef;

typedef struct {
    u8 tag;
    u8 pad1[7];
    void* target;
    u8 padC[4];
} HudEntry;

typedef struct {
    u8 pad0[0x74];
    s32 team;
    u8 pad78[0x1D8];
} RacePlayer;

typedef struct {
    u8 pad0[4];
    HudEntry* bindings;
} ResultsMenu;

typedef struct {
    void* ptr;
    s32 pad;
} ResultsSlot;

typedef struct {
    u8 pad0[0x78];
    void* p78;
    u8 pad7C[0xC];
    void* p88;
    u8 pad8C[0xC];
    void* p98;
    u8 pad9C[0xC];
    void* pA8;
} ResultsWidget;

typedef struct {
    u8 pad0[4];
    ResultsWidget* widget;
    u8 pad8[0xC];
    u16 player;
} ResultsPage;

extern ResultsPanelRef D_80120760;
extern ResultsPanelRef D_801211C0;
extern u8 D_803A665C;
extern u8 D_801201BC[];
extern u8 D_801201CC[];
extern u8 D_80120134[];
extern u8 D_8012012C[];
extern s8 D_80117EB0;
extern s32 D_801177D8;
extern s32 D_801177F4;
extern s32 D_80117810;
extern s32 D_80116EC4;
extern s32 D_80117864;
extern s32 D_80117848;
extern s32 D_8011782C;
extern ResultsMenu D_80120840;
extern ResultsMenu D_80120940;
extern ResultsMenu D_80120A60;
extern ResultsMenu D_80120B90;
extern ResultsPage D_80121290;
extern ResultsPage D_80121360;
extern ResultsPage D_80121430;
extern ResultsPage D_80121500;
extern ResultsPage D_801215D0;
extern ResultsPage D_801216A0;
extern ResultsPage D_80121770;
extern s32 D_803A6640;
extern u8 D_802194A4[];
extern RacePlayer D_80235F00[];
extern u8 D_80120184[];
extern s32 D_803A65E0[];
extern s32 D_803A6588[];
extern u8 D_803A6638[];
extern u8 D_803A65FC[];
extern u8 D_803A65B4[];
extern s16 D_803A65F0[];
extern u16 D_803A6598[];
extern s16 D_803A65B8[];
extern u16 D_803A6648[];
extern char D_803A65C8[];
extern s32 D_803A65A0[];
extern s32 D_803A6628[];
extern u8 D_803A65D4[];
extern char D_803A6600[][10];
extern ResultsSlot D_80120528[];
extern ResultsSlot D_80120548[];
extern ResultsSlot D_80120568[];
extern char D_800740E0[];
extern char D_800740EC[];
extern s32 sprintf(char* buf, const char* fmt, ...);
extern void func_800CE188(s32 a0, s32 a1, s32 a2, s32 a3, u16 a4, u16 a5, u16 a6, u16 a7,
                          s32 t1, s32 t2, u16 a10, u16 a11, u16 a12);
extern void func_800BEBA8(void* arg0, s32 arg1, s32 arg2);

static inline RacePlayer* race_player_get(s32 id) {
    if (id == 0x7F) {
        return 0;
    }
    return &D_80235F00[id];
}

void func_800CE9A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4, u16 arg5, u16 arg6,
                   u16 arg7, s32 arg8, s32 arg9) {
    ResultsPanel* p = D_80120760.panel;

    p->icon = D_801201BC;
    p->frame = D_801201CC;
    p->iconY = 0xB9;
    p->frameY = 0xC8;
    p->label = D_80120134;
    p = D_801211C0.panel;
    D_803A665C = 0;
    p->colorA = 1;
    p->colorB = 1;
    func_800CE188(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, 0, 0, 0);
}

void func_800CEA50(u16 player, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 time1, s32 time2) {
    ResultsPage* page = 0;
    HudEntry* e;
    u16 i;
    u8 count;
    s32 h;
    s32 m;
    s32 s;
    s32 r;
    char* buf;
    s32 h2;
    s32 m2;
    s32 s2;
    s32 r2;

    if (player == 0) {
        D_801177D8 = 0;
        D_801177F4 = 0;
        D_80117810 = 0;
        D_80116EC4 = 0;
        D_80117864 = 0;
        D_80117848 = 0;
        D_8011782C = 0;
        switch (D_80117EB0) {
        case 2:
            page = (ResultsPage*)&D_80120940;
            break;
        case 3:
            page = (ResultsPage*)&D_80120A60;
            break;
        case 4:
            page = (ResultsPage*)&D_80120B90;
            break;
        case 1:
        default:
            page = (ResultsPage*)&D_80120840;
            break;
        }
        D_803A6640 = time2 / 120;
        if (D_803A6640 < 100) {
            D_803A6640 = 100;
        }
        for (e = ((ResultsMenu*)page)->bindings; e->tag != 6; e++) {
        }
        count = D_802194A4[0];
        for (i = 0; i < count; i++, e++) {
            RacePlayer* pl = race_player_get(i);

            if (pl->team == 1) {
                e->target = D_8012012C;
            } else {
                e->target = D_80120134;
            }
            count = D_802194A4[0];
        }
        D_80120184[0] = 'G';
        D_80120184[1] = 'a';
        D_80120184[2] = 'm';
        D_80120184[3] = 'e';
        func_800BEBA8(page, 0, 1);
    }
    switch (player) {
    case 0:
        if (D_80117EB0 < 4) {
            if (D_80117EB0 < 2) {
                if (D_80117EB0 != 1) {
                    goto other;
                }
                page = &D_80121290;
            } else {
                page = &D_80121360;
            }
        } else {
        other:
            page = &D_80121500;
        }
        break;
    case 1:
        if (D_80117EB0 != 3) {
            if (D_80117EB0 >= 4) {
                goto other1;
            }
            if (D_80117EB0 != 2) {
                goto other1;
            }
            page = &D_80121430;
        } else {
            page = &D_801216A0;
        }
        break;
    other1:
        page = &D_801215D0;
        break;
    case 2:
        page = &D_801216A0;
        if (D_80117EB0 == 2) {
            page = &D_80121770;
        }
        break;
    case 3:
        page = &D_80121770;
        break;
    }
    h = time1 / 108000;
    r = time1 - h * 108000;
    D_803A65E0[player] = arg1;
    m = r / 1800;
    r -= m * 1800;
    D_803A6588[player] = arg2;
    D_803A6638[player] = 0;
    D_803A65FC[player] = 0;
    D_803A65F0[player] = 0;
    D_803A65B4[player] = 0;
    D_803A6598[player] = arg3;
    D_803A65B8[player] = 0;
    D_803A6648[player] = arg4;
    s = r / 30;
    if (h == 0) {
        sprintf(D_803A65C8, D_800740E0, m, s);
    } else {
        sprintf(D_803A65C8, D_800740EC, h, m, s);
    }
    h2 = time2 / 108000;
    r2 = time2 - h2 * 108000;
    m2 = r2 / 1800;
    r2 -= m2 * 1800;
    D_803A65A0[player] = 0;
    D_803A6628[player] = time2;
    D_803A65D4[player] = 0;
    buf = D_803A6600[player];
    s2 = r2 / 30;
    if (h2 == 0) {
        sprintf(buf, D_800740E0, m2, s2);
    } else {
        sprintf(buf, D_800740EC, h2, m2, s2);
    }
    page->player = player;
    D_80120528[player].ptr = &D_803A65E0[player];
    D_80120548[player].ptr = &D_803A65F0[player];
    D_80120568[player].ptr = &D_803A65B8[player];
    page->widget->p78 = &D_80120528[player];
    page->widget->p88 = &D_80120548[player];
    page->widget->p98 = &D_80120568[player];
    page->widget->pA8 = D_803A6600[player];
    func_800BEBA8(page, 0, 1);
}
